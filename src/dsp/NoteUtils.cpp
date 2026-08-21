#include "NoteUtils.h"
#include <cmath>

namespace NoteUtils
{
    double midiToFrequency (int midiNote) noexcept
    {
        return a4Frequency * std::pow (2.0, (midiNote - a4MidiNote) / 12.0);
    }

    int frequencyToNearestMidi (double frequencyHz) noexcept
    {
        if (frequencyHz <= 0.0 || ! std::isfinite (frequencyHz))
            return a4MidiNote;

        const auto midi = a4MidiNote + 12.0 * std::log2 (frequencyHz / a4Frequency);
        return (int) std::lround (midi);
    }

    double centsOffset (double frequencyHz, double targetFrequencyHz) noexcept
    {
        if (frequencyHz <= 0.0 || targetFrequencyHz <= 0.0
            || ! std::isfinite (frequencyHz) || ! std::isfinite (targetFrequencyHz))
            return 0.0;

        return 1200.0 * std::log2 (frequencyHz / targetFrequencyHz);
    }

    juce::String midiToNoteName (int midiNote)
    {
        static const char* names[] = { "C", "C#", "D", "D#", "E", "F",
                                        "F#", "G", "G#", "A", "A#", "B" };

        // Floored mod/div so this behaves correctly for negative MIDI
        // numbers too (shouldn't happen in practice, but no reason to UB).
        const auto pitchClass = ((midiNote % 12) + 12) % 12;
        const auto octave = (int) std::floor (midiNote / 12.0) - 1;

        return juce::String (names[(size_t) pitchClass]) + juce::String (octave);
    }
}
