#pragma once
#include "../../Core/Effect.h"

class PhaseInvert : public Effect
{
public:
    PhaseInvert();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "PhaseInvert"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 1; }
private:
    bool invert = false;
};

class MonoMaker : public Effect
{
public:
    MonoMaker();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "MonoMaker"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 1; }
private:
    float mix = 0.5f;
};

class Tuner : public Effect
{
public:
    Tuner();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Tuner"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 0; }
    float getFrequency() const { return frequency; }
private:
    float frequency = 0.0f;
    double sampleRate = 44100.0;
    float zeroCrossingCounter = 0.0f;
};
