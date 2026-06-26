#include "SynthVoice.h"

SynthVoice::SynthVoice()
{
    lfo.setFrequency (2.0f);
}

SynthVoice::~SynthVoice()
{
}

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*> (sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float vel, juce::SynthesiserSound*, int)
{
    noteNumber = midiNoteNumber;
    velocity = vel;
    currentFrequency = 440.0f * std::pow (2.0f, (midiNoteNumber - 69) / 12.0f);

    osc.setFrequency (currentFrequency);
    ampEnv.noteOn();
    filterEnv.noteOn();
    isActive = true;
}

void SynthVoice::stopNote (float vel, bool allowTailOff)
{
    ampEnv.noteOff();
    filterEnv.noteOff();
    if (!allowTailOff)
        clearCurrentNote();
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    // Not implemented yet
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    // Not implemented yet
}

void SynthVoice::prepareToPlay (double rate, int samplesPerBlock, int outputChannels)
{
    sampleRate = rate;
    ampEnv.setSampleRate (rate);
    filterEnv.setSampleRate (rate);
    osc.setSampleRate (rate);
    filter.setSampleRate (rate);
    lfo.prepare ({ rate, (juce::uint32) samplesPerBlock, (juce::uint32) outputChannels });
}

void SynthVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!isActive)
        return;

    auto* leftChannel = outputBuffer.getWritePointer (0, startSample);
    auto* rightChannel = outputBuffer.getWritePointer (1, startSample);

    // LFO modulation
    float lfoValue = lfoDepth * lfo.getNextValue();
    float modulatedFreq = currentFrequency;
    float modulatedCutoff = 8000.0f + filterEnvAmount * 0.5f;

    if (lfoTarget == 0) // Pitch
        modulatedFreq *= std::pow (2.0f, lfoValue * 12.0f / 12.0f);
    else if (lfoTarget == 1) // Filter
        modulatedCutoff += lfoValue * 5000.0f;
    // else Amp — handled via gain later

    // Envelopes
    float ampEnvVal = ampEnv.getNextSample();
    float filterEnvVal = filterEnv.getNextSample();

    // Filter modulation
    float finalCutoff = modulatedCutoff + filterEnvVal * filterEnvAmount;
    finalCutoff = juce::jlimit (20.0f, 20000.0f, finalCutoff);

    // Process
    for (int sample = 0; sample < numSamples; ++sample)
    {
        osc.setFrequency (modulatedFreq);
        float sampleValue = osc.getNextSample();

        filter.setCutoff (finalCutoff);
        float filtered = filter.processSample (sampleValue);

        float gain = ampEnvVal * velocity * masterGain;

        // LFO to Amp
        if (lfoTarget == 2)
            gain *= (1.0f + lfoValue * 0.3f);

        leftChannel[sample] += filtered * gain;
        rightChannel[sample] += filtered * gain;
    }

    if (!ampEnv.isActive())
        clearCurrentNote();
}
