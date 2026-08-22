#include "TuningDatabase.h"
#include "FactoryTunings.h"
#include <algorithm>

TuningDatabase::TuningDatabase() : factoryTunings (FactoryTunings::createAll())
{
}

const Tuning* TuningDatabase::findById (const juce::String& tuningId) const
{
    const auto it = std::find_if (factoryTunings.begin(), factoryTunings.end(),
                                   [&tuningId] (const Tuning& t) { return t.getId() == tuningId; });

    return it != factoryTunings.end() ? &(*it) : nullptr;
}

const Tuning* TuningDatabase::findByDisplayName (const juce::String& tuningDisplayName) const
{
    const auto it = std::find_if (factoryTunings.begin(), factoryTunings.end(),
                                   [&tuningDisplayName] (const Tuning& t)
                                   {
                                       return t.getDisplayName() == tuningDisplayName;
                                   });

    return it != factoryTunings.end() ? &(*it) : nullptr;
}
