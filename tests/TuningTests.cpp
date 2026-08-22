#include "tuning/Tuning.h"
#include <juce_core/juce_core.h>

class TuningTests : public juce::UnitTest
{
public:
    TuningTests() : juce::UnitTest ("Tuning") {}

    void runTest() override
    {
        beginTest ("A tuning with no strings is invalid");
        {
            const Tuning empty ("empty", "Empty", {});
            expect (! empty.isValid());
        }

        beginTest ("A tuning with duplicate string indices is invalid");
        {
            const Tuning duplicated ("dup", "Dup", { { 0, 40 }, { 0, 45 } });
            expect (! duplicated.isValid());
        }

        beginTest ("A tuning with an out-of-range MIDI note is invalid");
        {
            const Tuning outOfRange ("bad", "Bad", { { 0, 200 } });
            expect (! outOfRange.isValid());
        }

        beginTest ("A well-formed tuning is valid and queryable by string index");
        {
            const Tuning valid ("valid", "Valid", { { 0, 40 }, { 1, 45 } });
            expect (valid.isValid());

            const auto* first = valid.getString (0);
            expect (first != nullptr);

            if (first != nullptr)
                expectEquals (first->midiNote, 40);

            expect (valid.getString (99) == nullptr);
        }

        beginTest ("StringTarget::getDisplayName matches NoteUtils naming");
        {
            const StringTarget target { 0, 40 };
            expectEquals (target.getDisplayName(), juce::String ("E2"));
        }
    }
};

static TuningTests tuningTests;
