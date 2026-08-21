#include "YinPitchDetector.h"
#include <algorithm>
#include <cmath>

namespace
{
    float clamp01 (float value)
    {
        return std::max (0.0f, std::min (1.0f, value));
    }
}

void YinPitchDetector::prepare (const PitchDetectorSettings& newSettings)
{
    settings = newSettings;

    const auto halfWindow = std::max (settings.windowSize / 2, 2);
    differenceBuffer.assign ((size_t) halfWindow, 0.0f);
    cmndBuffer.assign ((size_t) halfWindow, 0.0f);

    tauMin = std::max ((int) (settings.sampleRate / settings.maxFrequencyHz), 2);
    tauMax = std::min ((int) (settings.sampleRate / settings.minFrequencyHz), halfWindow - 1);
}

void YinPitchDetector::computeDifferenceFunction (const float* samples)
{
    const auto halfWindow = (int) differenceBuffer.size();

    differenceBuffer[0] = 0.0f;

    // O(halfWindow^2). Fine for the window sizes used here (called at a
    // fixed analysis cadence, not necessarily once per audio callback --
    // see PluginProcessor for how input is buffered before this runs).
    for (int tau = 1; tau < halfWindow; ++tau)
    {
        float sum = 0.0f;

        for (int j = 0; j < halfWindow; ++j)
        {
            const auto delta = samples[j] - samples[j + tau];
            sum += delta * delta;
        }

        differenceBuffer[(size_t) tau] = sum;
    }
}

void YinPitchDetector::computeCumulativeMeanNormalizedDifference()
{
    const auto halfWindow = (int) differenceBuffer.size();

    cmndBuffer[0] = 1.0f;
    float runningSum = 0.0f;

    for (int tau = 1; tau < halfWindow; ++tau)
    {
        runningSum += differenceBuffer[(size_t) tau];

        cmndBuffer[(size_t) tau] = runningSum > 0.0f
            ? differenceBuffer[(size_t) tau] * (float) tau / runningSum
            : 1.0f;
    }
}

int YinPitchDetector::findAbsoluteThresholdMinimum() const
{
    if (tauMax <= tauMin)
        return -1;

    for (int tau = tauMin; tau <= tauMax; ++tau)
    {
        if (cmndBuffer[(size_t) tau] < settings.yinThreshold)
        {
            // Walk forward to the local minimum, as the original YIN paper
            // recommends -- the first dip below threshold isn't always the
            // best estimate.
            int localTau = tau;

            while (localTau + 1 <= tauMax
                   && cmndBuffer[(size_t) (localTau + 1)] < cmndBuffer[(size_t) localTau])
                ++localTau;

            return localTau;
        }
    }

    // Nothing dipped below threshold: likely silence, noise, or polyphonic
    // input we can't reliably pitch. Deliberately not falling back to the
    // global minimum here -- for exactly these cases that's often wrong,
    // and "no result" is the safer answer (PROJECT.md section 20).
    return -1;
}

double YinPitchDetector::parabolicInterpolation (int tauEstimate) const
{
    const auto halfWindow = (int) cmndBuffer.size();

    if (tauEstimate <= 0 || tauEstimate >= halfWindow - 1)
        return (double) tauEstimate;

    const auto s0 = cmndBuffer[(size_t) (tauEstimate - 1)];
    const auto s1 = cmndBuffer[(size_t) tauEstimate];
    const auto s2 = cmndBuffer[(size_t) (tauEstimate + 1)];

    const auto denom = 2.0 * (2.0 * s1 - s2 - s0);

    if (std::abs (denom) < 1.0e-9)
        return (double) tauEstimate;

    const auto shift = (s2 - s0) / denom;
    return tauEstimate + shift;
}

PitchResult YinPitchDetector::detectPitch (const float* samples, int numSamples)
{
    if (samples == nullptr || numSamples < settings.windowSize || tauMax <= tauMin)
        return {};

    computeDifferenceFunction (samples);
    computeCumulativeMeanNormalizedDifference();

    const auto tau = findAbsoluteThresholdMinimum();

    if (tau < 0)
        return {};

    const auto refinedTau = parabolicInterpolation (tau);

    if (refinedTau <= 0.0)
        return {};

    const auto frequency = settings.sampleRate / refinedTau;

    if (! std::isfinite (frequency)
        || frequency < settings.minFrequencyHz
        || frequency > settings.maxFrequencyHz)
        return {};

    PitchResult result;
    result.valid = true;
    result.frequencyHz = frequency;
    result.confidence = clamp01 (1.0f - cmndBuffer[(size_t) tau]);
    return result;
}
