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
    // MIDI processing
    void processMidi (juce::MidiBuffer& midiMessages, juce::MidiBuffer& outputBuffer);
private:
    int root = 0;
    int chordType = 0;
    juce::Array<int> heldNotes;
    bool chordActive = false;
    int lastRootNote = 0;
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
    // MIDI processing
    void processMidi (juce::MidiBuffer& midiMessages, juce::MidiBuffer& outputBuffer, double bpm);
private:
    float rate = 0.5f, gate = 0.5f;
    double sampleRate = 44100.0;
    double stepTime = 0.0;
    int stepIndex = 0;
    juce::Array<int> heldNotes;
    juce::Array<int> arpeggioNotes;
    bool arpeggioActive = false;
};
