/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

void BigDialLAF::drawRotarySlider(juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPosProportional,
    float rotaryStartAngle,
    float rotaryEndAngle,
    juce::Slider& slider)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 10.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // fill
    g.setColour(juce::Colours::steelblue);
    g.fillEllipse(rx, ry, rw, rw);

    // outline
    g.setColour(juce::Colours::lightsteelblue);
    g.drawEllipse(rx, ry, rw, rw, 2.0f);

    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // pointer
    g.setColour(juce::Colours::lightsteelblue);
    g.fillPath(p);

}

//==============================================================================

void SmallDialLAF::drawRotarySlider(juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPosProportional,
    float rotaryStartAngle,
    float rotaryEndAngle,
    juce::Slider& slider)
{

    auto radius = (float)juce::jmin(width / 2, height / 2) - 12.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // fill
    g.setColour(juce::Colours::steelblue);
    g.fillEllipse(rx, ry, rw, rw);

    // outline
    g.setColour(juce::Colours::lightsteelblue);
    g.drawEllipse(rx, ry, rw, rw, 2.0f);

    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // pointer
    g.setColour(juce::Colours::lightsteelblue);
    g.fillPath(p);

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

void Analyzer::paint(juce::Graphics& g)
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

void TitleBlock::paint(juce::Graphics& g)
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
    // Stuff for Response Curve
    const auto& params = audioProcessor.getParameters();

    for (auto param : params)
    {
        param->addListener(this);
    }

    startTimer(60);

    setSize(400, 420);

    // Blocks

    addAndMakeVisible(titleBlock);
    addAndMakeVisible(analyzer);
    addAndMakeVisible(gainControl);
    addAndMakeVisible(lowControl);
    addAndMakeVisible(peakControl);
    addAndMakeVisible(highControl);
    addAndMakeVisible(gridLines);

    // LowCut

    addAndMakeVisible(lowFreqDial);
    lowFreqDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lowFreqDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 20);
    lowFreqDial.setRange(20, 20000);
    lowFreqDial.setTextValueSuffix(" Hz");
    lowFreqDial.addListener(this);

    addAndMakeVisible(lowFreqLabel);
    lowFreqLabel.setText("Low Cut Controls", juce::NotificationType::dontSendNotification);
    lowFreqLabel.setSize(100, 20);
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
    peakQDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 10);
    peakQDial.setRange(0.1, 10);
    peakQDial.setTextValueSuffix("");
    peakQDial.addListener(this);

    addAndMakeVisible(peakGainDial);
    peakGainDial.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    peakGainDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 10);
    peakGainDial.setRange(-24, 24);
    peakGainDial.setTextValueSuffix(" dB");
    peakGainDial.addListener(this);

    addAndMakeVisible(peakFreqLabel);
    peakFreqLabel.setText("Peak Controls", juce::NotificationType::dontSendNotification);
    peakFreqLabel.setSize(100, 20);
    peakFreqLabel.setJustificationType(juce::Justification::bottom);

    // Gain Slider
    addAndMakeVisible(outGainSlider);
    outGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    outGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    outGainSlider.setRange(-144, 24);
    outGainSlider.setTextValueSuffix(" dB");
    outGainSlider.addListener(this);

    addAndMakeVisible(outGainLabel);
    outGainLabel.setText("  Out Gain", juce::NotificationType::dontSendNotification);
    outGainLabel.setSize(100, 20);
    outGainLabel.setJustificationType(juce::Justification::bottom);

    //For Title Block
    addAndMakeVisible(titleLabel);
    titleLabel.setText("KirbEqualizer", juce::NotificationType::dontSendNotification);
    titleLabel.setSize(310,40);
    titleLabel.setFont(juce::Font(35.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::horizontallyCentred);

    // Big Dial LAF

    lowFreqDial.setLookAndFeel(&bigDialLAF);
    highFreqDial.setLookAndFeel(&bigDialLAF);
    peakFreqDial.setLookAndFeel(&bigDialLAF);

    // Small Dial LAF
    peakQDial.setLookAndFeel(&smallDialLAF);
    peakGainDial.setLookAndFeel(&smallDialLAF);

}

SimpleEQAudioProcessorEditor::~SimpleEQAudioProcessorEditor()
{
    lowFreqDial.setLookAndFeel(nullptr);
    highFreqDial.setLookAndFeel(nullptr);
    peakFreqDial.setLookAndFeel(nullptr);
    peakQDial.setLookAndFeel(nullptr);
    peakGainDial.setLookAndFeel(nullptr);

    const auto& params = audioProcessor.getParameters();

    for (auto param : params)
    {
        param->removeListener(this);
    }
}

