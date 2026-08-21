#include "dsp/PitchSmoother.h"
#include <juce_core/juce_core.h>

class PitchSmootherTests : public juce::UnitTest
{
public:
    PitchSmootherTests() : juce::UnitTest ("PitchSmoother") {}

    void runTest() override
    {
        beginTest ("First valid result is passed through immediately");
        {
            PitchSmoother smoother;
            const auto output = smoother.process (PitchResult { true, 440.0, 0.9f });

            expect (output.valid);
            expectWithinAbsoluteError (output.frequencyHz, 440.0, 0.001);
        }

        beginTest ("Holds last reading through a brief dropout");
        {
            PitchSmoother smoother;
            smoother.process (PitchResult { true, 440.0, 0.9f });

            const auto held = smoother.process (PitchResult {}); // one invalid frame

            expect (held.valid);
            expectWithinAbsoluteError (held.frequencyHz, 440.0, 1.0);
        }

        beginTest ("Reports invalid after sustained dropout");
        {
            PitchSmootherSettings settings;
            settings.maxMissedFramesBeforeInvalid = 3;
            PitchSmoother smoother (settings);

            smoother.process (PitchResult { true, 440.0, 0.9f });

            PitchResult last;
            for (int i = 0; i < 5; ++i)
                last = smoother.process (PitchResult {});

            expect (! last.valid);
        }

        beginTest ("Converges toward a new sustained pitch rather than jumping instantly");
        {
            PitchSmoother smoother;
            smoother.process (PitchResult { true, 440.0, 0.9f });

            const auto afterOneFrame = smoother.process (PitchResult { true, 220.0, 0.9f });

            // Should move toward 220 but not land exactly on it in a single frame.
            expect (afterOneFrame.frequencyHz < 440.0);
            expect (afterOneFrame.frequencyHz > 220.0);
        }

        beginTest ("reset() clears state back to first-result behaviour");
        {
            PitchSmoother smoother;
            smoother.process (PitchResult { true, 440.0, 0.9f });
            smoother.reset();

            const auto output = smoother.process (PitchResult { true, 220.0, 0.9f });

            expectWithinAbsoluteError (output.frequencyHz, 220.0, 0.001);
        }
    }
};

static PitchSmootherTests pitchSmootherTests;
