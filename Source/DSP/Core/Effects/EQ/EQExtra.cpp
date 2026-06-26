#include "EQExtra.h"

// ===== BandPassFilter =====
BandPassFilter::BandPassFilter() = default;

void BandPassFilter::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    filter.prepare (spec);
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeBandPass (sampleRate, frequency, q);
}

void BandPassFilter::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    filter.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void BandPassFilter::reset() { filter.reset(); }

juce::StringArray BandPassFilter::getParameterNames() const
{
    return { "Frequency", "Q", "Gain" };
}

float BandPassFilter::getParameter (int index) const
{
    switch (index) {
        case 0: return frequency;
        case 1: return q;
        case 2: return gain;
        default: return 0.0f;
    }
}

void BandPassFilter::setParameter (int index, float value)
{
    switch (index) {
        case 0: frequency = value; break;
        case 1: q = value; break;
        case 2: gain = value; break;
    }
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeBandPass (sampleRate, frequency, q);
}

// ===== BandStopFilter =====
BandStopFilter::BandStopFilter() = default;

void BandStopFilter::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    filter.prepare (spec);
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeNotch (sampleRate, frequency, q);
}

void BandStopFilter::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    filter.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void BandStopFilter::reset() { filter.reset(); }

juce::StringArray BandStopFilter::getParameterNames() const
{
    return { "Frequency", "Q", "Gain" };
}

float BandStopFilter::getParameter (int index) const
{
    switch (index) {
        case 0: return frequency;
        case 1: return q;
        case 2: return gain;
        default: return 0.0f;
    }
}

void BandStopFilter::setParameter (int index, float value)
{
    switch (index) {
        case 0: frequency = value; break;
        case 1: q = value; break;
        case 2: gain = value; break;
    }
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeNotch (sampleRate, frequency, q);
}

// ===== DynamicEQ =====
DynamicEQ::DynamicEQ() = default;

void DynamicEQ::prepare (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    chain.prepare (spec);
    
    auto* filter = chain.template get<0>();
    *filter = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, frequency, q, 1.0f);
    
    auto* comp = chain.template get<1>();
    comp->setThreshold (threshold);
    comp->setRatio (ratio);
    comp->setAttack (5.0f);
    comp->setRelease (100.0f);
    comp->setMakeupGain (makeup);
}

void DynamicEQ::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    chain.process (juce::dsp::ProcessContextReplacing<float> (block));
}

void DynamicEQ::reset() { chain.reset(); }

juce::StringArray DynamicEQ::getParameterNames() const
{
    return { "Frequency", "Q", "Threshold", "Ratio", "Makeup" };
}

float DynamicEQ::getParameter (int index) const
{
    switch (index) {
        case 0: return frequency;
        case 1: return q;
        case 2: return threshold;
        case 3: return ratio;
        case 4: return makeup;
        default: return 0.0f;
    }
}

void DynamicEQ::setParameter (int index, float value)
{
    switch (index) {
        case 0: frequency = value; break;
        case 1: q = value; break;
        case 2: threshold = value; break;
        case 3: ratio = value; break;
        case 4: makeup = value; break;
    }
    auto* filter = chain.template get<0>();
    *filter = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, frequency, q, 1.0f);
    
    auto* comp = chain.template get<1>();
    comp->setThreshold (threshold);
    comp->setRatio (ratio);
    comp->setMakeupGain (makeup);
}
