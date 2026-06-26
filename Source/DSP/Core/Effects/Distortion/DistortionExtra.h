#pragma once
#include "../../Core/Effect.h"

class Waveshaper : public Effect
{
public:
    Waveshaper();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Waveshaper"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float drive = 0.5f, symmetry = 0.0f;
};

class Foldback : public Effect
{
public:
    Foldback();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Foldback"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float threshold = 0.5f, gain = 0.5f;
};

class TubeSaturation : public Effect
{
public:
    TubeSaturation();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "TubeSaturation"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float drive = 0.5f, tone = 0.5f;
};
