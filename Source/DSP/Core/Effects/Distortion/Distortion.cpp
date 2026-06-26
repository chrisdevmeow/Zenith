#include "Distortion.h"
#include <cmath>

Overdrive::Overdrive() = default;
void Overdrive::prepare(double, int) {}
void Overdrive::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float x = data[i];
            float driveAmt = 1.0f + drive * 10.0f;
            float y = tanh(x * driveAmt);
            data[i] = y * (1.0f - tone * 0.5f) + x * tone * 0.5f;
        }
    }
}
void Overdrive::reset() {}
juce::StringArray Overdrive::getParameterNames() const { return {"Drive", "Tone"}; }
float Overdrive::getParameter(int index) const { return index == 0 ? drive : tone; }
void Overdrive::setParameter(int index, float value) { if (index == 0) drive = value; else if (index == 1) tone = value; }

Distortion::Distortion() = default;
void Distortion::prepare(double, int) {}
void Distortion::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float x = data[i];
            float g = 1.0f + gain * 20.0f;
            float y = (x * g) / (1.0f + fabs(x * g));
            data[i] = y * (1.0f - tone * 0.5f) + x * tone * 0.5f;
        }
    }
}
void Distortion::reset() {}
juce::StringArray Distortion::getParameterNames() const { return {"Gain", "Tone"}; }
float Distortion::getParameter(int index) const { return index == 0 ? gain : tone; }
void Distortion::setParameter(int index, float value) { if (index == 0) gain = value; else if (index == 1) tone = value; }

Fuzz::Fuzz() = default;
void Fuzz::prepare(double, int) {}
void Fuzz::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float x = data[i];
            float g = 1.0f + gain * 30.0f;
            float y = tanh(x * g);
            if (y > 0.8f) y = 0.8f;
            if (y < -0.8f) y = -0.8f;
            data[i] = y * (1.0f - tone * 0.5f) + x * tone * 0.5f;
        }
    }
}
void Fuzz::reset() {}
juce::StringArray Fuzz::getParameterNames() const { return {"Gain", "Tone"}; }
float Fuzz::getParameter(int index) const { return index == 0 ? gain : tone; }
void Fuzz::setParameter(int index, float value) { if (index == 0) gain = value; else if (index == 1) tone = value; }

Bitcrusher::Bitcrusher() = default;
void Bitcrusher::prepare(double, int) {}
void Bitcrusher::process(juce::AudioBuffer<float>& buffer) {
    int maxVal = (1 << bits) - 1;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float x = data[i];
            int quant = (int)((x * 0.5f + 0.5f) * maxVal);
            float y = (float)quant / maxVal * 2.0f - 1.0f;
            data[i] = y * (1.0f - reduction) + x * reduction;
        }
    }
}
void Bitcrusher::reset() {}
juce::StringArray Bitcrusher::getParameterNames() const { return {"Bits", "Reduction"}; }
float Bitcrusher::getParameter(int index) const { return index == 0 ? (float)bits : reduction; }
void Bitcrusher::setParameter(int index, float value) { if (index == 0) bits = (int)value; else if (index == 1) reduction = value; }
