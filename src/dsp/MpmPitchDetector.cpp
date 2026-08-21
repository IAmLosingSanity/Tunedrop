#include "MpmPitchDetector.h"
#include <algorithm>
#include <cmath>

void MpmPitchDetector::prepare (const PitchDetectorSettings& newSettings)
{
    settings = newSettings;

    const auto halfWindow = std::max (settings.windowSize / 2, 2);
    nsdfBuffer.assign ((size_t) halfWindow, 0.0f);

    peakBuffer.clear();
    peakBuffer.reserve ((size_t) halfWindow);

    tauMin = std::max ((int) (settings.sampleRate / settings.maxFrequencyHz), 2);
    tauMax = std::min ((int) (settings.sampleRate / settings.minFrequencyHz), halfWindow - 1);
}

void MpmPitchDetector::computeNsdf (const float* samples)
{
    const auto halfWindow = (int) nsdfBuffer.size();

    // NSDF(tau) = 2 * ACF(tau) / m'(tau), where ACF is the (unnormalized)
    // autocorrelation and m'(tau) is the sum of squared energy of the two
    // overlapping segments being compared. Ranges roughly -1..1; 1 means
    // perfect periodicity at that lag. O(halfWindow^2), same order as
    // YIN's difference function.
    for (int tau = 0; tau < halfWindow; ++tau)
    {
        float acf = 0.0f;
        float m = 0.0f;

        for (int j = 0; j < halfWindow - tau; ++j)
        {
            acf += samples[j] * samples[j + tau];
            m += samples[j] * samples[j] + samples[j + tau] * samples[j + tau];
        }

        nsdfBuffer[(size_t) tau] = m > 0.0f ? 2.0f * acf / m : 0.0f;
    }
}

double MpmPitchDetector::parabolicInterpolation (int tauEstimate) const
{
    const auto halfWindow = (int) nsdfBuffer.size();

    if (tauEstimate <= 0 || tauEstimate >= halfWindow - 1)
        return (double) tauEstimate;

    const auto s0 = nsdfBuffer[(size_t) (tauEstimate - 1)];
    const auto s1 = nsdfBuffer[(size_t) tauEstimate];
    const auto s2 = nsdfBuffer[(size_t) (tauEstimate + 1)];

    const auto denom = 2.0 * (2.0 * s1 - s2 - s0);

    if (std::abs (denom) < 1.0e-9)
        return (double) tauEstimate;

    const auto shift = (s2 - s0) / denom;
    return tauEstimate + shift;
}

PitchResult MpmPitchDetector::detectPitch (const float* samples, int numSamples)
{
    if (samples == nullptr || numSamples < settings.windowSize || tauMax <= tauMin)
        return {};

    computeNsdf (samples);

    peakBuffer.clear();

    const auto halfWindow = (int) nsdfBuffer.size();
    int tau = 1;

    // Skip past the initial descent from the trivial tau=0 peak (NSDF(0)
    // is always 1) up to the first negative-going zero crossing, per the
    // MPM paper -- peaks before that point aren't meaningful periodicity
    // candidates.
    while (tau < halfWindow - 1 && nsdfBuffer[(size_t) tau] > 0.0f)
        ++tau;

    for (; tau < halfWindow - 1; ++tau)
    {
        const auto isLocalMaximum = nsdfBuffer[(size_t) tau] > nsdfBuffer[(size_t) (tau - 1)]
                                     && nsdfBuffer[(size_t) tau] >= nsdfBuffer[(size_t) (tau + 1)];

        if (isLocalMaximum && tau >= tauMin && tau <= tauMax)
            peakBuffer.push_back ({ tau, nsdfBuffer[(size_t) tau] });
    }

    if (peakBuffer.empty())
        return {};

    float maxPeakValue = 0.0f;
    for (const auto& peak : peakBuffer)
        maxPeakValue = std::max (maxPeakValue, peak.value);

    if (maxPeakValue <= 0.0f)
        return {};

    // Pick the smallest-lag peak close enough to the global peak -- this
    // (not the raw computation) is what actually reduces octave errors
    // compared to just taking the single highest peak.
    int chosenTau = -1;
    float chosenValue = 0.0f;

    for (const auto& peak : peakBuffer)
    {
        if (peak.value >= settings.mpmClarityThreshold * maxPeakValue)
        {
            chosenTau = peak.tau;
            chosenValue = peak.value;
            break;
        }
    }

    if (chosenTau < 0)
        return {};

    const auto refinedTau = parabolicInterpolation (chosenTau);

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
    result.confidence = std::max (0.0f, std::min (1.0f, chosenValue)); // "clarity" in MPM terms
    return result;
}
