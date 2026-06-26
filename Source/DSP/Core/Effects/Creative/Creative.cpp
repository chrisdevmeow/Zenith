#include "Creative.h"
#include <cmath>
#include <random>

GranularProcessor::GranularProcessor() = default;
void GranularProcessor::prepare(double sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    grainBuffer.setSize(1, (int)(sampleRate * grainSize));
    grainBuffer.clear();
    grainPos = 0;
}
void GranularProcessor::process(juce::AudioBuffer<float>& buffer) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            if (dist(gen) < density) {
                if (grainPos < grainBuffer.getNumSamples()) {
                    grainBuffer.setSample(ch, grainPos, data[i]);
                    grainPos++;
                }
                if (grainPos >= grainBuffer.getNumSamples()) {
                    grainPos = 0;
                }
            }
            if (grainBuffer.getNumSamples() > 0) {
                float idx = (float)grainPos * pitch;
                int idxInt = (int)idx % grainBuffer.getNumSamples();
                data[i] = grainBuffer.getSample(ch, idxInt);
            }
        }
    }
}
void GranularProcessor::reset() { grainPos = 0; grainBuffer.clear(); }
juce::StringArray GranularProcessor::getParameterNames() const { return {"GrainSize", "Density", "Pitch"}; }
float GranularProcessor::getParameter(int index) const {
    switch(index){ case 0: return grainSize; case 1: return density; case 2: return pitch; default: return 0.0f; }
}
void GranularProcessor::setParameter(int index, float value) {
    if (index == 0) { grainSize = value; grainBuffer.setSize(1, (int)(sampleRate * grainSize)); }
    else if (index == 1) density = value;
    else if (index == 2) pitch = value;
}

GlitchMachine::GlitchMachine() = default;
void GlitchMachine::prepare(double sampleRate, int) { this->sampleRate = sampleRate; counter = 0; }
void GlitchMachine::process(juce::AudioBuffer<float>& buffer) {
    int glitchInterval = (int)(sampleRate / (rate * 10.0f + 1.0f));
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            counter++;
            if (counter % glitchInterval == 0) {
                float glitch = data[i] * depth * 2.0f;
                data[i] = glitch;
            }
        }
    }
}
void GlitchMachine::reset() { counter = 0; }
juce::StringArray GlitchMachine::getParameterNames() const { return {"Rate", "Depth"}; }
float GlitchMachine::getParameter(int index) const { return index == 0 ? rate : depth; }
void GlitchMachine::setParameter(int index, float value) { if (index == 0) rate = value; else if (index == 1) depth = value; }

Stutter::Stutter() = default;
void Stutter::prepare(double sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    stutterBuffer.setSize(2, (int)(sampleRate * 0.5f));
    stutterBuffer.clear();
    counter = 0;
}
void Stutter::process(juce::AudioBuffer<float>& buffer) {
    int stutterLen = (int)(sampleRate * length * 0.1f);
    int stutterInterval = (int)(sampleRate / (rate * 10.0f + 1.0f));
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            stutterBuffer.setSample(ch, counter % stutterBuffer.getNumSamples(), data[i]);
            counter++;
            if (counter % stutterInterval < stutterLen && stutterLen > 0) {
                int idx = (counter % stutterInterval) % stutterBuffer.getNumSamples();
                data[i] = stutterBuffer.getSample(ch, idx);
            }
        }
    }
}
void Stutter::reset() { counter = 0; stutterBuffer.clear(); }
juce::StringArray Stutter::getParameterNames() const { return {"Rate", "Length"}; }
float Stutter::getParameter(int index) const { return index == 0 ? rate : length; }
void Stutter::setParameter(int index, float value) { if (index == 0) rate = value; else if (index == 1) length = value; }
