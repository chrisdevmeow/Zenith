#pragma once
#include "../../Core/Effect.h"

class ParametricEQ : public Effect
{
public:
    ParametricEQ();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "ParametricEQ"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 12; } // 4 bands × 3 params

private:
    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>, 
                              juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>> filterChain;
    float freq[4] = { 100.0f, 400.0f, 2000.0f, 8000.0f };
    float gain[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float q[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    double sampleRate = 44100.0;
};

class GraphicEQ : public Effect
{
public:
    GraphicEQ();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "GraphicEQ"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 10; }

private:
    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>,
                              juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>,
                              juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>,
                              juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>,
                              juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Filter<float>> filterChain;
    float gains[10] = { 0.0f };
    double sampleRate = 44100.0;
};

class HighPassFilter : public Effect
{
public:
    HighPassFilter();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "HighPassFilter"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 2; }

private:
    juce::dsp::IIR::Filter<float> filter;
    float frequency = 80.0f;
    float slope = 24.0f;
    double sampleRate = 44100.0;
};

class LowPassFilter : public Effect
{
public:
    LowPassFilter();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "LowPassFilter"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 2; }

private:
    juce::dsp::IIR::Filter<float> filter;
    float frequency = 12000.0f;
    float slope = 24.0f;
    double sampleRate = 44100.0;
};
