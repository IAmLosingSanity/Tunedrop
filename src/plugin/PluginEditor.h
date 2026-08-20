#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class TunedropAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit TunedropAudioProcessorEditor (TunedropAudioProcessor&);
    ~TunedropAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TunedropAudioProcessor& processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TunedropAudioProcessorEditor)
};
