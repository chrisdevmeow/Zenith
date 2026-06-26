#include "EQ.h"

// ===== ParametricEQ =====
ParametricEQ::ParametricEQ() = default;

void ParametricEQ::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    filterChain.prepare (spec);
    
    for (int i = 0; i < 4; ++i)
    {
        auto* filter = filterChain.template get<0> (i);
        *filter = juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, freq[i], q[i], juce::Decibels::decibelsToGain (gain[i]));
    }
}

void ParametricEQ::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    filterChain.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void ParametricEQ::reset() { filterChain.reset(); }

juce::StringArray ParametricEQ::getParameterNames() const
{
    return { "Freq1", "Gain1", "Q1", "Freq2", "Gain2", "Q2", "Freq3", "Gain3", "Q3", "Freq4", "Gain4", "Q4" };
}

float ParametricEQ::getParameter (int index) const
{
    int band = index / 3;
    int param = index % 3;
    if (band >= 4) return 0.0f;
    switch (param) {
        case 0: return freq[band];
        case 1: return gain[band];
        case 2: return q[band];
        default: return 0.0f;
    }
}

void ParametricEQ::setParameter (int index, float value)
{
    int band = index / 3;
    int param = index % 3;
    if (band >= 4) return;
    switch (param) {
        case 0: freq[band] = value; break;
        case 1: gain[band] = value; break;
        case 2: q[band] = value; break;
    }
    auto* filter = filterChain.template get<0> (band);
    *filter = juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, freq[band], q[band], juce::Decibels::decibelsToGain (gain[band]));
}

// ===== GraphicEQ =====
GraphicEQ::GraphicEQ() = default;

void GraphicEQ::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    filterChain.prepare (spec);
    
    const float freqs[10] = { 31.0f, 63.0f, 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f };
    for (int i = 0; i < 10; ++i)
    {
        auto* filter = filterChain.template get<0> (i);
        *filter = juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, freqs[i], 1.0f, juce::Decibels::decibelsToGain (gains[i]));
    }
}

void GraphicEQ::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    filterChain.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void GraphicEQ::reset() { filterChain.reset(); }

juce::StringArray GraphicEQ::getParameterNames() const
{
    return { "31Hz", "63Hz", "125Hz", "250Hz", "500Hz", "1kHz", "2kHz", "4kHz", "8kHz", "16kHz" };
}

float GraphicEQ::getParameter (int index) const
{
    return (index >= 0 && index < 10) ? gains[index] : 0.0f;
}

void GraphicEQ::setParameter (int index, float value)
{
    if (index < 0 || index >= 10) return;
    gains[index] = value;
    const float freqs[10] = { 31.0f, 63.0f, 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f };
    auto* filter = filterChain.template get<0> (index);
    *filter = juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, freqs[index], 1.0f, juce::Decibels::decibelsToGain (value));
}

// ===== HighPassFilter =====
HighPassFilter::HighPassFilter() = default;

void HighPassFilter::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    filter.prepare (spec);
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, frequency, slope);
}

void HighPassFilter::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    filter.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void HighPassFilter::reset() { filter.reset(); }

juce::StringArray HighPassFilter::getParameterNames() const
{
    return { "Frequency", "Slope" };
}

float HighPassFilter::getParameter (int index) const
{
    return (index == 0) ? frequency : slope;
}

void HighPassFilter::setParameter (int index, float value)
{
    if (index == 0) { frequency = value; }
    else if (index == 1) { slope = value; }
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, frequency, slope);
}

// ===== LowPassFilter =====
LowPassFilter::LowPassFilter() = default;

void LowPassFilter::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    filter.prepare (spec);
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, frequency, slope);
}

void LowPassFilter::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    filter.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void LowPassFilter::reset() { filter.reset(); }

juce::StringArray LowPassFilter::getParameterNames() const
{
    return { "Frequency", "Slope" };
}

float LowPassFilter::getParameter (int index) const
{
    return (index == 0) ? frequency : slope;
}

void LowPassFilter::setParameter (int index, float value)
{
    if (index == 0) { frequency = value; }
    else if (index == 1) { slope = value; }
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, frequency, slope);
}
