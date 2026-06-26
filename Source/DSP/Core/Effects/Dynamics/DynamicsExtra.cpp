#include "DynamicsExtra.h"

// ===== MultibandCompressor =====
MultibandCompressor::MultibandCompressor() = default;

void MultibandCompressor::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    compressorChain.prepare (spec);
    // Set up each band's compressor
    for (int i = 0; i < 3; ++i)
    {
        auto* comp = compressorChain.template get<0> (i);
        comp->setThreshold (lowThresh);
        comp->setRatio (lowRatio);
        // In a real implementation, you'd set crossover frequencies here
    }
}

void MultibandCompressor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    compressorChain.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void MultibandCompressor::reset() { compressorChain.reset(); }

juce::StringArray MultibandCompressor::getParameterNames() const
{
    return { "LowThresh", "MidThresh", "HighThresh", "LowRatio", "MidRatio", "HighRatio" };
}

float MultibandCompressor::getParameter (int index) const
{
    switch (index) {
        case 0: return lowThresh;
        case 1: return midThresh;
        case 2: return highThresh;
        case 3: return lowRatio;
        case 4: return midRatio;
        case 5: return highRatio;
        default: return 0.0f;
    }
}

void MultibandCompressor::setParameter (int index, float value)
{
    switch (index) {
        case 0: lowThresh = value; break;
        case 1: midThresh = value; break;
        case 2: highThresh = value; break;
        case 3: lowRatio = value; break;
        case 4: midRatio = value; break;
        case 5: highRatio = value; break;
    }
}

// ===== MultibandExpander =====
MultibandExpander::MultibandExpander() = default;

void MultibandExpander::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    expanderChain.prepare (spec);
}

void MultibandExpander::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    expanderChain.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void MultibandExpander::reset() { expanderChain.reset(); }

juce::StringArray MultibandExpander::getParameterNames() const
{
    return { "LowThresh", "MidThresh", "HighThresh", "LowRatio", "MidRatio", "HighRatio" };
}

float MultibandExpander::getParameter (int index) const
{
    switch (index) {
        case 0: return lowThresh;
        case 1: return midThresh;
        case 2: return highThresh;
        case 3: return lowRatio;
        case 4: return midRatio;
        case 5: return highRatio;
        default: return 0.0f;
    }
}

void MultibandExpander::setParameter (int index, float value)
{
    switch (index) {
        case 0: lowThresh = value; break;
        case 1: midThresh = value; break;
        case 2: highThresh = value; break;
        case 3: lowRatio = value; break;
        case 4: midRatio = value; break;
        case 5: highRatio = value; break;
    }
}

// ===== PeakCompressor =====
PeakCompressor::PeakCompressor() = default;

void PeakCompressor::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    compressor.prepare (spec);
    compressor.setThreshold (threshold);
    compressor.setRatio (ratio);
    compressor.setAttack (attack);
    compressor.setRelease (release);
}

void PeakCompressor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    compressor.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void PeakCompressor::reset() { compressor.reset(); }

juce::StringArray PeakCompressor::getParameterNames() const
{
    return { "Threshold", "Ratio", "Attack", "Release" };
}

float PeakCompressor::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return ratio;
        case 2: return attack;
        case 3: return release;
        default: return 0.0f;
    }
}

void PeakCompressor::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; compressor.setThreshold (threshold); break;
        case 1: ratio = value; compressor.setRatio (ratio); break;
        case 2: attack = value; compressor.setAttack (attack); break;
        case 3: release = value; compressor.setRelease (release); break;
    }
}

// ===== RMSCompressor =====
RMSCompressor::RMSCompressor() = default;

void RMSCompressor::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    compressor.prepare (spec);
    compressor.setThreshold (threshold);
    compressor.setRatio (ratio);
    compressor.setAttack (attack);
    compressor.setRelease (release);
}

void RMSCompressor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    compressor.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void RMSCompressor::reset() { compressor.reset(); }

juce::StringArray RMSCompressor::getParameterNames() const
{
    return { "Threshold", "Ratio", "Attack", "Release" };
}

float RMSCompressor::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return ratio;
        case 2: return attack;
        case 3: return release;
        default: return 0.0f;
    }
}

