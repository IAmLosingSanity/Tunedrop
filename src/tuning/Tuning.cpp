#include "Tuning.h"
#include <algorithm>

Tuning::Tuning (juce::String idToUse, juce::String displayNameToUse, std::vector<StringTarget> stringsToUse)
    : id (std::move (idToUse)),
      displayName (std::move (displayNameToUse)),
      strings (std::move (stringsToUse))
{
}

const StringTarget* Tuning::getString (int stringIndex) const
{
    const auto it = std::find_if (strings.begin(), strings.end(),
                                   [stringIndex] (const StringTarget& s)
                                   {
                                       return s.stringIndex == stringIndex;
                                   });

    return it != strings.end() ? &(*it) : nullptr;
}

bool Tuning::isValid() const
{
    if (id.isEmpty() || strings.empty())
        return false;

    std::vector<int> seenIndices;
    seenIndices.reserve (strings.size());

    for (const auto& s : strings)
    {
        if (s.midiNote < 0 || s.midiNote > 127)
            return false;

        if (std::find (seenIndices.begin(), seenIndices.end(), s.stringIndex) != seenIndices.end())
            return false; // duplicate string index

        seenIndices.push_back (s.stringIndex);
    }

    return true;
}
