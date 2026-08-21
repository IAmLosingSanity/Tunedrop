#include "dsp/PitchDetector.h"
#include <juce_core/juce_core.h>
#include <cmath>
#include <vector>

namespace
{
    std::vector<float> generateSine (double frequencyHz, double sampleRate, int numSamples)
    {
        std::vector<float> samples ((size_t) numSamples);
        const auto phaseIncrement = 2.0 * juce::MathConstants<double>::pi * frequencyHz / sampleRate;

        double phase = 0.0;

        for (int i = 0; i < numSamples; ++i)
        {
            samples[(size_t) i] = (float) std::sin (phase);
            phase += phaseIncrement;
        }

        return samples;
    }
}

class PitchDetectorFacadeTests : public juce::UnitTest
{
public:
    PitchDetectorFacadeTests() : juce::UnitTest ("PitchDetector (facade)") {}

    void runTest() override
    {
        constexpr double sampleRate = 44100.0;

        PitchDetectorSettings settings;
        settings.sampleRate = sampleRate;
        settings.windowSize = 4096;
        settings.minFrequencyHz = 40.0;
        settings.maxFrequencyHz = 1000.0;

        beginTest ("Defaults to YIN");
        {
            PitchDetector detector (settings);
            expect (detector.getAlgorithm() == PitchDetector::Algorithm::yin);
        }

        beginTest ("Both algorithms detect the same clean sine correctly");
        {
            const auto samples = generateSine (220.0, sampleRate, settings.windowSize);

            PitchDetector detector (settings, PitchDetector::Algorithm::yin);
            const auto yinResult = detector.detectPitch (samples.data(), (int) samples.size());

            detector.setAlgorithm (PitchDetector::Algorithm::mpm);
            const auto mpmResult = detector.detectPitch (samples.data(), (int) samples.size());

            expect (yinResult.valid);
            expect (mpmResult.valid);
            expectWithinAbsoluteError (yinResult.frequencyHz, 220.0, 2.0);
            expectWithinAbsoluteError (mpmResult.frequencyHz, 220.0, 2.0);
        }

        beginTest ("setAlgorithm re-applies current settings to the new algorithm");
        {
            PitchDetector detector (settings, PitchDetector::Algorithm::yin);
            detector.setAlgorithm (PitchDetector::Algorithm::mpm);

            expect (detector.getAlgorithm() == PitchDetector::Algorithm::mpm);
            expectWithinAbsoluteError (detector.getSettings().minFrequencyHz,
                                        settings.minFrequencyHz, 0.001);
        }

        beginTest ("setSettings updates the currently active algorithm");
        {
            PitchDetector detector (settings, PitchDetector::Algorithm::mpm);

            auto updated = settings;
            updated.minFrequencyHz = 50.0;
            detector.setSettings (updated);

            expectWithinAbsoluteError (detector.getSettings().minFrequencyHz, 50.0, 0.001);
        }
    }
};

static PitchDetectorFacadeTests pitchDetectorFacadeTests;
