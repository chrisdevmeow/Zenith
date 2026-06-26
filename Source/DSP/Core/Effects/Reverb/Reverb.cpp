#include "Reverb.h"

// ===== HallReverb =====
HallReverb::HallReverb() = default;

void HallReverb::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    reverb.prepare (spec);
    reverb.setParameters ({ roomSize, damping, wetLevel, 0.0f, 0.5f, 0.0f });
}

void HallReverb::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    reverb.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void HallReverb::reset() { reverb.reset(); }

juce::StringArray HallReverb::getParameterNames() const
{
    return { "Room Size", "Damping", "Wet Level" };
}

float HallReverb::getParameter (int index) const
{
    switch (index) {
        case 0: return roomSize;
        case 1: return damping;
        case 2: return wetLevel;
        default: return 0.0f;
    }
}

void HallReverb::setParameter (int index, float value)
{
    switch (index) {
        case 0: roomSize = value; break;
        case 1: damping = value; break;
        case 2: wetLevel = value; break;
    }
    reverb.setParameters ({ roomSize, damping, wetLevel, 0.0f, 0.5f, 0.0f });
}

// ===== RoomReverb =====
RoomReverb::RoomReverb() = default;

void RoomReverb::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    reverb.prepare (spec);
    reverb.setParameters ({ roomSize, damping, wetLevel, 0.0f, 0.3f, 0.0f });
}

void RoomReverb::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    reverb.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void RoomReverb::reset() { reverb.reset(); }

juce::StringArray RoomReverb::getParameterNames() const
{
    return { "Room Size", "Damping", "Wet Level" };
}

float RoomReverb::getParameter (int index) const
{
    switch (index) {
        case 0: return roomSize;
        case 1: return damping;
        case 2: return wetLevel;
        default: return 0.0f;
    }
}

void RoomReverb::setParameter (int index, float value)
{
    switch (index) {
        case 0: roomSize = value; break;
        case 1: damping = value; break;
        case 2: wetLevel = value; break;
    }
    reverb.setParameters ({ roomSize, damping, wetLevel, 0.0f, 0.3f, 0.0f });
}

// ===== PlateReverb =====
PlateReverb::PlateReverb() = default;

void PlateReverb::prepare (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    reverb.prepare (spec);
    reverb.setParameters ({ roomSize, damping, wetLevel, 0.0f, 0.0f, 0.0f });
}

void PlateReverb::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    reverb.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void PlateReverb::reset() { reverb.reset(); }

juce::StringArray PlateReverb::getParameterNames() const
{
    return { "Room Size", "Damping", "Wet Level" };
}

float PlateReverb::getParameter (int index) const
{
    switch (index) {
        case 0: return roomSize;
        case 1: return damping;
        case 2: return wetLevel;
        default: return 0.0f;
    }
}

void PlateReverb::setParameter (int index, float value)
{
    switch (index) {
        case 0: roomSize = value; break;
        case 1: damping = value; break;
        case 2: wetLevel = value; break;
    }
    reverb.setParameters ({ roomSize, damping, wetLevel, 0.0f, 0.0f, 0.0f });
}
