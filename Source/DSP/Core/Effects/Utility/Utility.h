#pragma once
#include "../../Core/Effect.h"

class Gain : public Effect
{
public:
    Gain();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Gain"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 1; }
private:
    float gain = 0.0f; // dB
};

class Pan : public Effect
{
public:
    Pan();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Pan"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 1; }
private:
    float pan = 0.0f; // -1.0 left, 0.0 center, 1.0 right
};

class Meter : public Effect
{
public:
    Meter();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Meter"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 0; }
    float getPeak() const { return peak; }
private:
    float peak = 0.0f;
};
