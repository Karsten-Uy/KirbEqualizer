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

//==============================================================================

struct LowCutControls : juce::Component
{
    void paint(juce::Graphics& g) override;
};

struct PeakControls : juce::Component
{
    void paint(juce::Graphics& g) override;
};

struct HighCutControls : juce::Component
{
    void paint(juce::Graphics& g) override;
};

struct GainControls : juce::Component
{
    void paint(juce::Graphics& g) override;
};

struct Analyzer : juce::Component
{
    void paint(juce::Graphics& g) override;
};

struct TitleBlock : juce::Component
{
    void paint(juce::Graphics& g) override;
};

struct BigDialLAF : juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider&) override;
};

struct SmallDialLAF : juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider&) override;

};

//==============================================================================

/**
*/
class SimpleEQAudioProcessorEditor : public juce::AudioProcessorEditor,
    juce::Slider::Listener, juce::AudioProcessorParameter::Listener, juce::Timer
{
public:
    SimpleEQAudioProcessorEditor(SimpleEQAudioProcessor&);
    ~SimpleEQAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider);

    void parameterValueChanged(int parameterIndex, float newValue) override;

    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    virtual void timerCallback() override;

private:

    juce::Image background;

    juce::Atomic<bool> parameterChanged{ false };

    juce::Slider lowFreqDial{ "lowFreqDial" };
    juce::Label lowFreqLabel{ "Low Cut Frequency" };

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

    juce::Slider highSlopeSelect{ "highSlopeSelect" };

    juce::Label titleLabel;



    SimpleEQAudioProcessor& audioProcessor;

    using APVTS = juce::AudioProcessorValueTreeState;
    using sliderAttachment = APVTS::SliderAttachment;
    using comboBoxAttachment = APVTS::ComboBoxAttachment;

    // For Response Curve
    MonoChain monoChain;


    sliderAttachment // connects it to the parameter in the process block
        lowFreqSliderAttachment,
        peakFreqSilderAttachment,
        highFreqSliderAttachment,
        peakQSliderAttachemnt,
        peakGainSliderAttachment,
        outGainSliderAttachment,
        lowSlopeSliderAttachment,
        highSlopeSliderAttachment;

    LowCutControls lowControl;

    PeakControls peakControl;

    HighCutControls highControl;

    GainControls gainControl;

    Analyzer analyzer;

    Placeholder gridLines;

    TitleBlock titleBlock;


    BigDialLAF bigDialLAF;

    SmallDialLAF smallDialLAF;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleEQAudioProcessorEditor)
};
