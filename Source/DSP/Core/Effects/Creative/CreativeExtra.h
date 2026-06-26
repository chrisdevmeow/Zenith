#pragma once
#include "../../Core/Effect.h"

class Looper : public Effect
{
public:
    Looper();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Looper"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float loopLength = 1.0f, mix = 0.5f;
    double sampleRate = 44100.0;
    juce::AudioBuffer<float> loopBuffer;
    int writePos = 0, readPos = 0;
    bool recording = true;
};

class SpectralBlur : public Effect
{
public:
    SpectralBlur();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "SpectralBlur"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float blur = 0.5f, mix = 0.5f;
    juce::dsp::FFT fft{12};
    juce::AudioBuffer<float> fftBuffer;
};

class HarmonicGenerator : public Effect
{
public:
    HarmonicGenerator();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "HarmonicGenerator"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float amount = 0.5f, tone = 0.5f;
};
