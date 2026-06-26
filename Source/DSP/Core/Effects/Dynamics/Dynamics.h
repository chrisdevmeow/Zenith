#pragma once
#include "../../Core/Effect.h"

class Compressor : public Effect
{
public:
    Compressor();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Compressor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 5; }

private:
    juce::dsp::Compressor<float> compressor;
    float threshold = -12.0f, ratio = 4.0f, attack = 10.0f, release = 100.0f, makeup = 0.0f;
};

class Limiter : public Effect
{
public:
    Limiter();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Limiter"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    juce::dsp::Limiter<float> limiter;
    float threshold = -6.0f, release = 100.0f, ceiling = 0.0f;
};

class Expander : public Effect
{
public:
    Expander();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Expander"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::Expander<float> expander;
    float threshold = -20.0f, ratio = 2.0f, attack = 10.0f, release = 100.0f;
};

class Gate : public Effect
{
public:
    Gate();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Gate"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::Gate<float> gate;
    float threshold = -40.0f, attack = 10.0f, release = 100.0f, hold = 50.0f;
};

class DeEsser : public Effect
{
public:
    DeEsser();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "DeEsser"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    juce::dsp::Deesser<float> deesser;
    float frequency = 6000.0f, threshold = -20.0f, width = 1000.0f;
};

class TransientShaper : public Effect
{
public:
    TransientShaper();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "TransientShaper"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    float attack = 0.5f, sustain = 0.5f, output = 0.0f;
    double sampleRate = 44100.0;
};
