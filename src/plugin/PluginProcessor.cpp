#include "PluginProcessor.h"
#include "PluginEditor.h"

TunedropAudioProcessor::TunedropAudioProcessor()
    : AudioProcessor (BusesProperties()
                        .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                        .withOutput ("Output", juce::AudioChannelSet::mono(), true))
{
}

void TunedropAudioProcessor::prepareToPlay (double, int)
{
    // Pitch detector allocation/reset lands here in Milestone 2.
}

void TunedropAudioProcessor::releaseResources()
{
}

bool TunedropAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono();
}

void TunedropAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    // Milestone 0: pass audio through unchanged (tuner never modifies the
    // signal anyway -- it only listens). Pitch detection wired in later.
    juce::ignoreUnused (buffer);
}

juce::AudioProcessorEditor* TunedropAudioProcessor::createEditor()
{
    return new TunedropAudioProcessorEditor (*this);
}

void TunedropAudioProcessor::getStateInformation (juce::MemoryBlock&)
{
    // State (selected tuning/string/custom tunings) wired in Milestone 4/5.
}

void TunedropAudioProcessor::setStateInformation (const void*, int)
{
}

// This creates the actual plugin instance. JUCE calls this internally --
// don't call it yourself and don't remove it.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TunedropAudioProcessor();
}
