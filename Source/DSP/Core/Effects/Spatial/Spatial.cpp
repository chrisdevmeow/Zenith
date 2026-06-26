#include "Spatial.h"
#include <cmath>

StereoWidener::StereoWidener() = default;
void StereoWidener::prepare(double, int) {}
void StereoWidener::process(juce::AudioBuffer<float>& buffer) {
    if (buffer.getNumChannels() < 2) return;
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        float m = (left[i] + right[i]) * 0.5f;
        float s = (left[i] - right[i]) * 0.5f * width;
        float newL = m + s;
        float newR = m - s;
        left[i] = left[i] * (1.0f - mix) + newL * mix;
        right[i] = right[i] * (1.0f - mix) + newR * mix;
    }
}
void StereoWidener::reset() {}
juce::StringArray StereoWidener::getParameterNames() const { return {"Width", "Mix"}; }
float StereoWidener::getParameter(int index) const { return index == 0 ? width : mix; }
void StereoWidener::setParameter(int index, float value) { if (index == 0) width = value; else if (index == 1) mix = value; }

MSProcessor::MSProcessor() = default;
void MSProcessor::prepare(double, int) {}
void MSProcessor::process(juce::AudioBuffer<float>& buffer) {
    if (buffer.getNumChannels() < 2) return;
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        float m = (left[i] + right[i]) * 0.5f;
        float s = (left[i] - right[i]) * 0.5f;
        float midGainLinear = std::pow(10.0f, midGain / 20.0f);
        float sideGainLinear = std::pow(10.0f, sideGain / 20.0f);
        left[i] = m * midGainLinear + s * sideGainLinear;
        right[i] = m * midGainLinear - s * sideGainLinear;
    }
}
void MSProcessor::reset() {}
juce::StringArray MSProcessor::getParameterNames() const { return {"MidGain", "SideGain"}; }
float MSProcessor::getParameter(int index) const { return index == 0 ? midGain : sideGain; }
void MSProcessor::setParameter(int index, float value) { if (index == 0) midGain = value; else if (index == 1) sideGain = value; }