void RMSCompressor::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; compressor.setThreshold (threshold); break;
        case 1: ratio = value; compressor.setRatio (ratio); break;
        case 2: attack = value; compressor.setAttack (attack); break;
        case 3: release = value; compressor.setRelease (release); break;
    }
}

// ===== OpticalCompressor =====
OpticalCompressor::OpticalCompressor() = default;

void OpticalCompressor::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    compressor.prepare (spec);
    compressor.setThreshold (threshold);
    compressor.setRatio (ratio);
    compressor.setAttack (attack);
    compressor.setRelease (release);
}

void OpticalCompressor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    compressor.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void OpticalCompressor::reset() { compressor.reset(); }

juce::StringArray OpticalCompressor::getParameterNames() const
{
    return { "Threshold", "Ratio", "Attack", "Release" };
}

float OpticalCompressor::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return ratio;
        case 2: return attack;
        case 3: return release;
        default: return 0.0f;
    }
}

void OpticalCompressor::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; compressor.setThreshold (threshold); break;
        case 1: ratio = value; compressor.setRatio (ratio); break;
        case 2: attack = value; compressor.setAttack (attack); break;
        case 3: release = value; compressor.setRelease (release); break;
    }
}

// ===== FETCompressor =====
FETCompressor::FETCompressor() = default;

void FETCompressor::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    compressor.prepare (spec);
    compressor.setThreshold (threshold);
    compressor.setRatio (ratio);
    compressor.setAttack (attack);
    compressor.setRelease (release);
}

void FETCompressor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    compressor.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void FETCompressor::reset() { compressor.reset(); }

juce::StringArray FETCompressor::getParameterNames() const
{
    return { "Threshold", "Ratio", "Attack", "Release" };
}

float FETCompressor::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return ratio;
        case 2: return attack;
        case 3: return release;
        default: return 0.0f;
    }
}

void FETCompressor::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; compressor.setThreshold (threshold); break;
        case 1: ratio = value; compressor.setRatio (ratio); break;
        case 2: attack = value; compressor.setAttack (attack); break;
        case 3: release = value; compressor.setRelease (release); break;
    }
}

// ===== VCACompressor =====
VCACompressor::VCACompressor() = default;

void VCACompressor::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    compressor.prepare (spec);
    compressor.setThreshold (threshold);
    compressor.setRatio (ratio);
    compressor.setAttack (attack);
    compressor.setRelease (release);
}

void VCACompressor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    compressor.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void VCACompressor::reset() { compressor.reset(); }

juce::StringArray VCACompressor::getParameterNames() const
{
    return { "Threshold", "Ratio", "Attack", "Release" };
}

float VCACompressor::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return ratio;
        case 2: return attack;
        case 3: return release;
        default: return 0.0f;
    }
}

void VCACompressor::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; compressor.setThreshold (threshold); break;
        case 1: ratio = value; compressor.setRatio (ratio); break;
        case 2: attack = value; compressor.setAttack (attack); break;
        case 3: release = value; compressor.setRelease (release); break;
    }
}

// ===== VariMuCompressor =====
VariMuCompressor::VariMuCompressor() = default;

void VariMuCompressor::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    compressor.prepare (spec);
    compressor.setThreshold (threshold);
    compressor.setRatio (ratio);
    compressor.setAttack (attack);
    compressor.setRelease (release);
}

void VariMuCompressor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    compressor.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void VariMuCompressor::reset() { compressor.reset(); }

juce::StringArray VariMuCompressor::getParameterNames() const
{
    return { "Threshold", "Ratio", "Attack", "Release" };
}

float VariMuCompressor::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return ratio;
        case 2: return attack;
        case 3: return release;
        default: return 0.0f;
    }
}

void VariMuCompressor::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; compressor.setThreshold (threshold); break;
        case 1: ratio = value; compressor.setRatio (ratio); break;
        case 2: attack = value; compressor.setAttack (attack); break;
        case 3: release = value; compressor.setRelease (release); break;
    }
}

// ===== DiodeCompressor =====
DiodeCompressor::DiodeCompressor() = default;

