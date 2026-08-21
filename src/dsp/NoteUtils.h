#pragma once

#include <juce_core/juce_core.h>

// Pure, stateless note/pitch math shared by the pitch detector, the tuning
// database, and the UI. No JUCE audio/DSP dependency on purpose -- this
// should stay trivially unit-testable outside the plugin.
namespace NoteUtils
{
    constexpr double a4Frequency = 440.0;
    constexpr int a4MidiNote = 69;

    // MIDI note number -> frequency in Hz. MIDI 69 == A4 == 440 Hz.
    double midiToFrequency (int midiNote) noexcept;

    // Frequency in Hz -> nearest MIDI note number.
    // Returns a4MidiNote as a safe fallback for invalid input (<= 0, NaN,
    // inf) -- callers doing pitch detection should already be filtering
    // invalid frequencies before this point (see PitchResult.valid) and
    // should not rely on this fallback to mean "in tune".
    int frequencyToNearestMidi (double frequencyHz) noexcept;

    // Deviation of frequencyHz from targetFrequencyHz, in cents.
    // Positive = sharp (frequencyHz above target), negative = flat.
    // Returns 0.0 for invalid input rather than propagating NaN/inf.
    double centsOffset (double frequencyHz, double targetFrequencyHz) noexcept;

    // MIDI note number -> note name in scientific pitch notation.
    // Sharps only for v1 (see PROJECT.md section 16) -- e.g. 69 -> "A4",
    // 61 -> "C#4". MIDI 60 == C4 (middle C).
    juce::String midiToNoteName (int midiNote);
}
