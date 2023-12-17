/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

Placeholder::Placeholder() 
{
    juce::Random r;
    customColour = juce::Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));
}


//==============================================================================
SimpleEQAudioProcessorEditor::SimpleEQAudioProcessorEditor (SimpleEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),

    lowFreqSliderAttachment(audioProcessor.apvts, "LowCut Freq", lowFreqDial),
    peakFreqSilderAttachment(audioProcessor.apvts, "Peak Freq", peakFreqDial),
    highFreqSliderAttachment(audioProcessor.apvts, "HighCut Freq", highFreqDial),
    peakQSliderAttachemnt(audioProcessor.apvts, "Peak Quality", peakQDial),
    peakGainSliderAttachment(audioProcessor.apvts, "Peak Gain", peakGainDial),
    outGainSliderAttachment(audioProcessor.apvts, "Output Gain", outGainSlider),

    lowSlopeSliderAttachment(audioProcessor.apvts,"LowCut Slope" ,lowSlopeSelect),
    highSlopeSliderAttachment(audioProcessor.apvts, "HighCut Slope", highSlopeSelect)
{
    setSize(400, 420);

    // Blocks

    addAndMakeVisible(titleStrip);
    addAndMakeVisible(analyzer);
    addAndMakeVisible(gainControl);
    addAndMakeVisible(lowControl);
    addAndMakeVisible(peakControl);
    addAndMakeVisible(highControl);

    // LowCut

    //addAndMakeVisible(lowFreqDial);
    lowFreqDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lowFreqDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lowFreqDial.setRange(20, 20000);
    lowFreqDial.setTextValueSuffix(" Hz");
    lowFreqDial.addListener(this);
    lowFreqLabel.setText("Low Cut Frequency", juce::NotificationType::dontSendNotification);
    lowFreqLabel.attachToComponent(&lowFreqDial, false);

    //addAndMakeVisible(lowSlopeSelect);
    lowSlopeSelect.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    lowSlopeSelect.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lowSlopeSelect.setRange(0, 3);
    lowSlopeSelect.setTextValueSuffix(" dB");
    lowSlopeSelect.addListener(this);
    lowFreqLabel.setText("Low Cut Select", juce::NotificationType::dontSendNotification);
    lowFreqLabel.attachToComponent(&lowSlopeSelect, false);
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

    auto bounds = getLocalBounds();

    gainControl.setBounds(bounds.removeFromRight(70));

    titleStrip.setBounds(bounds.removeFromTop(50));

    analyzer.setBounds(bounds.removeFromTop(200));

    lowControl.setBounds(bounds.removeFromLeft(110));

    peakControl.setBounds(bounds.removeFromLeft(110));

    highControl.setBounds(bounds.removeFromLeft(110));


    
}

void SimpleEQAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    
}

