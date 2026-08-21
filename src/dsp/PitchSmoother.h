#pragma once

#include "PitchResult.h"

// Kept as a free-standing struct rather than nested inside PitchSmoother:
// nesting it and using `= {}` as a constructor default argument for the
// nested type triggers a GCC parsing quirk ("could not convert
// '<brace-enclosed initializer list>()'... ") -- reproduced on GCC 13.
// Top-level type + non-nested default argument sidesteps it entirely and
// matches the pattern already used for PitchDetectorSettings.
struct PitchSmootherSettings
{
    // 0..1. Higher = snappier response when the incoming pitch is
    // clearly different from the current reading (new string, bend).
    float attackCoefficient = 0.6f;

    // 0..1. Lower = holds the current reading more steadily against
    // small frame-to-frame jitter around the same pitch.
    float releaseCoefficient = 0.15f;

    // Consecutive invalid frames tolerated before reporting "no
    // pitch" -- absorbs brief dropouts without a full reset.
    int maxMissedFramesBeforeInvalid = 6;
};

// Smooths a stream of per-window PitchResults into a stable readout for the
// UI: fast attack toward a genuinely new pitch, slower release, and a
// short hold through brief dropouts so a single noisy/unvoiced window
// doesn't make the meter flicker to "no pitch" (see PROJECT.md section 18).
//
// This does not replace PitchDetector's own confidence threshold -- it
// operates purely on the stream of already-accepted results.
class PitchSmoother
{
public:
    explicit PitchSmoother (PitchSmootherSettings settingsToUse = {});

    void reset();
    PitchResult process (const PitchResult& newResult);

private:
    PitchSmootherSettings settings;

    bool hasValue = false;
    double smoothedFrequencyHz = 0.0;
    float smoothedConfidence = 0.0f;
    int missedFrameCount = 0;
};
