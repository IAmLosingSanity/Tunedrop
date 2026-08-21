#pragma once

#include "PitchResult.h"
#include "PitchDetectorSettings.h"

// Common interface for a single pitch-detection algorithm implementation
// (see YinPitchDetector, MpmPitchDetector). Implementations must not
// allocate inside detectPitch() -- all buffers must be sized once in
// prepare(), which should only ever be called from a non-audio thread
// (e.g. prepareToPlay(), or in response to a UI settings change), never
// from inside processBlock().
class PitchDetectorAlgorithm
{
public:
    virtual ~PitchDetectorAlgorithm() = default;

    virtual void prepare (const PitchDetectorSettings& settings) = 0;
    virtual PitchResult detectPitch (const float* samples, int numSamples) = 0;
};
