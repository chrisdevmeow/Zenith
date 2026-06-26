#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "../DSP/Oscillator.h"
#include "../DSP/Filter.h"
#include "../DSP/Envelope.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice();
    ~SynthVoice() override;

    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);

    // Parameter setters (called from processor)
    void setWaveform (int waveform) { osc.setWaveform (waveform); }
    void setFreq (float freq) { osc.setFrequency (freq); }
    void setFine (float fine) { osc.setFine (fine); }
    void setCutoff (float cutoff) { filter.setCutoff (cutoff); }
    void setResonance (float res) { filter.setResonance (res); }
    void setFilterEnvAmount (float amount) { filterEnvAmount = amount; }
    void setAttack (float attack) { ampEnv.setAttack (attack); filterEnv.setAttack (attack); }
    void setDecay (float decay) { ampEnv.setDecay (decay); filterEnv.setDecay (decay); }
    void setSustain (float sustain) { ampEnv.setSustain (sustain); filterEnv.setSustain (sustain); }
    void setRelease (float release) { ampEnv.setRelease (release); filterEnv.setRelease (release); }
    void setLfoRate (float rate) { lfo.setRate (rate); }
    void setLfoDepth (float depth) { lfoDepth = depth; }
    void setLfoTarget (int target) { lfoTarget = target; }

    void setMasterGain (float gain) { masterGain = gain; }

private:
    Oscillator osc;
    Filter filter;
    Envelope ampEnv;
    Envelope filterEnv;
    juce::dsp::LFO<float> lfo;

    float currentFrequency = 440.0f;
    float filterEnvAmount = 0.0f;
    float lfoDepth = 0.0f;
    int lfoTarget = 1; // 0=pitch, 1=filter, 2=amp
    float masterGain = 0.8f;
    float sampleRate = 44100.0f;

    int noteNumber = 0;
    float velocity = 0.0f;
    bool isActive = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthVoice)
};
