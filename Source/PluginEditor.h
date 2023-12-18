/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================

struct Placeholder : juce::Component
{
    Placeholder();

    juce::Colour customColour;

    void paint(juce::Graphics& g) override
    {
        g.fillAll(customColour);
    }

};

struct LowCutControls : juce::Component
{
    void paint(juce::Graphics& g) override;
};

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider&) override {};

};

struct RotarySliderWithLabels : juce::Slider
{
    RotarySliderWithLabels(
        juce::RangedAudioParameter& rap, 
        const juce::String& unitSuffix) :
            juce::Slider(
                juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                juce::Slider::TextEntryBoxPosition::NoTextBox),
        param(&rap),
        suffix(unitSuffix)
    {
        setLookAndFeel(&lnf);
    }

    ~RotarySliderWithLabels() 
    {
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g) override;
    juce::Rectangle<int> getSliderBounds() const;
    int getTextHeight() const { return 14; };
    juce::String getDisplayString() const;


private:
    LookAndFeel lnf;

    juce::RangedAudioParameter* param;
    juce::String suffix;
};

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



    juce::Slider lowSlopeSelect{ "lowSlopeSelect" };
    juce::Label lowSlopeLabel{ "Low Cut Slope" };

    juce::Slider highSlopeSelect{ "highSlopeSelect" };
    juce::Label highSlopeLabel{ "High Cut Slope" };



    SimpleEQAudioProcessor& audioProcessor;

    using APVTS = juce::AudioProcessorValueTreeState;
    using sliderAttachment = APVTS::SliderAttachment;
    using comboBoxAttachment = APVTS::ComboBoxAttachment;

    sliderAttachment // connects it to the parameter in the process block
        lowFreqSliderAttachment,
        peakFreqSilderAttachment,
        highFreqSliderAttachment,
        peakQSliderAttachemnt,
        peakGainSliderAttachment,
        outGainSliderAttachment,
        lowSlopeSliderAttachment,
        highSlopeSliderAttachment;

    Placeholder titleStrip, analyzer, gainControl, /* lowControl, */ peakControl, highControl;

    LowCutControls lowControl;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleEQAudioProcessorEditor)
};
