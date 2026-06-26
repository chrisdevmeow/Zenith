#include "ReverbExtra.h"

// ===== SpringReverb =====
SpringReverb::SpringReverb() = default;

void SpringReverb::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    reverb.prepare (spec);
    reverb.setParameters ({ roomSize, damping, wetLevel, 0.0f, 0.8f, 0.0f });
}

void SpringReverb::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    reverb.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void SpringReverb::reset() { reverb.reset(); }

juce::StringArray SpringReverb::getParameterNames() const
{
    return { "Room Size", "Damping", "Wet Level" };
}

float SpringReverb::getParameter (int index) const
{
    switch (index) {
        case 0: return roomSize;
        case 1: return damping;
        case 2: return wetLevel;
        default: return 0.0f;
    }
}

void SpringReverb::setParameter (int index, float value)
{
    switch (index) {
        case 0: roomSize = value; break;
        case 1: damping = value; break;
        case 2: wetLevel = value; break;
    }
    reverb.setParameters ({ roomSize, damping, wetLevel, 0.0f, 0.8f, 0.0f });
}

// ===== ConvolutionReverb =====
ConvolutionReverb::ConvolutionReverb() = default;

void ConvolutionReverb::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    convolution.prepare (spec);
    // Load a default impulse (would be replaced by user IR)
    juce::AudioBuffer<float> impulse (1, (int)sampleRate);
    impulse.clear();
    impulse.setSample (0, 0, 1.0f);
    convolution.loadImpulse (impulse);
}

void ConvolutionReverb::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    convolution.process (juce::dsp::ProcessContextReplacing<float> (block));
    // Apply wet/dry mix manually since convolution doesn't have built-in mix
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* data = buffer.getWritePointer (channel);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            data[i] = data[i] * wetLevel + data[i] * dryLevel;
    }
}

void ConvolutionReverb::reset() { convolution.reset(); }

juce::StringArray ConvolutionReverb::getParameterNames() const
{
    return { "Wet Level", "Dry Level" };
}

float ConvolutionReverb::getParameter (int index) const
{
    return (index == 0) ? wetLevel : dryLevel;
}

void ConvolutionReverb::setParameter (int index, float value)
{
    if (index == 0) wetLevel = value;
    else if (index == 1) dryLevel = value;
}

// ===== CathedralReverb =====
CathedralReverb::CathedralReverb() = default;

void CathedralReverb::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    reverb.prepare (spec);
    reverb.setParameters ({ roomSize, damping, wetLevel, 0.0f, 0.9f, 0.0f });
}

void CathedralReverb::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    reverb.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void CathedralReverb::reset() { reverb.reset(); }

juce::StringArray CathedralReverb::getParameterNames() const
{
    return { "Room Size", "Damping", "Wet Level" };
}

float CathedralReverb::getParameter (int index) const
{
    switch (index) {
        case 0: return roomSize;
        case 1: return damping;
        case 2: return wetLevel;
        default: return 0.0f;
    }
}

void CathedralReverb::setParameter (int index, float value)
{
    switch (index) {
        case 0: roomSize = value; break;
        case 1: damping = value; break;
        case 2: wetLevel = value; break;
    }
    reverb.setParameters ({ roomSize, damping, wetLevel, 0.0f, 0.9f, 0.0f });
}
