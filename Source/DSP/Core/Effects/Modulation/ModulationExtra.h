#pragma once
#include "../../Core/Effect.h"

class AutoPan : public Effect
{
public:
    AutoPan();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "AutoPan"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float rate = 1.0f, depth = 1.0f;
    double sampleRate = 44100.0;
    float phase = 0.0f;
};

class RotarySpeaker : public Effect
{
public:
    RotarySpeaker();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "RotarySpeaker"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float rate = 2.0f, depth = 0.5f;
    double sampleRate = 44100.0;
    float phase = 0.0f;
};

class Vocoder : public Effect
{
public:
    Vocoder();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Vocoder"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float bands = 20.0f, mix = 0.5f;
};
