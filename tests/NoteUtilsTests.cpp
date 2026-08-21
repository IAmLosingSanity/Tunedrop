#include "dsp/NoteUtils.h"
#include <juce_core/juce_core.h>

class NoteUtilsTests : public juce::UnitTest
{
public:
    NoteUtilsTests() : juce::UnitTest ("NoteUtils") {}

    void runTest() override
    {
        beginTest ("midiToFrequency - reference points");
        {
            expectWithinAbsoluteError (NoteUtils::midiToFrequency (69), 440.0, 0.001);
            expectWithinAbsoluteError (NoteUtils::midiToFrequency (57), 220.0, 0.001);
            expectWithinAbsoluteError (NoteUtils::midiToFrequency (81), 880.0, 0.001);
            // Low E on a standard-tuned guitar.
            expectWithinAbsoluteError (NoteUtils::midiToFrequency (40), 82.4069, 0.01);
        }

        beginTest ("frequencyToNearestMidi - exact and near matches");
        {
            expectEquals (NoteUtils::frequencyToNearestMidi (440.0), 69);
            expectEquals (NoteUtils::frequencyToNearestMidi (445.0), 69); // still nearer to A4 than A#4
            expectEquals (NoteUtils::frequencyToNearestMidi (82.4069), 40);
        }

        beginTest ("frequencyToNearestMidi - invalid input falls back safely");
        {
            expectEquals (NoteUtils::frequencyToNearestMidi (0.0), NoteUtils::a4MidiNote);
            expectEquals (NoteUtils::frequencyToNearestMidi (-10.0), NoteUtils::a4MidiNote);
        }

        beginTest ("centsOffset - in tune and detuned");
        {
            expectWithinAbsoluteError (NoteUtils::centsOffset (440.0, 440.0), 0.0, 0.001);
            expectWithinAbsoluteError (NoteUtils::centsOffset (440.0, 220.0), 1200.0, 0.001);
            expect (NoteUtils::centsOffset (438.0, 440.0) < 0.0); // flat
            expect (NoteUtils::centsOffset (442.0, 440.0) > 0.0); // sharp
        }

        beginTest ("centsOffset - invalid input never produces NaN/inf");
        {
            expectEquals (NoteUtils::centsOffset (0.0, 440.0), 0.0);
            expectEquals (NoteUtils::centsOffset (440.0, 0.0), 0.0);
            expectEquals (NoteUtils::centsOffset (-5.0, 440.0), 0.0);
        }

        beginTest ("midiToNoteName - naming");
        {
            expectEquals (NoteUtils::midiToNoteName (69), juce::String ("A4"));
            expectEquals (NoteUtils::midiToNoteName (60), juce::String ("C4")); // middle C
            expectEquals (NoteUtils::midiToNoteName (61), juce::String ("C#4"));
            expectEquals (NoteUtils::midiToNoteName (40), juce::String ("E2")); // low E
        }
    }
};

static NoteUtilsTests noteUtilsTests;
