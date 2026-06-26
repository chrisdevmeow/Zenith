#include "DelayExtra.h"

// ===== PingPongDelay =====
PingPongDelay::PingPongDelay() = default;

void PingPongDelay::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    delayL.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 1 });
    delayR.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 1 });
    delayL.setDelay (juce::dsp::seconds (delayTimeMs / 1000.0));
    delayR.setDelay (juce::dsp::seconds (delayTimeMs / 1000.0));
    
    feedbackGainL.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 1 });
    feedbackGainL.setGain (feedback);
    feedbackGainR.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 1 });
    feedbackGainR.setGain (feedback);
    dryGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    dryGain.setGain (1.0f - mix);
    wetGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    wetGain.setGain (mix);
}

void PingPongDelay::process (juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* data = buffer.getWritePointer (channel);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float input = data[i];
            if (channel == 0)
            {
                float delayed = delayL.popSample (0);
                float feedbackSignal = input + crossFeed * delayed;
                delayL.pushSample (0, feedbackSignal * feedback);
                float output = dryGain.processSample (channel, input) + wetGain.processSample (channel, delayed);
                delayR.pushSample (0, feedbackSignal * feedback * crossFeed);
                data[i] = output;
            }
            else
            {
                float delayed = delayR.popSample (0);
                float feedbackSignal = input + crossFeed * delayed;
                delayR.pushSample (0, feedbackSignal * feedback);
                float output = dryGain.processSample (channel, input) + wetGain.processSample (channel, delayed);
                delayL.pushSample (0, feedbackSignal * feedback * crossFeed);
                data[i] = output;
            }
        }
    }
}

void PingPongDelay::reset()
{
    delayL.reset();
    delayR.reset();
    feedbackGainL.reset();
    feedbackGainR.reset();
    dryGain.reset();
    wetGain.reset();
}

juce::StringArray PingPongDelay::getParameterNames() const
{
    return { "Time (ms)", "Feedback", "Mix", "CrossFeed" };
}

float PingPongDelay::getParameter (int index) const
{
    switch (index) {
        case 0: return delayTimeMs;
        case 1: return feedback;
        case 2: return mix;
        case 3: return crossFeed;
        default: return 0.0f;
    }
}

void PingPongDelay::setParameter (int index, float value)
{
    switch (index) {
        case 0: delayTimeMs = value; delayL.setDelay (juce::dsp::seconds (delayTimeMs / 1000.0)); delayR.setDelay (juce::dsp::seconds (delayTimeMs / 1000.0)); break;
        case 1: feedback = value; feedbackGainL.setGain (feedback); feedbackGainR.setGain (feedback); break;
        case 2: mix = value; dryGain.setGain (1.0f - mix); wetGain.setGain (mix); break;
        case 3: crossFeed = value; break;
    }
}

// ===== ModulatedDelay =====
ModulatedDelay::ModulatedDelay() = default;

void ModulatedDelay::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    delayLine.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    delayLine.setDelay (juce::dsp::seconds (delayTimeMs / 1000.0));
    
    feedbackGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    feedbackGain.setGain (feedback);
    dryGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    dryGain.setGain (1.0f - mix);
    wetGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    wetGain.setGain (mix);
    phase = 0.0f;
}

void ModulatedDelay::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* data = block.getChannelPointer (channel);
        for (int i = 0; i < block.getNumSamples(); ++i)
        {
            float input = data[i];
            phase += 2.0f * juce::MathConstants<float>::pi * modRate / (float)sampleRate;
            float mod = 1.0f + modDepth * std::sin (phase);
            delayLine.setDelay (juce::dsp::seconds (delayTimeMs * mod / 1000.0));
            float delayed = delayLine.popSample (channel);
            delayLine.pushSample (channel, input + feedback * delayed);
            data[i] = dryGain.processSample (channel, input) + wetGain.processSample (channel, delayed);
        }
    }
}

void ModulatedDelay::reset()
{
    delayLine.reset();
    feedbackGain.reset();
    dryGain.reset();
    wetGain.reset();
    phase = 0.0f;
}

juce::StringArray ModulatedDelay::getParameterNames() const
{
    return { "Time (ms)", "Feedback", "Mix", "Mod Rate", "Mod Depth" };
}

float ModulatedDelay::getParameter (int index) const
{
    switch (index) {
        case 0: return delayTimeMs;
        case 1: return feedback;
        case 2: return mix;
        case 3: return modRate;
        case 4: return modDepth;
        default: return 0.0f;
    }
}

void ModulatedDelay::setParameter (int index, float value)
{
    switch (index) {
        case 0: delayTimeMs = value; break;
        case 1: feedback = value; feedbackGain.setGain (feedback); break;
        case 2: mix = value; dryGain.setGain (1.0f - mix); wetGain.setGain (mix); break;
        case 3: modRate = value; break;
        case 4: modDepth = value; break;
    }
}
