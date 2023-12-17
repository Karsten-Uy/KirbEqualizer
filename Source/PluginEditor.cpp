/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleEQAudioProcessorEditor::SimpleEQAudioProcessorEditor (SimpleEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    lowFreqDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lowFreqDial.setTextBoxStyle(juce::Slider::TextBoxBelow,false,50,20);

    addAndMakeVisible(lowFreqDial);


    setSize (600, 600);
}

SimpleEQAudioProcessorEditor::~SimpleEQAudioProcessorEditor()
{
}

//==============================================================================
void SimpleEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    

}

void SimpleEQAudioProcessorEditor::resized()
{
    auto r = getLocalBounds();

    juce::Rectangle<int> area = getLocalBounds();

    juce::Rectangle<int> lowFreqDialArea = area.removeFromTop(area.getHeight() / 2);

    lowFreqDial.setBounds(lowFreqDialArea);
}


