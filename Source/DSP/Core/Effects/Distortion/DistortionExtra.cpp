#include "DistortionExtra.h"
#include <cmath>

Waveshaper::Waveshaper() = default;
void Waveshaper::prepare(double, int) {}
void Waveshaper::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float x = data[i];
            float x2 = x + symmetry * 0.5f;
            float y = tanh(x2 * (1.0f + drive * 10.0f));
            data[i] = y;
        }
    }
}
void Waveshaper::reset() {}
juce::StringArray Waveshaper::getParameterNames() const { return {"Drive", "Symmetry"}; }
float Waveshaper::getParameter(int index) const { return index == 0 ? drive : symmetry; }
void Waveshaper::setParameter(int index, float value) { if (index == 0) drive = value; else if (index == 1) symmetry = value; }

Foldback::Foldback() = default;
void Foldback::prepare(double, int) {}
void Foldback::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float x = data[i] * (1.0f + gain * 10.0f);
            float t = threshold;
            float y = fmod(x + t, 4.0f * t);
            if (y < 0) y += 4.0f * t;
            if (y > 2.0f * t) y = 4.0f * t - y;
            data[i] = y - t;
        }
    }
}
void Foldback::reset() {}
juce::StringArray Foldback::getParameterNames() const { return {"Threshold", "Gain"}; }
float Foldback::getParameter(int index) const { return index == 0 ? threshold : gain; }
void Foldback::setParameter(int index, float value) { if (index == 0) threshold = value; else if (index == 1) gain = value; }

TubeSaturation::TubeSaturation() = default;
void TubeSaturation::prepare(double, int) {}
void TubeSaturation::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float x = data[i];
            float g = 1.0f + drive * 15.0f;
            float y = (x * g) / (1.0f + fabs(x * g));
            data[i] = y * (1.0f - tone * 0.5f) + x * tone * 0.5f;
        }
    }
}
void TubeSaturation::reset() {}
juce::StringArray TubeSaturation::getParameterNames() const { return {"Drive", "Tone"}; }
float TubeSaturation::getParameter(int index) const { return index == 0 ? drive : tone; }
void TubeSaturation::setParameter(int index, float value) { if (index == 0) drive = value; else if (index == 1) tone = value; }
