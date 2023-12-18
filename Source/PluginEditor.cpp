/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

void LookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPosProportional,
    float rotaryStartAngle,
    float rotaryEndAngle,
    juce::Slider& slider)
{
    using namespace juce;

    auto bounds = Rectangle<float>(x, y, width, height);

    auto enabled = slider.isEnabled();

    g.setColour(enabled ? Colour(97u, 18u, 167u) : Colours::darkgrey);
    g.fillEllipse(bounds);

    g.setColour(enabled ? Colour(255u, 154u, 1u) : Colours::grey);
    g.drawEllipse(bounds, 1.f);

    if (auto* rswl = dynamic_cast<RotarySliderWithLabels*>(&slider))
    {
        auto center = bounds.getCentre();
        Path p;

        Rectangle<float> r;
        r.setLeft(center.getX() - 2);
        r.setRight(center.getX() + 2);
        r.setTop(bounds.getY());
        r.setBottom(center.getY() - rswl->getTextHeight() * 1.5);

        p.addRoundedRectangle(r, 2.f);

        jassert(rotaryStartAngle < rotaryEndAngle);

        auto sliderAngRad = jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);

        p.applyTransform(AffineTransform().rotated(sliderAngRad, center.getX(), center.getY()));

        g.fillPath(p);

        g.setFont(rswl->getTextHeight());
        auto text = rswl->getDisplayString();
        auto strWidth = g.getCurrentFont().getStringWidth(text);

        r.setSize(strWidth + 4, rswl->getTextHeight() + 2);
        r.setCentre(bounds.getCentre());

        g.setColour(enabled ? Colours::black : Colours::darkgrey);
        g.fillRect(r);

        g.setColour(enabled ? Colours::white : Colours::lightgrey);
        g.drawFittedText(text, r.toNearestInt(), juce::Justification::centred, 1);
    }
}

//==============================================================================
void RotarySliderWithLabels::paint(juce::Graphics& g)
{
    using namespace juce;

    auto startAng = degreesToRadians(180.f + 45.f);
    auto endAng = degreesToRadians(180.f - 45.f) + MathConstants<float>::twoPi;

    auto range = getRange();

    auto sliderBounds = getSliderBounds();

    //    g.setColour(Colours::red);
    //    g.drawRect(getLocalBounds());
    //    g.setColour(Colours::yellow);
    //    g.drawRect(sliderBounds);

    getLookAndFeel().drawRotarySlider(g,
        sliderBounds.getX(),
        sliderBounds.getY(),
        sliderBounds.getWidth(),
        sliderBounds.getHeight(),
        jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
        startAng,
        endAng,
        *this);

    auto center = sliderBounds.toFloat().getCentre();
    auto radius = sliderBounds.getWidth() * 0.5f;

    g.setColour(Colour(0u, 172u, 1u));
    g.setFont(getTextHeight());

    auto numChoices = labels.size();
    for (int i = 0; i < numChoices; ++i)
    {
        auto pos = labels[i].pos;
        jassert(0.f <= pos);
        jassert(pos <= 1.f);

        auto ang = jmap(pos, 0.f, 1.f, startAng, endAng);

        auto c = center.getPointOnCircumference(radius + getTextHeight() * 0.5f + 1, ang);

        Rectangle<float> r;
        auto str = labels[i].label;
        r.setSize(g.getCurrentFont().getStringWidth(str), getTextHeight());
        r.setCentre(c);
        r.setY(r.getY() + getTextHeight());

        g.drawFittedText(str, r.toNearestInt(), juce::Justification::centred, 1);
    }

}

juce::Rectangle<int> RotarySliderWithLabels::getSliderBounds() const
{
    auto bounds = getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());

    size -= getTextHeight() * 2;
    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), 0);
    r.setY(2);

    return r;

}

juce::String RotarySliderWithLabels::getDisplayString() const
{
    if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(param))
        return choiceParam->getCurrentChoiceName();

    juce::String str;
    bool addK = false;

    if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(param))
    {
        float val = getValue();

        if (val > 999.f)
        {
            val /= 1000.f; //1001 / 1000 = 1.001
            addK = true;
        }

        str = juce::String(val, (addK ? 2 : 0));
    }
    else
    {
        jassertfalse; //this shouldn't happen!
    }

    if (suffix.isNotEmpty())
    {
        str << " ";
        if (addK)
            str << "k";

        str << suffix;
    }

    return str;
}

//==============================================================================

Placeholder::Placeholder()
{
    juce::Random r;
    customColour = juce::Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));
}
//==============================================================================

void LowCutControls::paint(juce::Graphics& g)
{
    using namespace juce;
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::steelblue);
    g.fillAll();

    auto localBounds = getLocalBounds();

    bounds.reduce(3, 3);
    g.setColour(juce::Colours::slategrey);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
}

//==============================================================================

void PeakControls::paint(juce::Graphics& g)
{
    using namespace juce;
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::steelblue);
    g.fillAll();

    auto localBounds = getLocalBounds();

    bounds.reduce(3, 3);
    g.setColour(juce::Colours::slategrey);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
}

//==============================================================================

void HighCutControls::paint(juce::Graphics& g)
{
    using namespace juce;
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::steelblue);
    g.fillAll();

    auto localBounds = getLocalBounds();

    bounds.reduce(3, 3);
    g.setColour(juce::Colours::slategrey);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
}

