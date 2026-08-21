#include "dsp/YinPitchDetector.h"
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

    PitchDetectorSettings makeTestSettings (double sampleRate)
    {
        PitchDetectorSettings settings;
        settings.sampleRate = sampleRate;
        settings.windowSize = 4096; // wide enough to cover 55 Hz with margin
        settings.minFrequencyHz = 40.0;
        settings.maxFrequencyHz = 1000.0;
        return settings;
    }
}

class YinPitchDetectorTests : public juce::UnitTest
{
public:
    YinPitchDetectorTests() : juce::UnitTest ("YinPitchDetector") {}

    void runTest() override
    {
        constexpr double sampleRate = 44100.0;
        const auto settings = makeTestSettings (sampleRate);

        YinPitchDetector detector;
        detector.prepare (settings);

        // Reference points from PROJECT.md section 21: low guitar range
        // through an octave above open high E.
        const double testFrequencies[] = { 55.0, 82.4069, 110.0, 146.832, 220.0, 440.0, 880.0 };

        for (const auto freq : testFrequencies)
        {
            beginTest ("Detects clean sine at " + juce::String (freq, 2) + " Hz");

            const auto samples = generateSine (freq, sampleRate, settings.windowSize);
            const auto result = detector.detectPitch (samples.data(), (int) samples.size());

            expect (result.valid);

            if (result.valid)
            {
                expectWithinAbsoluteError (result.frequencyHz, freq, freq * 0.01);
                expect (result.confidence > 0.5f);
            }
        }

        beginTest ("Slightly detuned sine (110 Hz + 15 cents) still lands near true frequency");
        {
            const auto detunedFreq = 110.0 * std::pow (2.0, 15.0 / 1200.0);
            const auto samples = generateSine (detunedFreq, sampleRate, settings.windowSize);
            const auto result = detector.detectPitch (samples.data(), (int) samples.size());

            expect (result.valid);
            expectWithinAbsoluteError (result.frequencyHz, detunedFreq, detunedFreq * 0.01);
        }

        beginTest ("Silence yields an invalid result, not a guessed pitch");
        {
            const std::vector<float> silence ((size_t) settings.windowSize, 0.0f);
            const auto result = detector.detectPitch (silence.data(), (int) silence.size());

            expect (! result.valid);
        }

        beginTest ("Too-short input yields an invalid result rather than reading out of bounds");
        {
            const std::vector<float> shortBuffer ((size_t) settings.windowSize / 2, 0.5f);
            const auto result = detector.detectPitch (shortBuffer.data(), (int) shortBuffer.size());

            expect (! result.valid);
        }

        beginTest ("Null input is handled safely");
        {
            const auto result = detector.detectPitch (nullptr, settings.windowSize);
            expect (! result.valid);
        }
    }
};

static YinPitchDetectorTests yinPitchDetectorTests;
