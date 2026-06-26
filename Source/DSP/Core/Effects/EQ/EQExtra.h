#pragma once
#include "../../Core/Effect.h"

class BandPassFilter : public Effect
{
public:
    BandPassFilter();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "BandPassFilter"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    juce::dsp::IIR::Filter<float> filter;
    float frequency = 1000.0f, q = 1.0f, gain = 0.0f;
    double sampleRate = 44100.0;
};

class BandStopFilter : public Effect
{
public:
    BandStopFilter();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "BandStopFilter"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    juce::dsp::IIR::Filter<float> filter;
    float frequency = 1000.0f, q = 1.0f, gain = 0.0f;
    double sampleRate = 44100.0;
};

class DynamicEQ : public Effect
{
public:
    DynamicEQ();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "DynamicEQ"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 5; }

private:
    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>, juce::dsp::Compressor<float>> chain;
    float frequency = 1000.0f, q = 1.0f, threshold = -12.0f, ratio = 4.0f, makeup = 0.0f;
    double sampleRate = 44100.0;
};