//==============================================================================
void SimpleEQAudioProcessorEditor::paint(juce::Graphics& g)
{
    using namespace juce;

    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black); // NOTE: this is the background of the visualizer

    // Drawing response curve
    auto responseArea = gridLines.getBounds();
    auto w = responseArea.getWidth();

    auto& lowcut = monoChain.get<ChainPositions::LowCut>();
    auto& peak = monoChain.get<ChainPositions::Peak>();
    auto& highcut = monoChain.get<ChainPositions::HighCut>();

    auto sampleRate = audioProcessor.getSampleRate();

    std::vector<double> mags;

    mags.resize(w);

    for (int i = 0; i < w; ++i)
    {
        double mag = 1.f;
        auto freq = mapToLog10(double(i) / double(w), 20.0, 20000.0);
        mag *= peak.coefficients->getMagnitudeForFrequency(freq, sampleRate);

        if (!lowcut.isBypassed<0>()) {
            mag *= lowcut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }
        if (!lowcut.isBypassed<1>()) {
            mag *= lowcut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }
        if (!lowcut.isBypassed<2>()) {
            mag *= lowcut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }
        if (!lowcut.isBypassed<3>()) {
            mag *= lowcut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }

        if (!highcut.isBypassed<0>()) {
            mag *= highcut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }
        if (!highcut.isBypassed<1>()) {
            mag *= highcut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }
        if (!highcut.isBypassed<2>()) {
            mag *= highcut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }
        if (!highcut.isBypassed<3>()) {
            mag *= highcut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }

        mags[i] = Decibels::gainToDecibels(mag);
    }

    Path responseCurve;

    const double outputMin = responseArea.getBottom();
    const double outputMax = responseArea.getY();
    auto map = [outputMin, outputMax](double input)
        {
            return jmap(input, -24.0, 24.0, outputMin, outputMax);
        };

    responseCurve.startNewSubPath(responseArea.getX(), map(mags.front()));

    for (int i = 1; i < mags.size(); ++i)
    {
        responseCurve.lineTo(responseArea.getX() + i, map(mags[i]));
    }

    // For Response curve Grid
    g.drawImage(background, gridLines.getBounds().toFloat());


    // For Response Curve
    g.setColour(Colours::steelblue); // colour of border
    g.drawRoundedRectangle(responseArea.toFloat(), 3, 3);

    g.setColour(Colours::white); // colour of line
    g.strokePath(responseCurve, PathStrokeType(2.f));

    analyzer.setVisible(false);
    gridLines.setVisible(false);

}

void SimpleEQAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    gainControl.setBounds(bounds.removeFromRight(70));
    titleBlock.setBounds(bounds.removeFromTop(50));
    analyzer.setBounds(bounds.removeFromTop(200));

    gridLines.setBounds(analyzer.getBounds());

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
    peakQDial.setBounds(peakControl.getBounds().removeFromBottom(65).removeFromTop(65).removeFromRight(55).removeFromLeft(50));
    peakGainDial.setBounds(peakControl.getBounds().removeFromBottom(65).removeFromTop(65).removeFromLeft(55).removeFromRight(50));

    outGainSlider.setBounds(gainControl.getBounds().removeFromTop(380).removeFromBottom(360));
    outGainLabel.setBounds(gainControl.getBounds().removeFromBottom(30));

    titleLabel.setBounds(titleBlock.getBounds().removeFromBottom(45).removeFromLeft(100));
  
}

void SimpleEQAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{

}

void SimpleEQAudioProcessorEditor::parameterValueChanged(int parameterIndex, float newValue)
{
    parameterChanged = true; // This parameter indicates that a parameter value has changed and 
    // and needs to be updated
}

void SimpleEQAudioProcessorEditor::timerCallback()
{
    if (parameterChanged.compareAndSetBool(false, true))
    {
        // update monochain
        auto chainSettings = getChainSettings(audioProcessor.apvts);
        auto peakCoefficients = makePeakFilter(chainSettings, audioProcessor.getSampleRate());
        updateCoefficients(monoChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);

        auto lowCutCoefficients = makeLowCutFilter(chainSettings, audioProcessor.getSampleRate());
        auto highCutCoefficients = makeHighCutFilter(chainSettings, audioProcessor.getSampleRate());

        updateCutFilter(monoChain.get<ChainPositions::LowCut>(),
            lowCutCoefficients,
            static_cast<Slope>(chainSettings.lowCutSlope));

        updateCutFilter(monoChain.get<ChainPositions::HighCut>(),
            highCutCoefficients,
            static_cast<Slope>(chainSettings.highCutSlope));

        //signal repaint
        repaint();
    }
}

