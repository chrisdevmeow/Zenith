#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../DSP/Core/EffectFactory.h"

ZenithAudioProcessor::ZenithAudioProcessor()
     : AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                                         .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
       apvts (*this, nullptr, "Parameters", createParameterLayout())
{
    synth.clearVoices();
    for (int i = 0; i < 16; ++i)
        synth.addVoice (new SynthVoice());
    synth.clearSounds();
    synth.addSound (new SynthSound());

    lookAndFeel.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::cyan);
    lookAndFeel.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);

    apvts.state.setProperty ("gain", 0.8f, nullptr);
    effectRack.clear();
}

ZenithAudioProcessor::~ZenithAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout ZenithAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add (std::make_unique<juce::AudioParameterChoice> ("waveform", "Waveform",
        juce::StringArray { "Sine", "Saw", "Square", "Triangle" }, 0));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("freq", "Frequency",
        juce::NormalisableRange<float> (20.0f, 2000.0f, 1.0f, 0.5f), 440.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("fine", "Fine Tune",
        juce::NormalisableRange<float> (-100.0f, 100.0f, 0.1f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("cutoff", "Cutoff",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 1.0f, 0.5f), 8000.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("resonance", "Resonance",
        juce::NormalisableRange<float> (0.1f, 2.0f, 0.01f), 0.5f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("filterEnv", "Filter Envelope Amount",
        juce::NormalisableRange<float> (0.0f, 10000.0f, 10.0f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("attack", "Attack",
        juce::NormalisableRange<float> (0.001f, 2.0f, 0.001f, 0.5f), 0.1f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("decay", "Decay",
        juce::NormalisableRange<float> (0.001f, 2.0f, 0.001f, 0.5f), 0.2f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("sustain", "Sustain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.8f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("release", "Release",
        juce::NormalisableRange<float> (0.001f, 4.0f, 0.001f, 0.5f), 0.3f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("lfoRate", "LFO Rate",
        juce::NormalisableRange<float> (0.01f, 20.0f, 0.01f, 0.5f), 2.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("lfoDepth", "LFO Depth",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.3f));
    layout.add (std::make_unique<juce::AudioParameterChoice> ("lfoTarget", "LFO Target",
        juce::StringArray { "Pitch", "Filter", "Amp" }, 1));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("chorusMix", "Chorus Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.3f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("delayMix", "Delay Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.2f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("reverbMix", "Reverb Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.15f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("masterGain", "Master Gain",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.8f));
    return layout;
}

void ZenithAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* voice = dynamic_cast<SynthVoice*> (synth.getVoice (i)))
            voice->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    effectRack.prepare (sampleRate, samplesPerBlock);
}

void ZenithAudioProcessor::releaseResources() {}

void ZenithAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    // MIDI effects (safe)
    for (int i = 0; i < effectRack.size(); ++i)
    {
        auto* effect = effectRack.getEffect(i);
        if (!effect) continue;
        juce::String name = effect->getName();
        juce::MidiBuffer processedMidi;

        if (name == "ChordGenerator")
            dynamic_cast<ChordGenerator*>(effect)->processMidi (midiMessages, processedMidi);
        else if (name == "Arpeggiator")
            dynamic_cast<Arpeggiator*>(effect)->processMidi (midiMessages, processedMidi, getBpm());
        else if (name == "StepSequencer")
            dynamic_cast<StepSequencer*>(effect)->processMidi (midiMessages, processedMidi, getBpm());
        else if (name == "ScaleFilter")
            dynamic_cast<ScaleFilter*>(effect)->processMidi (midiMessages, processedMidi);
        else
            continue;

        midiMessages.swapWith (processedMidi);
    }

    effectRack.process (buffer);

    float gain = apvts.getRawParameterValue ("masterGain")->load();
    buffer.applyGain (gain);
}

double ZenithAudioProcessor::getBpm() const
{
    if (auto* playhead = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo posInfo;
        if (playhead->getCurrentPosition (posInfo))
            return posInfo.bpm;
    }
    return 120.0;
}

void ZenithAudioProcessor::playChord (int root, int type)
{
    const int intervals[6][4] = {{0,4,7,12},{0,3,7,12},{0,4,7,10},{0,4,7,11},{0,2,7,12},{0,5,7,12}};
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* voice = dynamic_cast<SynthVoice*> (synth.getVoice (i)))
            voice->stopNote (0.0f, true);

    int baseNote = 60 + root;
    for (int i = 0; i < 4; ++i)
    {
        int note = baseNote + intervals[type][i];
        float freq = 440.0f * std::pow (2.0f, (note - 69) / 12.0f);
        for (int v = 0; v < synth.getNumVoices(); ++v)
        {
            if (auto* voice = dynamic_cast<SynthVoice*> (synth.getVoice (v)))
            {
                if (!voice->isVoiceActive())
                {
                    voice->setFrequency (freq);
                    voice->noteOn();
                    break;
                }
            }
        }
    }
}

juce::AudioProcessorEditor* ZenithAudioProcessor::createEditor()
{
    return new ZenithAudioEditor (*this);
}

bool ZenithAudioProcessor::hasEditor() const { return true; }
const juce::String ZenithAudioProcessor::getName() const { return JucePlugin_Name; }
bool ZenithAudioProcessor::acceptsMidi() const { return true; }
bool ZenithAudioProcessor::producesMidi() const { return false; }
double ZenithAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int ZenithAudioProcessor::getNumPrograms() { return 1; }
int ZenithAudioProcessor::getCurrentProgram() { return 0; }
void ZenithAudioProcessor::setCurrentProgram (int) {}
const juce::String ZenithAudioProcessor::getProgramName (int) { return {}; }
void ZenithAudioProcessor::changeProgramName (int, const juce::String&) {}

void ZenithAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    auto xml = state.createXml();
    auto effectState = effectRack.toVar();
    juce::DynamicObject::Ptr obj = new juce::DynamicObject();
    obj->setProperty ("effects", effectState);
    auto effectXml = juce::var (obj.get()).createXml();
    if (xml && effectXml) xml->setChildElement (effectXml.release(), -1);
    copyXmlToBinary (*xml, destData);
}

void ZenithAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto xml = getXmlFromBinary (data, sizeInBytes);
    if (xml)
    {
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
        if (auto* effectXml = xml->getChildByName ("effects"))
            effectRack.fromVar (juce::var::fromXml (*effectXml));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ZenithAudioProcessor();
}
