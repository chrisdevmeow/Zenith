#pragma once
#include "../../Core/Effect.h"

class StepSequencer : public Effect
{
public:
    StepSequencer();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "StepSequencer"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
    // MIDI processing
    void processMidi (juce::MidiBuffer& midiMessages, juce::MidiBuffer& outputBuffer, double bpm);
private:
    float rate = 0.5f, gate = 0.5f;
    double sampleRate = 44100.0;
    int step = 0;
    juce::Array<int> sequence;
    bool active = false;
};

class ScaleFilter : public Effect
{
public:
    ScaleFilter();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "ScaleFilter"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 1; }
    // MIDI processing
    void processMidi (juce::MidiBuffer& midiMessages, juce::MidiBuffer& outputBuffer);
private:
    int scaleType = 0;
    // Scale patterns: 0=major, 1=minor, 2=pentatonic
    const int majorScale[7] = {0, 2, 4, 5, 7, 9, 11};
    const int minorScale[7] = {0, 2, 3, 5, 7, 8, 10};
    const int pentatonicScale[5] = {0, 3, 5, 7, 10};
};
