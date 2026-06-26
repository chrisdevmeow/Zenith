#include "ModulationExtra.h"
#include <cmath>

AutoPan::AutoPan() = default;
void AutoPan::prepare(double sampleRate, int) { this->sampleRate = sampleRate; phase = 0.0f; }
void AutoPan::process(juce::AudioBuffer<float>& buffer) {
    if (buffer.getNumChannels() < 2) return;
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        phase += 2.0f * juce::MathConstants<float>::pi * rate / (float)sampleRate;
        if (phase >= 2.0f * juce::MathConstants<float>::pi) phase -= 2.0f * juce::MathConstants<float>::pi;
        float pan = depth * std::sin(phase);
        left[i] *= (1.0f - pan) * 0.5f;
        right[i] *= (1.0f + pan) * 0.5f;
    }
}
void AutoPan::reset() { phase = 0.0f; }
juce::StringArray AutoPan::getParameterNames() const { return {"Rate","Depth"}; }
float AutoPan::getParameter(int index) const { return index == 0 ? rate : depth; }
void AutoPan::setParameter(int index, float value) { if (index == 0) rate = value; else if (index == 1) depth = value; }

RotarySpeaker::RotarySpeaker() = default;
void RotarySpeaker::prepare(double sampleRate, int) { this->sampleRate = sampleRate; phase = 0.0f; }
void RotarySpeaker::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            phase += 2.0f * juce::MathConstants<float>::pi * rate / (float)sampleRate;
            if (phase >= 2.0f * juce::MathConstants<float>::pi) phase -= 2.0f * juce::MathConstants<float>::pi;
            data[i] *= (1.0f + depth * (0.5f + 0.5f * std::sin(phase)));
        }
    }
}
void RotarySpeaker::reset() { phase = 0.0f; }
juce::StringArray RotarySpeaker::getParameterNames() const { return {"Rate","Depth"}; }
float RotarySpeaker::getParameter(int index) const { return index == 0 ? rate : depth; }
void RotarySpeaker::setParameter(int index, float value) { if (index == 0) rate = value; else if (index == 1) depth = value; }

Vocoder::Vocoder() = default;
void Vocoder::prepare(double, int) {}
void Vocoder::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            data[i] *= mix;
        }
    }
}
void Vocoder::reset() {}
juce::StringArray Vocoder::getParameterNames() const { return {"Bands","Mix"}; }
float Vocoder::getParameter(int index) const { return index == 0 ? bands : mix; }
void Vocoder::setParameter(int index, float value) { if (index == 0) bands = value; else if (index == 1) mix = value; }
