#pragma once
#include "../../Core/Effect.h"

class SpringReverb : public Effect
{
public:
    SpringReverb();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "SpringReverb"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    juce::dsp::Reverb reverb;
    float roomSize = 0.2f, damping = 0.9f, wetLevel = 0.5f;
};

class ConvolutionReverb : public Effect
{
public:
    ConvolutionReverb();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "ConvolutionReverb"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 2; }

private:
    juce::dsp::Convolution convolution;
    float wetLevel = 0.5f, dryLevel = 0.5f;
};

class CathedralReverb : public Effect
{
public:
    CathedralReverb();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "CathedralReverb"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    juce::dsp::Reverb reverb;
    float roomSize = 0.9f, damping = 0.1f, wetLevel = 0.6f;
};
