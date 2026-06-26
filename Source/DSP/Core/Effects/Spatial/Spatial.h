#pragma once
#include "../../Core/Effect.h"

class StereoWidener : public Effect
{
public:
    StereoWidener();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "StereoWidener"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float width = 1.0f, mix = 0.5f;
};

class MSProcessor : public Effect
{
public:
    MSProcessor();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "MSProcessor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float midGain = 0.0f, sideGain = 0.0f;
};
