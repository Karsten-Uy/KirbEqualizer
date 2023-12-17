/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleEQAudioProcessorEditor::SimpleEQAudioProcessorEditor (SimpleEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), lowFreqSliderAttachment(audioProcessor.apvts, "LowCut Freq", lowFreqDial)
{
    // Initialize the slider before creating the attachment
    lowFreqDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lowFreqDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lowFreqDial.setRange(20, 20000);
    lowFreqDial.setTextValueSuffix(" Hz");
    lowFreqDial.addListener(this);

    lowFreqLabel.setText("Low Cut Frequency", juce::NotificationType::dontSendNotification);
    lowFreqLabel.attachToComponent(&lowFreqDial, false);

    addAndMakeVisible(lowFreqDial);

    setSize(600, 400);
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

    juce::Rectangle<int> lowFreqDialArea = area.removeFromBottom(area.getHeight() / 2);

    lowFreqDial.setBounds(lowFreqDialArea);
}

void SimpleEQAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    
}

