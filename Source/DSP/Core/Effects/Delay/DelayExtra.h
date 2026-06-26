#pragma once
#include "../../Core/Effect.h"

class PingPongDelay : public Effect
{
public:
    PingPongDelay();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "PingPongDelay"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayL, delayR;
    juce::dsp::Gain<float> feedbackGainL, feedbackGainR, dryGain, wetGain;
    float delayTimeMs = 300.0f, feedback = 0.5f, mix = 0.3f, crossFeed = 0.5f;
    double sampleRate = 44100.0;
};

class ModulatedDelay : public Effect
{
public:
    ModulatedDelay();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "ModulatedDelay"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 5; }

private:
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    juce::dsp::Gain<float> feedbackGain, dryGain, wetGain;
    float delayTimeMs = 300.0f, feedback = 0.4f, mix = 0.3f, modRate = 2.0f, modDepth = 0.1f;
    double sampleRate = 44100.0;
    float phase = 0.0f;
};
