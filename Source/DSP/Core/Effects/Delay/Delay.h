#pragma once
#include "../../Core/Effect.h"

class DigitalDelay : public Effect
{
public:
    DigitalDelay();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "DigitalDelay"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    juce::dsp::Gain<float> feedbackGain, dryGain, wetGain;
    float delayTimeMs = 300.0f, feedback = 0.4f, mix = 0.2f;
    double sampleRate = 44100.0;
    int delayInSamples = 0;
};

class AnalogDelay : public Effect
{
public:
    AnalogDelay();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "AnalogDelay"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    juce::dsp::Gain<float> feedbackGain, dryGain, wetGain;
    juce::dsp::IIR::Filter<float> lowpass;
    float delayTimeMs = 300.0f, feedback = 0.4f, mix = 0.2f, cutoff = 2000.0f;
    double sampleRate = 44100.0;
    int delayInSamples = 0;
};

class TapeDelay : public Effect
{
public:
    TapeDelay();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "TapeDelay"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 5; }

private:
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    juce::dsp::Gain<float> feedbackGain, dryGain, wetGain;
    juce::dsp::IIR::Filter<float> lowpass, highpass;
    float delayTimeMs = 300.0f, feedback = 0.5f, mix = 0.2f, wow = 0.1f, flutter = 0.05f;
    double sampleRate = 44100.0;
    int delayInSamples = 0;
    float phase = 0.0f;
};
