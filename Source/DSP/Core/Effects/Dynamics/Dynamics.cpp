#include "Dynamics.h"

// ===== Compressor =====
Compressor::Compressor() = default;

void Compressor::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    compressor.prepare (spec);
    compressor.setThreshold (threshold);
    compressor.setRatio (ratio);
    compressor.setAttack (attack);
    compressor.setRelease (release);
    compressor.setMakeupGain (makeup);
}

void Compressor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    compressor.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void Compressor::reset() { compressor.reset(); }

juce::StringArray Compressor::getParameterNames() const
{
    return { "Threshold", "Ratio", "Attack", "Release", "Makeup" };
}

float Compressor::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return ratio;
        case 2: return attack;
        case 3: return release;
        case 4: return makeup;
        default: return 0.0f;
    }
}

void Compressor::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; compressor.setThreshold (threshold); break;
        case 1: ratio = value; compressor.setRatio (ratio); break;
        case 2: attack = value; compressor.setAttack (attack); break;
        case 3: release = value; compressor.setRelease (release); break;
        case 4: makeup = value; compressor.setMakeupGain (makeup); break;
    }
}

// ===== Limiter =====
Limiter::Limiter() = default;

void Limiter::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    limiter.prepare (spec);
    limiter.setThreshold (threshold);
    limiter.setRelease (release);
    limiter.setCeiling (ceiling);
}

void Limiter::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    limiter.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void Limiter::reset() { limiter.reset(); }

juce::StringArray Limiter::getParameterNames() const
{
    return { "Threshold", "Release", "Ceiling" };
}

float Limiter::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return release;
        case 2: return ceiling;
        default: return 0.0f;
    }
}

void Limiter::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; limiter.setThreshold (threshold); break;
        case 1: release = value; limiter.setRelease (release); break;
        case 2: ceiling = value; limiter.setCeiling (ceiling); break;
    }
}

// ===== Expander =====
Expander::Expander() = default;

void Expander::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    expander.prepare (spec);
    expander.setThreshold (threshold);
    expander.setRatio (ratio);
    expander.setAttack (attack);
    expander.setRelease (release);
}

void Expander::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    expander.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void Expander::reset() { expander.reset(); }

juce::StringArray Expander::getParameterNames() const
{
    return { "Threshold", "Ratio", "Attack", "Release" };
}

float Expander::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return ratio;
        case 2: return attack;
        case 3: return release;
        default: return 0.0f;
    }
}

void Expander::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; expander.setThreshold (threshold); break;
        case 1: ratio = value; expander.setRatio (ratio); break;
        case 2: attack = value; expander.setAttack (attack); break;
        case 3: release = value; expander.setRelease (release); break;
    }
}

// ===== Gate =====
Gate::Gate() = default;

void Gate::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    gate.prepare (spec);
    gate.setThreshold (threshold);
    gate.setAttack (attack);
    gate.setRelease (release);
    gate.setHold (hold);
}

void Gate::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    gate.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void Gate::reset() { gate.reset(); }

juce::StringArray Gate::getParameterNames() const
{
    return { "Threshold", "Attack", "Release", "Hold" };
}

float Gate::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return attack;
        case 2: return release;
        case 3: return hold;
        default: return 0.0f;
    }
}

void Gate::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; gate.setThreshold (threshold); break;
        case 1: attack = value; gate.setAttack (attack); break;
        case 2: release = value; gate.setRelease (release); break;
        case 3: hold = value; gate.setHold (hold); break;
    }
}

// ===== DeEsser =====
DeEsser::DeEsser() = default;

void DeEsser::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    deesser.prepare (spec);
    deesser.setFrequency (frequency);
    deesser.setThreshold (threshold);
    deesser.setBandwidth (width);
}

void DeEsser::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    deesser.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void DeEsser::reset() { deesser.reset(); }

juce::StringArray DeEsser::getParameterNames() const
{
    return { "Frequency", "Threshold", "Width" };
}

float DeEsser::getParameter (int index) const
{
    switch (index) {
        case 0: return frequency;
        case 1: return threshold;
        case 2: return width;
        default: return 0.0f;
    }
}

void DeEsser::setParameter (int index, float value)
{
    switch (index) {
        case 0: frequency = value; deesser.setFrequency (frequency); break;
        case 1: threshold = value; deesser.setThreshold (threshold); break;
        case 2: width = value; deesser.setBandwidth (width); break;
    }
}

// ===== TransientShaper =====
TransientShaper::TransientShaper() = default;

void TransientShaper::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
}

void TransientShaper::process (juce::AudioBuffer<float>& buffer)
{
    // Simple transient shaper: attack boost, sustain cut
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* data = buffer.getWritePointer (channel);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float env = std::abs (data[i]);
            // naive envelope follower
            static float prevEnv = 0.0f;
            env = 0.9f * prevEnv + 0.1f * env;
            prevEnv = env;
            float gain = 1.0f + attack * env - sustain * (1.0f - env);
            data[i] *= gain;
        }
    }
}

void TransientShaper::reset() {}

juce::StringArray TransientShaper::getParameterNames() const
{
    return { "Attack", "Sustain", "Output" };
}

float TransientShaper::getParameter (int index) const
{
    switch (index) {
        case 0: return attack;
        case 1: return sustain;
        case 2: return output;
        default: return 0.0f;
    }
}

void TransientShaper::setParameter (int index, float value)
{
    switch (index) {
        case 0: attack = value; break;
        case 1: sustain = value; break;
        case 2: output = value; break;
    }
}
