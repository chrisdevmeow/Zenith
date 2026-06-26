#include "CreativeExtra.h"
#include <cmath>

Looper::Looper() = default;
void Looper::prepare(double sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    loopBuffer.setSize(2, (int)(sampleRate * loopLength));
    loopBuffer.clear();
    writePos = 0;
    readPos = 0;
    recording = true;
}
void Looper::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            if (recording) {
                loopBuffer.setSample(ch, writePos, data[i]);
                writePos = (writePos + 1) % loopBuffer.getNumSamples();
                if (writePos == 0) recording = false;
            }
            float loopSample = loopBuffer.getSample(ch, readPos);
            readPos = (readPos + 1) % loopBuffer.getNumSamples();
            data[i] = data[i] * (1.0f - mix) + loopSample * mix;
        }
    }
}
void Looper::reset() { loopBuffer.clear(); writePos = 0; readPos = 0; recording = true; }
juce::StringArray Looper::getParameterNames() const { return {"LoopLength", "Mix"}; }
float Looper::getParameter(int index) const { return index == 0 ? loopLength : mix; }
void Looper::setParameter(int index, float value) {
    if (index == 0) { loopLength = value; loopBuffer.setSize(2, (int)(sampleRate * loopLength)); }
    else if (index == 1) mix = value;
}

SpectralBlur::SpectralBlur() = default;
void SpectralBlur::prepare(double sampleRate, int samplesPerBlock) {
    fftBuffer.setSize(2, 1 << 12);
    fftBuffer.clear();
}
void SpectralBlur::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            fftBuffer.setSample(ch, i % fftBuffer.getNumSamples(), data[i]);
            if (i % fftBuffer.getNumSamples() == fftBuffer.getNumSamples() - 1) {
                // Simplified spectral blur: average adjacent samples
                for (int j = 1; j < fftBuffer.getNumSamples() - 1; ++j) {
                    float avg = (fftBuffer.getSample(ch, j - 1) + fftBuffer.getSample(ch, j + 1)) * 0.5f;
                    fftBuffer.setSample(ch, j, fftBuffer.getSample(ch, j) * (1.0f - blur) + avg * blur);
                }
            }
            data[i] = data[i] * (1.0f - mix) + fftBuffer.getSample(ch, i % fftBuffer.getNumSamples()) * mix;
        }
    }
}
void SpectralBlur::reset() { fftBuffer.clear(); }
juce::StringArray SpectralBlur::getParameterNames() const { return {"Blur", "Mix"}; }
float SpectralBlur::getParameter(int index) const { return index == 0 ? blur : mix; }
void SpectralBlur::setParameter(int index, float value) { if (index == 0) blur = value; else if (index == 1) mix = value; }

HarmonicGenerator::HarmonicGenerator() = default;
void HarmonicGenerator::prepare(double, int) {}
void HarmonicGenerator::process(juce::AudioBuffer<float>& buffer) {
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float x = data[i];
            float harm = x * 0.5f + x * x * 0.25f + x * x * x * 0.125f;
            data[i] = x * (1.0f - amount) + harm * amount * (1.0f + tone);
        }
    }
}
void HarmonicGenerator::reset() {}
juce::StringArray HarmonicGenerator::getParameterNames() const { return {"Amount", "Tone"}; }
float HarmonicGenerator::getParameter(int index) const { return index == 0 ? amount : tone; }
void HarmonicGenerator::setParameter(int index, float value) { if (index == 0) amount = value; else if (index == 1) tone = value; }