void DiodeCompressor::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    compressor.prepare (spec);
    compressor.setThreshold (threshold);
    compressor.setRatio (ratio);
    compressor.setAttack (attack);
    compressor.setRelease (release);
}

void DiodeCompressor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    compressor.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void DiodeCompressor::reset() { compressor.reset(); }

juce::StringArray DiodeCompressor::getParameterNames() const
{
    return { "Threshold", "Ratio", "Attack", "Release" };
}

float DiodeCompressor::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return ratio;
        case 2: return attack;
        case 3: return release;
        default: return 0.0f;
    }
}

void DiodeCompressor::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; compressor.setThreshold (threshold); break;
        case 1: ratio = value; compressor.setRatio (ratio); break;
        case 2: attack = value; compressor.setAttack (attack); break;
        case 3: release = value; compressor.setRelease (release); break;
    }
}

// ===== FeedbackCompressor =====
FeedbackCompressor::FeedbackCompressor() = default;

void FeedbackCompressor::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    compressor.prepare (spec);
    compressor.setThreshold (threshold);
    compressor.setRatio (ratio);
    compressor.setAttack (attack);
    compressor.setRelease (release);
}

void FeedbackCompressor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    compressor.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void FeedbackCompressor::reset() { compressor.reset(); }

juce::StringArray FeedbackCompressor::getParameterNames() const
{
    return { "Threshold", "Ratio", "Attack", "Release" };
}

float FeedbackCompressor::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return ratio;
        case 2: return attack;
        case 3: return release;
        default: return 0.0f;
    }
}

void FeedbackCompressor::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; compressor.setThreshold (threshold); break;
        case 1: ratio = value; compressor.setRatio (ratio); break;
        case 2: attack = value; compressor.setAttack (attack); break;
        case 3: release = value; compressor.setRelease (release); break;
    }
}

// ===== FeedForwardCompressor =====
FeedForwardCompressor::FeedForwardCompressor() = default;

void FeedForwardCompressor::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    compressor.prepare (spec);
    compressor.setThreshold (threshold);
    compressor.setRatio (ratio);
    compressor.setAttack (attack);
    compressor.setRelease (release);
}

void FeedForwardCompressor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    compressor.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void FeedForwardCompressor::reset() { compressor.reset(); }

juce::StringArray FeedForwardCompressor::getParameterNames() const
{
    return { "Threshold", "Ratio", "Attack", "Release" };
}

float FeedForwardCompressor::getParameter (int index) const
{
    switch (index) {
        case 0: return threshold;
        case 1: return ratio;
        case 2: return attack;
        case 3: return release;
        default: return 0.0f;
    }
}

void FeedForwardCompressor::setParameter (int index, float value)
{
    switch (index) {
        case 0: threshold = value; compressor.setThreshold (threshold); break;
        case 1: ratio = value; compressor.setRatio (ratio); break;
        case 2: attack = value; compressor.setAttack (attack); break;
        case 3: release = value; compressor.setRelease (release); break;
    }
}

// ===== EnvelopeFollower =====
EnvelopeFollower::EnvelopeFollower() = default;

void EnvelopeFollower::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    envelope = 0.0f;
}

void EnvelopeFollower::process (juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* data = buffer.getWritePointer (channel);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float input = std::abs (data[i]);
            float attackCoeff = std::exp (-1.0f / (attack * 0.001f * (float)sampleRate));
            float releaseCoeff = std::exp (-1.0f / (release * 0.001f * (float)sampleRate));
            if (input > envelope)
                envelope = attackCoeff * envelope + (1.0f - attackCoeff) * input;
            else
                envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * input;
            data[i] = envelope * output;
        }
    }
}

void EnvelopeFollower::reset() { envelope = 0.0f; }

juce::StringArray EnvelopeFollower::getParameterNames() const
{
    return { "Attack", "Release", "Output" };
}

float EnvelopeFollower::getParameter (int index) const
{
    switch (index) {
        case 0: return attack;
        case 1: return release;
        case 2: return output;
        default: return 0.0f;
    }
}

void EnvelopeFollower::setParameter (int index, float value)
{
    switch (index) {
        case 0: attack = value; break;
        case 1: release = value; break;
        case 2: output = value; break;
    }
}
