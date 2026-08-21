#pragma once

#include "PitchDetectorAlgorithm.h"
#include <memory>

// Facade owning a swappable pitch-detection algorithm (YIN or MPM) so
// callers (PluginProcessor, standalone app, UI) talk to one stable type
// regardless of which algorithm is selected. Intended to back an "advanced
// settings" algorithm choice in the UI (PROJECT.md UI section) -- most
// users will never touch this, it defaults to YIN.
//
// setSettings()/setAlgorithm() allocate (they resize internal buffers) and
// must only be called from a non-audio thread -- prepareToPlay(), or in
// response to a UI change that's then applied via a message-thread ->
// audio-thread handoff, never from inside processBlock() directly.
class PitchDetector
{
public:
    enum class Algorithm
    {
        yin,
        mpm
    };

    explicit PitchDetector (PitchDetectorSettings settingsToUse = {},
                             Algorithm algorithmToUse = Algorithm::yin);

    void setSettings (const PitchDetectorSettings& newSettings);
    const PitchDetectorSettings& getSettings() const noexcept { return settings; }

    void setAlgorithm (Algorithm newAlgorithm);
    Algorithm getAlgorithm() const noexcept { return algorithm; }

    PitchResult detectPitch (const float* samples, int numSamples);

private:
    PitchDetectorSettings settings;
    Algorithm algorithm;
    std::unique_ptr<PitchDetectorAlgorithm> impl;

    static std::unique_ptr<PitchDetectorAlgorithm> createAlgorithm (Algorithm algorithmToCreate);
};
