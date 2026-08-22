#pragma once

#include "Tuning.h"
#include <vector>

// Read-only registry of factory tunings for Milestone 1. User-defined
// tuning storage/management arrives in Milestone 5 alongside PluginState
// persistence -- deliberately not stubbed out here with a half-shaped API
// that would need reworking once that lands.
class TuningDatabase
{
public:
    TuningDatabase();

    // Returns nullptr if no tuning with that id exists.
    const Tuning* findById (const juce::String& tuningId) const;

    // Returns nullptr if no tuning with that exact display name exists.
    const Tuning* findByDisplayName (const juce::String& tuningDisplayName) const;

    const std::vector<Tuning>& getFactoryTunings() const noexcept { return factoryTunings; }

private:
    std::vector<Tuning> factoryTunings;
};
