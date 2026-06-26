#pragma once
#include "../../Core/Effect.h"

class HallReverb : public Effect
{
public:
    HallReverb();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "HallReverb"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    juce::dsp::Reverb reverb;
    float roomSize = 0.7f, damping = 0.5f, wetLevel = 0.3f;
};

class RoomReverb : public Effect
{
public:
    RoomReverb();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "RoomReverb"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    juce::dsp::Reverb reverb;
    float roomSize = 0.3f, damping = 0.3f, wetLevel = 0.2f;
};

class PlateReverb : public Effect
{
public:
    PlateReverb();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "PlateReverb"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    juce::dsp::Reverb reverb;
    float roomSize = 0.5f, damping = 0.1f, wetLevel = 0.4f;
};
