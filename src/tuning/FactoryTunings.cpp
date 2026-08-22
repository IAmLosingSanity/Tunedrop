#include "FactoryTunings.h"

namespace
{
    Tuning makeTuning (juce::String id, juce::String displayName,
                        std::initializer_list<int> midiNotesLowToHigh)
    {
        std::vector<StringTarget> strings;
        strings.reserve (midiNotesLowToHigh.size());

        int index = 0;

        for (const auto midiNote : midiNotesLowToHigh)
        {
            strings.push_back ({ index, midiNote });
            ++index;
        }

        return Tuning (std::move (id), std::move (displayName), std::move (strings));
    }
}

std::vector<Tuning> FactoryTunings::createAll()
{
    return {
        // Standard EADGBE. E2 A2 D3 G3 B3 E4.
        makeTuning ("standard", "Standard", { 40, 45, 50, 55, 59, 64 }),

        // Half step down from Standard.
        makeTuning ("eb_standard", "Eb Standard", { 39, 44, 49, 54, 58, 63 }),

        // Whole step down from Standard.
        makeTuning ("d_standard", "D Standard", { 38, 43, 48, 53, 57, 62 }),

        // Standard with only the low string dropped a whole step: D A D G B E.
        makeTuning ("drop_d", "Drop D", { 38, 45, 50, 55, 59, 64 }),

        // Eb Standard with only the low string dropped a whole step: C# G# C# F# A# D#.
        makeTuning ("drop_c_sharp", "Drop C#", { 37, 44, 49, 54, 58, 63 }),

        // D Standard with only the low string dropped a whole step: C G C F A D.
        makeTuning ("drop_c", "Drop C", { 36, 43, 48, 53, 57, 62 }),

        // "C# Standard" shape (3 semitones down) with the low string
        // dropped a further whole step: B F# B E G# C#.
        makeTuning ("drop_b", "Drop B", { 35, 42, 47, 52, 56, 61 }),

        // D A D G A D.
        makeTuning ("dadgad", "DADGAD", { 38, 45, 50, 55, 57, 62 }),

        // Open D: D A D F# A D.
        makeTuning ("open_d", "Open D", { 38, 45, 50, 54, 57, 62 }),

        // Open E: E B E G# B E.
        makeTuning ("open_e", "Open E", { 40, 47, 52, 56, 59, 64 }),

        // Open G: D G D G B D.
        makeTuning ("open_g", "Open G", { 38, 43, 50, 55, 59, 62 }),

        // Open A: E A E A C# E.
        makeTuning ("open_a", "Open A", { 40, 45, 52, 57, 61, 64 })
    };
}
