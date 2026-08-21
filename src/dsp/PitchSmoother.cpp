#include "PitchSmoother.h"
#include <cmath>

PitchSmoother::PitchSmoother (PitchSmootherSettings settingsToUse) : settings (settingsToUse)
{
}

void PitchSmoother::reset()
{
    hasValue = false;
    smoothedFrequencyHz = 0.0;
    smoothedConfidence = 0.0f;
    missedFrameCount = 0;
}

PitchResult PitchSmoother::process (const PitchResult& newResult)
{
    if (! newResult.valid)
    {
        ++missedFrameCount;

        if (! hasValue || missedFrameCount >= settings.maxMissedFramesBeforeInvalid)
        {
            reset();
            return {};
        }

        // Briefly hold the last known-good reading through short dropouts
        // rather than flickering to "no pitch" on a single missed window.
        PitchResult held;
        held.valid = true;
        held.frequencyHz = smoothedFrequencyHz;
        held.confidence = smoothedConfidence;
        return held;
    }

    missedFrameCount = 0;

    if (! hasValue)
    {
        hasValue = true;
        smoothedFrequencyHz = newResult.frequencyHz;
        smoothedConfidence = newResult.confidence;
    }
    else
    {
        // Attack quickly toward a clearly different pitch (new string,
        // bend); release slowly to damp frame-to-frame jitter around a
        // held note. 1 Hz is well below any perceptible pitch change at
        // guitar frequencies, so it's a safe threshold for "same note".
        const auto coefficient = std::abs (newResult.frequencyHz - smoothedFrequencyHz) > 1.0
                                    ? settings.attackCoefficient
                                    : settings.releaseCoefficient;

        smoothedFrequencyHz += coefficient * (newResult.frequencyHz - smoothedFrequencyHz);
        smoothedConfidence += coefficient * (newResult.confidence - smoothedConfidence);
    }

    PitchResult result;
    result.valid = true;
    result.frequencyHz = smoothedFrequencyHz;
    result.confidence = smoothedConfidence;
    return result;
}
