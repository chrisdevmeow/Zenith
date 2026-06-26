#pragma once
#include "../../Core/Effect.h"

class HaasEffect : public Effect
{
public:
    HaasEffect();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "HaasEffect"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float delayMs = 20.0f, mix = 0.5f;
    double sampleRate = 44100.0;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayL, delayR;
};

class StereoDelay : public Effect
{
public:
    StereoDelay();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "StereoDelay"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 3; }
private:
    float delayLMs = 300.0f, delayRMs = 600.0f, feedback = 0.3f;
    double sampleRate = 44100.0;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayL, delayR;
    juce::dsp::Gain<float> fbGainL, fbGainR;
};
