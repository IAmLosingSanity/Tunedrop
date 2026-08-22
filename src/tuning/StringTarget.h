#pragma once

#include "dsp/NoteUtils.h"

// One string's target pitch within a Tuning. stringIndex 0 = lowest-pitched
// string, increasing toward the highest-pitched string (see Tuning.h) --
// this is a data-model convention, independent of how a UI labels strings
// (many guitarists number strings 6->1, low to high; that's a display
// concern handled wherever the UI renders string labels, not here).
struct StringTarget
{
    int stringIndex = 0;
    int midiNote = 0;

    // Computed rather than stored, so it can never drift out of sync with
    // midiNote (see PROJECT.md section 16 -- sharps only for v1).
    juce::String getDisplayName() const { return NoteUtils::midiToNoteName (midiNote); }
};
