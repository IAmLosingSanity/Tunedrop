#include "PluginEditor.h"

TunedropAudioProcessorEditor::TunedropAudioProcessorEditor (TunedropAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    setSize (400, 300);
}

void TunedropAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawFittedText ("Tunedrop -- Milestone 0",
                       getLocalBounds(),
                       juce::Justification::centred,
                       1);
}

void TunedropAudioProcessorEditor::resized()
{
    // Layout of real tuner UI components lands here from Milestone 3 on.
}
