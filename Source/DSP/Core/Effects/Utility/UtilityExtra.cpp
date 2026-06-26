#include "UtilityExtra.h"
#include <cmath>

// ===== PhaseInvert =====
PhaseInvert::PhaseInvert() = default;
void PhaseInvert::prepare(double, int) {}
void PhaseInvert::process(juce::AudioBuffer<float>& buffer) {
    if (!invert) return;
    buffer.applyGain(-1.0f);
}
void PhaseInvert::reset() {}
juce::StringArray PhaseInvert::getParameterNames() const { return {"Invert"}; }
float PhaseInvert::getParameter(int index) const { return index == 0 ? (invert ? 1.0f : 0.0f) : 0.0f; }
void PhaseInvert::setParameter(int index, float value) { if (index == 0) invert = (value > 0.5f); }

// ===== MonoMaker =====
MonoMaker::MonoMaker() = default;
void MonoMaker::prepare(double, int) {}
void MonoMaker::process(juce::AudioBuffer<float>& buffer) {
    if (buffer.getNumChannels() < 2) return;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float mono = (buffer.getSample(0, i) + buffer.getSample(1, i)) * 0.5f;
            data[i] = data[i] * (1.0f - mix) + mono * mix;
        }
    }
}
void MonoMaker::reset() {}
juce::StringArray MonoMaker::getParameterNames() const { return {"Mix"}; }
float MonoMaker::getParameter(int index) const { return index == 0 ? mix : 0.0f; }
void MonoMaker::setParameter(int index, float value) { if (index == 0) mix = value; }

// ===== Tuner =====
Tuner::Tuner() = default;
void Tuner::prepare(double sampleRate, int) { this->sampleRate = sampleRate; zeroCrossingCounter = 0.0f; frequency = 0.0f; }
void Tuner::process(juce::AudioBuffer<float>& buffer) {
    if (buffer.getNumChannels() == 0) return;
    auto* data = buffer.getReadPointer(0);
    float zeroCrossings = 0.0f;
    for (int i = 1; i < buffer.getNumSamples(); ++i) {
        if ((data[i] > 0.0f && data[i - 1] <= 0.0f) || (data[i] < 0.0f && data[i - 1] >= 0.0f)) {
            zeroCrossings++;
        }
    }
    zeroCrossingCounter += zeroCrossings;
    if (zeroCrossingCounter >= 2.0f) {
        frequency = (float)(zeroCrossingCounter * sampleRate / (buffer.getNumSamples() * 2));
        zeroCrossingCounter = 0.0f;
    }
}
void Tuner::reset() { frequency = 0.0f; zeroCrossingCounter = 0.0f; }
juce::StringArray Tuner::getParameterNames() const { return {}; }
float Tuner::getParameter(int) const { return 0.0f; }
void Tuner::setParameter(int, float) {}
