#include "PluginProcessor.h"
#include "PluginEditor.h"

// ============================================================================
// Constructor / Destructor
// ============================================================================

ZenithAudioProcessor::ZenithAudioProcessor()
     : AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                                         .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
       apvts (*this, nullptr, "Parameters", createParameterLayout())
{
    // Synth setup
    synth.clearVoices();
    for (int i = 0; i < 16; ++i)
        synth.addVoice (new SynthVoice());

    synth.clearSounds();
    synth.addSound (new SynthSound());

    // UI
    lookAndFeel.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::cyan);
    lookAndFeel.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);

    // Default parameters
    apvts.state.setProperty ("gain", 0.8f, nullptr);

    // Effect rack starts empty
    effectRack.clear();
}

ZenithAudioProcessor::~ZenithAudioProcessor()
{
}

// ============================================================================
// Parameter Layout
// ============================================================================

juce::AudioProcessorValueTreeState::ParameterLayout ZenithAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Oscillator
    layout.add (std::make_unique<juce::AudioParameterChoice> ("waveform", "Waveform",
        juce::StringArray { "Sine", "Saw", "Square", "Triangle" }, 0));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("freq", "Frequency",
        juce::NormalisableRange<float> (20.0f, 2000.0f, 1.0f, 0.5f), 440.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("fine", "Fine Tune",
        juce::NormalisableRange<float> (-100.0f, 100.0f, 0.1f), 0.0f));

    // Filter
    layout.add (std::make_unique<juce::AudioParameterFloat> ("cutoff", "Cutoff",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 1.0f, 0.5f), 8000.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("resonance", "Resonance",
        juce::NormalisableRange<float> (0.1f, 2.0f, 0.01f), 0.5f));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("filterEnv", "Filter Envelope Amount",
        juce::NormalisableRange<float> (0.0f, 10000.0f, 10.0f), 0.0f));

    // Envelope
    layout.add (std::make_unique<juce::AudioParameterFloat> ("attack", "Attack",
        juce::NormalisableRange<float> (0.001f, 2.0f, 0.001f, 0.5f), 0.1f));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("decay", "Decay",
        juce::NormalisableRange<float> (0.001f, 2.0f, 0.001f, 0.5f), 0.2f));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("sustain", "Sustain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.8f));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("release", "Release",
        juce::NormalisableRange<float> (0.001f, 4.0f, 0.001f, 0.5f), 0.3f));

    // LFO
    layout.add (std::make_unique<juce::AudioParameterFloat> ("lfoRate", "LFO Rate",
        juce::NormalisableRange<float> (0.01f, 20.0f, 0.01f, 0.5f), 2.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("lfoDepth", "LFO Depth",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.3f));

    layout.add (std::make_unique<juce::AudioParameterChoice> ("lfoTarget", "LFO Target",
        juce::StringArray { "Pitch", "Filter", "Amp" }, 1));

    // Effects (mix controls)
    layout.add (std::make_unique<juce::AudioParameterFloat> ("chorusMix", "Chorus Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.3f));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("delayMix", "Delay Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.2f));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("reverbMix", "Reverb Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.15f));

    // Master
    layout.add (std::make_unique<juce::AudioParameterFloat> ("masterGain", "Master Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.8f));

    return layout;
}

// ============================================================================
// Audio Processing
// ============================================================================

void ZenithAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* voice = dynamic_cast<SynthVoice*> (synth.getVoice (i)))
            voice->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumOutputChannels());

    effectRack.prepare (sampleRate, samplesPerBlock);
}

void ZenithAudioProcessor::releaseResources()
{
}

void ZenithAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // 1. Synth renders audio
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    // 2. Effects process the audio
    effectRack.process (buffer);

    // 3. Master gain
    float gain = apvts.getRawParameterValue ("masterGain")->load();
    buffer.applyGain (gain);
}

// ============================================================================
// Editor
// ============================================================================

juce::AudioProcessorEditor* ZenithAudioProcessor::createEditor()
{
    return new ZenithAudioEditor (*this);
}

bool ZenithAudioProcessor::hasEditor() const
{
    return true;
}

// ============================================================================
// Basic Info
// ============================================================================

const juce::String ZenithAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ZenithAudioProcessor::acceptsMidi() const
{
    return true;
}

bool ZenithAudioProcessor::producesMidi() const
{
    return false;
}

double ZenithAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

// ============================================================================
// Programs
// ============================================================================

int ZenithAudioProcessor::getNumPrograms()
{
    return 1;
}

int ZenithAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ZenithAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ZenithAudioProcessor::getProgramName (int index)
{
    return {};
}

void ZenithAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

// ============================================================================
// State (Presets)
// ============================================================================

void ZenithAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());

    // Save effect rack state
    auto effectState = effectRack.toVar();
    juce::DynamicObject::Ptr obj = new juce::DynamicObject();
    obj->setProperty ("effects", effectState);
    auto effectXml = juce::var (obj.get()).createXml();

    // Merge into main state
    if (xml && effectXml)
        xml->setChildElement (effectXml.release(), -1);

    copyXmlToBinary (*xml, destData);
}

void ZenithAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml.get() != nullptr)
    {
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));

        // Load effect rack state
        auto effectXml = xml->getChildByName ("effects");
        if (effectXml)
        {
            auto effectVar = juce::var::fromXml (*effectXml);
            effectRack.fromVar (effectVar);
        }
    }
}

// ============================================================================
// Plugin Entry Point
// ============================================================================

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ZenithAudioProcessor();
}
