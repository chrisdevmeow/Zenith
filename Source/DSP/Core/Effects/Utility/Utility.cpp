#include "Utility.h"
#include <cmath>

// ===== Gain =====
Gain::Gain() = default;
void Gain::prepare(double, int) {}
void Gain::process(juce::AudioBuffer<float>& buffer) {
    float gainLinear = std::pow(10.0f, gain / 20.0f);
    buffer.applyGain(gainLinear);
}
void Gain::reset() {}
juce::StringArray Gain::getParameterNames() const { return {"Gain"}; }
float Gain::getParameter(int index) const { return index == 0 ? gain : 0.0f; }
void Gain::setParameter(int index, float value) { if (index == 0) gain = value; }

// ===== Pan =====
Pan::Pan() = default;
void Pan::prepare(double, int) {}
void Pan::process(juce::AudioBuffer<float>& buffer) {
    if (buffer.getNumChannels() < 2) return;
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    float panL = (1.0f - pan) * 0.5f;
    float panR = (1.0f + pan) * 0.5f;
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        left[i] *= panL;
        right[i] *= panR;
    }
}
void Pan::reset() {}
juce::StringArray Pan::getParameterNames() const { return {"Pan"}; }
float Pan::getParameter(int index) const { return index == 0 ? pan : 0.0f; }
void Pan::setParameter(int index, float value) { if (index == 0) pan = value; }

// ===== Meter =====
Meter::Meter() = default;
void Meter::prepare(double, int) { peak = 0.0f; }
void Meter::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float absVal = std::abs(data[i]);
            if (absVal > peak) peak = absVal;
        }
    }
}
void Meter::reset() { peak = 0.0f; }
juce::StringArray Meter::getParameterNames() const { return {}; }
float Meter::getParameter(int) const { return 0.0f; }
void Meter::setParameter(int, float) {}
