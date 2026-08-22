#include "tuning/TuningDatabase.h"
#include <juce_core/juce_core.h>

class TuningDatabaseTests : public juce::UnitTest
{
public:
    TuningDatabaseTests() : juce::UnitTest ("TuningDatabase") {}

    void runTest() override
    {
        const TuningDatabase db;

        beginTest ("Contains exactly the 12 required factory tunings");
        {
            expectEquals ((int) db.getFactoryTunings().size(), 12);
        }

        beginTest ("All factory tunings are structurally valid");
        {
            for (const auto& tuning : db.getFactoryTunings())
                expect (tuning.isValid());
        }

        beginTest ("All factory tuning ids are unique");
        {
            juce::StringArray ids;
            for (const auto& tuning : db.getFactoryTunings())
                ids.add (tuning.getId());

            auto sorted = ids;
            sorted.sort (false);
            sorted.removeDuplicates (false);

            expectEquals (sorted.size(), ids.size());
        }

        beginTest ("findById finds Standard tuning with correct pitches");
        {
            const auto* standard = db.findById ("standard");
            expect (standard != nullptr);

            if (standard != nullptr)
            {
                expectEquals (standard->getNumberOfStrings(), 6);
                expectEquals (standard->getString (0)->midiNote, 40); // low E
                expectEquals (standard->getString (5)->midiNote, 64); // high E
            }
        }

        beginTest ("findById finds Drop D tuning: only the low string moves from Standard");
        {
            const auto* dropD = db.findById ("drop_d");
            expect (dropD != nullptr);

            if (dropD != nullptr)
            {
                expectEquals (dropD->getString (0)->midiNote, 38); // low string dropped to D
                expectEquals (dropD->getString (1)->midiNote, 45); // rest unchanged from Standard
                expectEquals (dropD->getString (5)->midiNote, 64);
            }
        }

        beginTest ("findById finds Drop C# tuning with correct low string");
        {
            const auto* dropCSharp = db.findById ("drop_c_sharp");
            expect (dropCSharp != nullptr);

            if (dropCSharp != nullptr)
                expectEquals (dropCSharp->getString (0)->midiNote, 37);
        }

        beginTest ("findById finds Drop B tuning with correct pitches");
        {
            const auto* dropB = db.findById ("drop_b");
            expect (dropB != nullptr);

            if (dropB != nullptr)
            {
                expectEquals (dropB->getString (0)->midiNote, 35); // B1
                expectEquals (dropB->getString (5)->midiNote, 61); // C#4
            }
        }

        beginTest ("findById returns nullptr for an unknown id");
        {
            expect (db.findById ("not_a_real_tuning") == nullptr);
        }

        beginTest ("findByDisplayName finds DADGAD by exact display name");
        {
            const auto* dadgad = db.findByDisplayName ("DADGAD");
            expect (dadgad != nullptr);

            if (dadgad != nullptr)
                expectEquals (dadgad->getId(), juce::String ("dadgad"));
        }

        beginTest ("findByDisplayName returns nullptr for an unknown name");
        {
            expect (db.findByDisplayName ("Not A Real Tuning") == nullptr);
        }
    }
};

static TuningDatabaseTests tuningDatabaseTests;
