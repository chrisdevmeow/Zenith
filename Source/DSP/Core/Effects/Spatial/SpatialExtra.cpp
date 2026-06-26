#include "SpatialExtra.h"
#include <cmath>

HaasEffect::HaasEffect() = default;
void HaasEffect::prepare(double sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    juce::dsp::ProcessSpec spec{sampleRate, (juce::uint32)samplesPerBlock, 1};
    delayL.prepare(spec);
    delayR.prepare(spec);
    delayL.setDelay(juce::dsp::seconds(delayMs / 1000.0));
    delayR.setDelay(juce::dsp::seconds(delayMs / 1000.0));
}
void HaasEffect::process(juce::AudioBuffer<float>& buffer) {
    if (buffer.getNumChannels() < 2) return;
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        float dryL = left[i];
        float dryR = right[i];
        float delayedL = delayL.popSample(0);
        float delayedR = delayR.popSample(0);
        delayL.pushSample(0, dryL);
        delayR.pushSample(0, dryR);
        left[i] = dryL * (1.0f - mix) + delayedR * mix;
        right[i] = dryR * (1.0f - mix) + delayedL * mix;
    }
}
void HaasEffect::reset() { delayL.reset(); delayR.reset(); }
juce::StringArray HaasEffect::getParameterNames() const { return {"DelayMs", "Mix"}; }
float HaasEffect::getParameter(int index) const { return index == 0 ? delayMs : mix; }
void HaasEffect::setParameter(int index, float value) {
    if (index == 0) { delayMs = value; delayL.setDelay(juce::dsp::seconds(delayMs / 1000.0)); delayR.setDelay(juce::dsp::seconds(delayMs / 1000.0)); }
    else if (index == 1) mix = value;
}

StereoDelay::StereoDelay() = default;
void StereoDelay::prepare(double sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    juce::dsp::ProcessSpec spec{sampleRate, (juce::uint32)samplesPerBlock, 1};
    delayL.prepare(spec);
    delayR.prepare(spec);
    delayL.setDelay(juce::dsp::seconds(delayLMs / 1000.0));
    delayR.setDelay(juce::dsp::seconds(delayRMs / 1000.0));
    fbGainL.prepare(spec);
    fbGainR.prepare(spec);
    fbGainL.setGain(feedback);
    fbGainR.setGain(feedback);
}
void StereoDelay::process(juce::AudioBuffer<float>& buffer) {
    if (buffer.getNumChannels() < 2) return;
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        float dryL = left[i];
        float dryR = right[i];
        float delayedL = delayL.popSample(0);
        float delayedR = delayR.popSample(0);
        delayL.pushSample(0, dryL + fbGainL.processSample(0, delayedL));
        delayR.pushSample(0, dryR + fbGainR.processSample(0, delayedR));
        left[i] = dryL + delayedL * 0.5f;
        right[i] = dryR + delayedR * 0.5f;
    }
}
void StereoDelay::reset() { delayL.reset(); delayR.reset(); fbGainL.reset(); fbGainR.reset(); }
juce::StringArray StereoDelay::getParameterNames() const { return {"DelayLMs", "DelayRMs", "Feedback"}; }
float StereoDelay::getParameter(int index) const {
    switch(index){ case 0: return delayLMs; case 1: return delayRMs; case 2: return feedback; default: return 0.0f; }
}
void StereoDelay::setParameter(int index, float value) {
    switch(index){
        case 0: delayLMs = value; delayL.setDelay(juce::dsp::seconds(delayLMs / 1000.0)); break;
        case 1: delayRMs = value; delayR.setDelay(juce::dsp::seconds(delayRMs / 1000.0)); break;
        case 2: feedback = value; fbGainL.setGain(feedback); fbGainR.setGain(feedback); break;
    }
}
