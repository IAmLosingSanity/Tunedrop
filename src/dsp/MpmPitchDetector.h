#pragma once

#include "PitchDetectorAlgorithm.h"
#include <vector>

// McLeod Pitch Method (McLeod & Wyvill, 2005), aka MPM. Uses the
// normalized square difference function (NSDF) and picks the
// smallest-lag peak that's within mpmClarityThreshold of the global peak,
// rather than YIN's "first value below threshold" rule -- this tends to
// reduce octave errors on harmonic-rich signals like a plucked guitar
// string. Computationally the same order as YIN for a given window size;
// the two are not "fast vs. slow", they trade off differently on noisy or
// harmonically complex input (see PROJECT.md section 10).
class MpmPitchDetector : public PitchDetectorAlgorithm
{
public:
    void prepare (const PitchDetectorSettings& settings) override;
    PitchResult detectPitch (const float* samples, int numSamples) override;

private:
    struct Peak
    {
        int tau;
        float value;
    };

    PitchDetectorSettings settings;
    int tauMin = 2;
    int tauMax = 2;

    std::vector<float> nsdfBuffer;
    std::vector<Peak> peakBuffer; // scratch, reused every call -- reserved in prepare()

    void computeNsdf (const float* samples);
    double parabolicInterpolation (int tauEstimate) const;
};
