#pragma once
#include "../../Core/Effect.h"

class GranularProcessor : public Effect
{
public:
    GranularProcessor();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "GranularProcessor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 3; }
private:
    float grainSize = 0.1f, density = 0.5f, pitch = 1.0f;
    double sampleRate = 44100.0;
    juce::AudioBuffer<float> grainBuffer;
    int grainPos = 0;
};

class GlitchMachine : public Effect
{
public:
    GlitchMachine();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "GlitchMachine"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float rate = 0.5f, depth = 0.5f;
    double sampleRate = 44100.0;
    int counter = 0;
};

class Stutter : public Effect
{
public:
    Stutter();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Stutter"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float rate = 0.5f, length = 0.5f;
    double sampleRate = 44100.0;
    int counter = 0;
    juce::AudioBuffer<float> stutterBuffer;
};
