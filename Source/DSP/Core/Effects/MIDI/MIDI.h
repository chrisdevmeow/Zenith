#pragma once
#include "../../Core/Effect.h"

class ChordGenerator : public Effect
{
public:
    ChordGenerator();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "ChordGenerator"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    int root = 0; // 0 = C
    int chordType = 0; // 0 = major, 1 = minor, 2 = 7th
    juce::MidiBuffer midiBuffer;
};

class Arpeggiator : public Effect
{
public:
    Arpeggiator();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Arpeggiator"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float rate = 0.5f, gate = 0.5f;
    double sampleRate = 44100.0;
    int step = 0;
    juce::MidiBuffer midiBuffer;
};
