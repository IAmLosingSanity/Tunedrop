#pragma once

#include "StringTarget.h"
#include <juce_core/juce_core.h>
#include <vector>

// A named set of per-string target pitches. Strings are ordered
// lowest-pitched to highest-pitched (index 0 = lowest).
class Tuning
{
public:
    Tuning() = default;
    Tuning (juce::String idToUse, juce::String displayNameToUse, std::vector<StringTarget> stringsToUse);

    const juce::String& getId() const noexcept { return id; }
    const juce::String& getDisplayName() const noexcept { return displayName; }
    int getNumberOfStrings() const noexcept { return (int) strings.size(); }
    const std::vector<StringTarget>& getStrings() const noexcept { return strings; }

    // Returns nullptr if no string with that index exists.
    const StringTarget* getString (int stringIndex) const;

    // Structural validation only: non-empty, no duplicate string indices,
    // MIDI notes within 0-127. Doesn't judge musical sensibility (e.g. an
    // absurd interval) -- that's a UI/UX concern for the custom tuning
    // editor (Milestone 5), not a data-integrity one.
    bool isValid() const;

private:
    // Stable identifier, e.g. "drop_c_sharp" -- never shown to the user,
    // never renamed once released (PluginState will persist selected
    // tunings by id, not by display name or array index).
    juce::String id;
    juce::String displayName; // e.g. "Drop C#"
    std::vector<StringTarget> strings;
};