//==============================================================================

void GainControls::paint(juce::Graphics& g)
{
    using namespace juce;
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::steelblue);
    g.fillAll();

    auto localBounds = getLocalBounds();

    bounds.reduce(3, 3);
    g.setColour(juce::Colours::slategrey);
    g.fillRoundedRectangle(bounds.toFloat(), 3);
}

//==============================================================================
SimpleEQAudioProcessorEditor::SimpleEQAudioProcessorEditor(SimpleEQAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),

    lowFreqSliderAttachment(audioProcessor.apvts, "LowCut Freq", lowFreqDial),
    peakFreqSilderAttachment(audioProcessor.apvts, "Peak Freq", peakFreqDial),
    highFreqSliderAttachment(audioProcessor.apvts, "HighCut Freq", highFreqDial),
    peakQSliderAttachemnt(audioProcessor.apvts, "Peak Quality", peakQDial),
    peakGainSliderAttachment(audioProcessor.apvts, "Peak Gain", peakGainDial),
    outGainSliderAttachment(audioProcessor.apvts, "Output Gain", outGainSlider),

    lowSlopeSliderAttachment(audioProcessor.apvts, "LowCut Slope", lowSlopeSelect),
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

    addAndMakeVisible(lowFreqDial);
    lowFreqDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lowFreqDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 20);
    lowFreqDial.setRange(20, 20000);
    lowFreqDial.setTextValueSuffix(" Hz");
    lowFreqDial.addListener(this);

    addAndMakeVisible(lowFreqLabel);
    lowFreqLabel.setText("Low Cut Controls",juce::NotificationType::dontSendNotification);
    lowFreqLabel.setSize(100,20);
    lowFreqLabel.setJustificationType(juce::Justification::bottom);
       
    addAndMakeVisible(lowSlopeSelect);
    lowSlopeSelect.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    lowSlopeSelect.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    lowSlopeSelect.setRange(0, 3);
    lowSlopeSelect.setTextValueSuffix(" dB");
    lowSlopeSelect.addListener(this);

    // High Cut

    addAndMakeVisible(highFreqDial);
    highFreqDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    highFreqDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 20);
    highFreqDial.setRange(20, 20000);
    highFreqDial.setTextValueSuffix(" Hz");
    highFreqDial.addListener(this);

    addAndMakeVisible(highFreqLabel);
    highFreqLabel.setText("High Cut Controls", juce::NotificationType::dontSendNotification);
    highFreqLabel.setSize(100, 20);
    highFreqLabel.setJustificationType(juce::Justification::bottom);

    addAndMakeVisible(highSlopeSelect);
    highSlopeSelect.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    highSlopeSelect.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    highSlopeSelect.setRange(0, 3);
    highSlopeSelect.setTextValueSuffix(" dB");
    highSlopeSelect.addListener(this);

    // Peak

    addAndMakeVisible(peakFreqDial);
    peakFreqDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    peakFreqDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 20);
    peakFreqDial.setRange(20, 20000);
    peakFreqDial.setTextValueSuffix(" Hz");
    peakFreqDial.addListener(this);

    addAndMakeVisible(peakQDial);
    peakQDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    peakQDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 10);
    peakQDial.setRange(0.1, 10);
    peakQDial.setTextValueSuffix("");
    peakQDial.addListener(this);

    addAndMakeVisible(peakGainDial);
    peakGainDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    peakGainDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 10);
    peakGainDial.setRange(-24, 24);
    peakGainDial.setTextValueSuffix(" Db");
    peakGainDial.addListener(this);

    addAndMakeVisible(peakFreqLabel);
    peakFreqLabel.setText("Peak Controls", juce::NotificationType::dontSendNotification);
    peakFreqLabel.setSize(100, 20);
    peakFreqLabel.setJustificationType(juce::Justification::bottom);


   
}

SimpleEQAudioProcessorEditor::~SimpleEQAudioProcessorEditor()
{
}

//==============================================================================
void SimpleEQAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);

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

    lowFreqLabel.setBounds(lowControl.getBounds().removeFromTop(100).removeFromRight(106));
    lowFreqDial.setBounds(lowControl.getBounds().removeFromTop(120).removeFromBottom(100));
    lowSlopeSelect.setBounds(lowControl.getBounds().removeFromBottom(40).removeFromTop(30).removeFromLeft(95).removeFromRight(80));

    highFreqLabel.setBounds(highControl.getBounds().removeFromTop(100).removeFromRight(105));
    highFreqDial.setBounds(highControl.getBounds().removeFromTop(120).removeFromBottom(100));
    highSlopeSelect.setBounds(highControl.getBounds().removeFromBottom(40).removeFromTop(30).removeFromLeft(95).removeFromRight(80));

    peakFreqLabel.setBounds(peakControl.getBounds().removeFromTop(100).removeFromRight(100));
    peakFreqDial.setBounds(peakControl.getBounds().removeFromTop(110).removeFromBottom(90));
    peakQDial.setBounds(peakControl.getBounds().removeFromBottom(70).removeFromTop(60).removeFromRight(55).removeFromLeft(50));
    peakGainDial.setBounds(peakControl.getBounds().removeFromBottom(70).removeFromTop(60).removeFromLeft(55).removeFromRight(50));

}

void SimpleEQAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{

}

