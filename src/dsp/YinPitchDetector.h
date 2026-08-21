#pragma once

#include "PitchDetectorAlgorithm.h"
#include <vector>

// YIN (de Cheveigne & Kawahara, 2002). Not FFT-peak-picking on purpose:
// guitar signals often have a stronger harmonic than the fundamental,
// which a naive spectral-peak approach would latch onto. YIN's
// time-domain difference-function approach is far more robust to this
// for monophonic string instruments (see PROJECT.md section 10).
class YinPitchDetector : public PitchDetectorAlgorithm
{
public:
    void prepare (const PitchDetectorSettings& settings) override;
    PitchResult detectPitch (const float* samples, int numSamples) override;

private:
    PitchDetectorSettings settings;
    int tauMin = 2;
    int tauMax = 2;

    std::vector<float> differenceBuffer; // d(tau)
    std::vector<float> cmndBuffer;       // cumulative mean normalized difference, d'(tau)

    void computeDifferenceFunction (const float* samples);
    void computeCumulativeMeanNormalizedDifference();
    int findAbsoluteThresholdMinimum() const;
    double parabolicInterpolation (int tauEstimate) const;
};
