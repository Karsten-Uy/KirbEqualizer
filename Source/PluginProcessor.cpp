/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleEQAudioProcessor::SimpleEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

SimpleEQAudioProcessor::~SimpleEQAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleEQAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SimpleEQAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SimpleEQAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SimpleEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleEQAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SimpleEQAudioProcessor::getProgramName(int index)
{
    return {};
}

void SimpleEQAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleEQAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec; // the block to be passed into the filters

    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    spec.sampleRate = sampleRate;

    leftChain.prepare(spec); // send to filters, they process stuff in mono so you have to split them here
    rightChain.prepare(spec);

    auto chainSettings = getChainSettings(apvts); // gets the chain settings

    updatePeakFilter(chainSettings);

    auto lowCutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(
        chainSettings.lowCutFreq,
        sampleRate,
        2 * (chainSettings.lowCutSlope + 1)); // This is for filter, to find why, refer to tutorial, 1:00:00

    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
    updateCutFilter(leftLowCut, lowCutCoefficients, static_cast<Slope>(chainSettings.lowCutSlope));

    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();
    updateCutFilter(rightLowCut, lowCutCoefficients, static_cast<Slope>(chainSettings.lowCutSlope));



    auto highCutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(
        chainSettings.highCutFreq,
        sampleRate,
        2 * (chainSettings.highCutSlope + 1)); // This is for filter, to find why, refer to tutorial, 1:00:00

    auto& lefthighCut = leftChain.get<ChainPositions::HighCut>();
    updateCutFilter(lefthighCut, highCutCoefficients, static_cast<Slope>(chainSettings.highCutSlope));

    auto& righthighCut = rightChain.get<ChainPositions::HighCut>();
    updateCutFilter(righthighCut, highCutCoefficients, static_cast<Slope>(chainSettings.highCutSlope));



}

void SimpleEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleEQAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void SimpleEQAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto chainSettings = getChainSettings(apvts); // gets the chain settings

    updatePeakFilter(chainSettings);

    auto lowCutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(
        chainSettings.lowCutFreq,
        getSampleRate(),
        2 * (chainSettings.lowCutSlope + 1)); // This is for filter, to find why, refer to tutorial, 1:00:00

    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
    updateCutFilter(leftLowCut, lowCutCoefficients, static_cast<Slope>(chainSettings.lowCutSlope));

    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();
    updateCutFilter(rightLowCut, lowCutCoefficients, static_cast<Slope>(chainSettings.lowCutSlope));

    auto highCutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(
        chainSettings.highCutFreq,
        getSampleRate(),
        2 * (chainSettings.highCutSlope + 1)); // This is for filter, to find why, refer to tutorial, 1:00:00

    auto& lefthighCut = leftChain.get<ChainPositions::HighCut>();
    updateCutFilter(lefthighCut, highCutCoefficients, static_cast<Slope>(chainSettings.highCutSlope));

    auto& righthighCut = rightChain.get<ChainPositions::HighCut>();
    updateCutFilter(righthighCut, highCutCoefficients, static_cast<Slope>(chainSettings.highCutSlope));



    juce::dsp::AudioBlock<float> block(buffer); // buffer has the audio information

    auto leftBlock = block.getSingleChannelBlock(0); // gets left channel
    auto rightBlock = block.getSingleChannelBlock(1); // gets right channel

    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock); // provides a wrapper around the block that the chain can process
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext); // processes the channels
    rightChain.process(rightContext);


}

//==============================================================================
bool SimpleEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleEQAudioProcessor::createEditor()
{
    // return new SimpleEQAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void SimpleEQAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleEQAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings settings;

    settings.lowCutFreq = apvts.getRawParameterValue("LowCut Freq")->load();
    settings.highCutFreq = apvts.getRawParameterValue("HighCut Freq")->load();
    settings.peakFreq = apvts.getRawParameterValue("Peak Freq")->load();
    settings.peakGainInDecibels = apvts.getRawParameterValue("Peak Gain")->load();
    settings.peakQuality = apvts.getRawParameterValue("Peak Quality")->load();
    settings.lowCutSlope = static_cast<Slope>(apvts.getRawParameterValue("LowCut Slope")->load()); // need cast to satisfy compiler
    settings.highCutSlope = static_cast<Slope>(apvts.getRawParameterValue("HighCut Slope")->load());

    return settings;
}

void SimpleEQAudioProcessor::updateCoefficients(Coefficients& old, const Coefficients& replacements)
{
    *old = *replacements;
}

void SimpleEQAudioProcessor::updatePeakFilter(const ChainSettings& chainSettings)
{
    auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter( // this is here to ensure that the setting changes are being applied in real time
        getSampleRate(),
        chainSettings.peakFreq,
        chainSettings.peakQuality,
        juce::Decibels::decibelsToGain(chainSettings.peakGainInDecibels));

    updateCoefficients(leftChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
    updateCoefficients(rightChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
}


juce::AudioProcessorValueTreeState::ParameterLayout SimpleEQAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "LowCut Freq",
        "LowCut Freq",
        juce::NormalisableRange<float>(
            20.f, // low
            20000.f, // high
            1.f, // step amount in frequency
            0.25f), // skeq amount, if <0, lower ranges are changed more and vice verca
        20.f)); // starting freq

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "HighCut Freq",
        "HighCut Freq",
        juce::NormalisableRange<float>(
            20.f,
            20000.f,
            1.f,
            0.25f),
        20000.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "Peak Freq",
        "Peak Freq",
        juce::NormalisableRange<float>(
            20.f,
            20000.f,
            1.f,
            0.25f),
        750.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "Peak Gain",
        "Peak Gain",
        juce::NormalisableRange<float>(
            -24.f,
            24.f,
            0.5f,
            1.f),
        0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "Peak Quality",
        "Peak Quality",
        juce::NormalisableRange<float>(
            0.1f,
            10.f,
            0.05f,
            1.f),
        1.f));

    juce::StringArray stringArray;
    for (int i = 0; i < 4; ++i) {
        juce::String str;
        str << (12 + i * 12);
        str << " db/Oct";
        stringArray.add(str);
    }

    layout.add(std::make_unique<juce::AudioParameterChoice>("LowCut Slope", "LowCut Slope", stringArray, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("HighCut Slope", "HighCut Slope", stringArray, 0));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleEQAudioProcessor();
}
