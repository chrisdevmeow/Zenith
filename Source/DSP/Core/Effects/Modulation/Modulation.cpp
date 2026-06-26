#include "Modulation.h"
#include <cmath>

// ===== Chorus =====
Chorus::Chorus() = default;
void Chorus::prepare(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec spec{sampleRate, (juce::uint32)samplesPerBlock, 2};
    chorus.prepare(spec);
    chorus.setRate(rate);
    chorus.setDepth(depth);
    chorus.setMix(mix);
}
void Chorus::process(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    chorus.process(juce::dsp::ProcessContextReplacing<float>(block));
}
void Chorus::reset() { chorus.reset(); }
juce::StringArray Chorus::getParameterNames() const { return {"Rate","Depth","Mix"}; }
float Chorus::getParameter(int index) const {
    switch(index){ case 0: return rate; case 1: return depth; case 2: return mix; default: return 0.0f; }
}
void Chorus::setParameter(int index, float value) {
    switch(index){ case 0: rate=value; chorus.setRate(rate); break; case 1: depth=value; chorus.setDepth(depth); break; case 2: mix=value; chorus.setMix(mix); break; }
}

// ===== Flanger =====
Flanger::Flanger() = default;
void Flanger::prepare(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec spec{sampleRate, (juce::uint32)samplesPerBlock, 2};
    flanger.prepare(spec);
    flanger.setRate(rate);
    flanger.setDepth(depth);
    flanger.setMix(mix);
}
void Flanger::process(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    flanger.process(juce::dsp::ProcessContextReplacing<float>(block));
}
void Flanger::reset() { flanger.reset(); }
juce::StringArray Flanger::getParameterNames() const { return {"Rate","Depth","Mix"}; }
float Flanger::getParameter(int index) const {
    switch(index){ case 0: return rate; case 1: return depth; case 2: return mix; default: return 0.0f; }
}
void Flanger::setParameter(int index, float value) {
    switch(index){ case 0: rate=value; flanger.setRate(rate); break; case 1: depth=value; flanger.setDepth(depth); break; case 2: mix=value; flanger.setMix(mix); break; }
}

// ===== Phaser =====
Phaser::Phaser() = default;
void Phaser::prepare(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec spec{sampleRate, (juce::uint32)samplesPerBlock, 2};
    phaser.prepare(spec);
    phaser.setRate(rate);
    phaser.setDepth(depth);
    phaser.setMix(mix);
}
void Phaser::process(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    phaser.process(juce::dsp::ProcessContextReplacing<float>(block));
}
void Phaser::reset() { phaser.reset(); }
juce::StringArray Phaser::getParameterNames() const { return {"Rate","Depth","Mix"}; }
float Phaser::getParameter(int index) const {
    switch(index){ case 0: return rate; case 1: return depth; case 2: return mix; default: return 0.0f; }
}
void Phaser::setParameter(int index, float value) {
    switch(index){ case 0: rate=value; phaser.setRate(rate); break; case 1: depth=value; phaser.setDepth(depth); break; case 2: mix=value; phaser.setMix(mix); break; }
}

// ===== Tremolo =====
Tremolo::Tremolo() = default;
void Tremolo::prepare(double sampleRate, int) { this->sampleRate = sampleRate; phase = 0.0f; }
void Tremolo::process(juce::AudioBuffer<float>& buffer) {
    for(int ch=0; ch<buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for(int i=0; i<buffer.getNumSamples(); ++i) {
            phase += 2.0f * juce::MathConstants<float>::pi * rate / (float)sampleRate;
            if(phase >= 2.0f * juce::MathConstants<float>::pi) phase -= 2.0f * juce::MathConstants<float>::pi;
            float gain = 1.0f - depth * (0.5f + 0.5f * std::sin(phase));
            data[i] *= gain;
        }
    }
}
void Tremolo::reset() { phase = 0.0f; }
juce::StringArray Tremolo::getParameterNames() const { return {"Rate","Depth"}; }
float Tremolo::getParameter(int index) const { return index==0 ? rate : depth; }
void Tremolo::setParameter(int index, float value) { if(index==0) rate=value; else if(index==1) depth=value; }

// ===== Vibrato =====
Vibrato::Vibrato() = default;
void Vibrato::prepare(double sampleRate, int) { this->sampleRate = sampleRate; phase = 0.0f; }
void Vibrato::process(juce::AudioBuffer<float>& buffer) {
    for(int ch=0; ch<buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for(int i=0; i<buffer.getNumSamples(); ++i) {
            phase += 2.0f * juce::MathConstants<float>::pi * rate / (float)sampleRate;
            if(phase >= 2.0f * juce::MathConstants<float>::pi) phase -= 2.0f * juce::MathConstants<float>::pi;
            data[i] *= (1.0f + depth * 0.1f * std::sin(phase));
        }
    }
}
void Vibrato::reset() { phase = 0.0f; }
juce::StringArray Vibrato::getParameterNames() const { return {"Rate","Depth"}; }
float Vibrato::getParameter(int index) const { return index==0 ? rate : depth; }
void Vibrato::setParameter(int index, float value) { if(index==0) rate=value; else if(index==1) depth=value; }
