/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================

/**
*/
class SimpleEQAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                      public juce::Slider::Listener
{
public:
    SimpleEQAudioProcessorEditor (SimpleEQAudioProcessor&);
    ~SimpleEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider);
 
private:


    juce::Slider lowFreqDial { "lowFreqDial" };
    juce::Label lowFreqLabel {"Low Cut Frequency"};

    juce::Slider peakFreqDial{ "peakFreqDial" };
    juce::Label peakFreqLabel{ "Peak Frequency" };

    juce::Slider highFreqDial{ "highFreqDial" };
    juce::Label highFreqLabel{ "High Cut Frequency" };

    juce::Slider peakQDial{ "peakQDial" };
    juce::Label peakQLabel{ "Peak Q Dial" };

    juce::Slider peakGainDial{ "peakGainDial" };
    juce::Label peakGainLabel{ "Peak Gain" };

    juce::Slider outGainSlider{ "outGainSlider" };
    juce::Label outGainLabel{ "Output Gain" };

    SimpleEQAudioProcessor& audioProcessor;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment lowFreqSliderAttachment; // connects it to the parameter in the process block

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleEQAudioProcessorEditor)
};
