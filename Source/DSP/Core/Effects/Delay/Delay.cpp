#include "Delay.h"

// ===== DigitalDelay =====
DigitalDelay::DigitalDelay() = default;

void DigitalDelay::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    delayLine.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    delayLine.setDelay (juce::dsp::seconds (delayTimeMs / 1000.0));
    delayInSamples = (int)(delayTimeMs * sampleRate / 1000.0);
    
    feedbackGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    feedbackGain.setGain (feedback);
    dryGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    dryGain.setGain (1.0f - mix);
    wetGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    wetGain.setGain (mix);
}

void DigitalDelay::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* data = block.getChannelPointer (channel);
        for (int i = 0; i < block.getNumSamples(); ++i)
        {
            float input = data[i];
            float delayed = delayLine.popSample (channel);
            delayLine.pushSample (channel, input + feedback * delayed);
            data[i] = dryGain.processSample (channel, input) + wetGain.processSample (channel, delayed);
        }
    }
}

void DigitalDelay::reset()
{
    delayLine.reset();
    feedbackGain.reset();
    dryGain.reset();
    wetGain.reset();
}

juce::StringArray DigitalDelay::getParameterNames() const
{
    return { "Time (ms)", "Feedback", "Mix" };
}

float DigitalDelay::getParameter (int index) const
{
    switch (index) {
        case 0: return delayTimeMs;
        case 1: return feedback;
        case 2: return mix;
        default: return 0.0f;
    }
}

void DigitalDelay::setParameter (int index, float value)
{
    switch (index) {
        case 0: delayTimeMs = value; delayLine.setDelay (juce::dsp::seconds (delayTimeMs / 1000.0)); break;
        case 1: feedback = value; feedbackGain.setGain (feedback); break;
        case 2: mix = value; dryGain.setGain (1.0f - mix); wetGain.setGain (mix); break;
    }
}

// ===== AnalogDelay =====
AnalogDelay::AnalogDelay() = default;

void AnalogDelay::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    delayLine.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    delayLine.setDelay (juce::dsp::seconds (delayTimeMs / 1000.0));
    delayInSamples = (int)(delayTimeMs * sampleRate / 1000.0);
    
    feedbackGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    feedbackGain.setGain (feedback);
    dryGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    dryGain.setGain (1.0f - mix);
    wetGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    wetGain.setGain (mix);
    
    lowpass.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    *lowpass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, cutoff);
}

void AnalogDelay::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* data = block.getChannelPointer (channel);
        for (int i = 0; i < block.getNumSamples(); ++i)
        {
            float input = data[i];
            float delayed = delayLine.popSample (channel);
            float filtered = lowpass.processSample (channel, delayed);
            delayLine.pushSample (channel, input + feedback * filtered);
            data[i] = dryGain.processSample (channel, input) + wetGain.processSample (channel, filtered);
        }
    }
}

void AnalogDelay::reset()
{
    delayLine.reset();
    feedbackGain.reset();
    dryGain.reset();
    wetGain.reset();
    lowpass.reset();
}

juce::StringArray AnalogDelay::getParameterNames() const
{
    return { "Time (ms)", "Feedback", "Mix", "Cutoff" };
}

float AnalogDelay::getParameter (int index) const
{
    switch (index) {
        case 0: return delayTimeMs;
        case 1: return feedback;
        case 2: return mix;
        case 3: return cutoff;
        default: return 0.0f;
    }
}

void AnalogDelay::setParameter (int index, float value)
{
    switch (index) {
        case 0: delayTimeMs = value; delayLine.setDelay (juce::dsp::seconds (delayTimeMs / 1000.0)); break;
        case 1: feedback = value; feedbackGain.setGain (feedback); break;
        case 2: mix = value; dryGain.setGain (1.0f - mix); wetGain.setGain (mix); break;
        case 3: cutoff = value; *lowpass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, cutoff); break;
    }
}

// ===== TapeDelay =====
TapeDelay::TapeDelay() = default;

void TapeDelay::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    delayLine.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    delayLine.setDelay (juce::dsp::seconds (delayTimeMs / 1000.0));
    delayInSamples = (int)(delayTimeMs * sampleRate / 1000.0);
    
    feedbackGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    feedbackGain.setGain (feedback);
    dryGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    dryGain.setGain (1.0f - mix);
    wetGain.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    wetGain.setGain (mix);
    
    lowpass.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    *lowpass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, 4000.0f);
    highpass.prepare ({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    *highpass.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, 80.0f);
}

void TapeDelay::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* data = block.getChannelPointer (channel);
        for (int i = 0; i < block.getNumSamples(); ++i)
        {
            float input = data[i];
            float delayed = delayLine.popSample (channel);
            float filtered = lowpass.processSample (channel, delayed);
            filtered = highpass.processSample (channel, filtered);
            // Simple wow/flutter modulation
            phase += 2.0f * juce::MathConstants<float>::pi * wow * 2.0f / (float)sampleRate;
            float mod = 1.0f + flutter * std::sin (phase);
            delayLine.setDelay (juce::dsp::seconds (delayTimeMs * mod / 1000.0));
            delayLine.pushSample (channel, input + feedback * filtered);
            data[i] = dryGain.processSample (channel, input) + wetGain.processSample (channel, filtered);
        }
    }
}

void TapeDelay::reset()
{
    delayLine.reset();
    feedbackGain.reset();
    dryGain.reset();
    wetGain.reset();
    lowpass.reset();
    highpass.reset();
    phase = 0.0f;
}

juce::StringArray TapeDelay::getParameterNames() const
{
    return { "Time (ms)", "Feedback", "Mix", "Wow", "Flutter" };
}

float TapeDelay::getParameter (int index) const
{
    switch (index) {
        case 0: return delayTimeMs;
        case 1: return feedback;
        case 2: return mix;
        case 3: return wow;
        case 4: return flutter;
        default: return 0.0f;
    }
}

void TapeDelay::setParameter (int index, float value)
{
    switch (index) {
        case 0: delayTimeMs = value; break;
        case 1: feedback = value; feedbackGain.setGain (feedback); break;
        case 2: mix = value; dryGain.setGain (1.0f - mix); wetGain.setGain (mix); break;
        case 3: wow = value; break;
        case 4: flutter = value; break;
    }
}
