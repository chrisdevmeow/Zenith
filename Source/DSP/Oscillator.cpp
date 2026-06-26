#include "Oscillator.h"
#include <cmath>

Oscillator::Oscillator() = default;
Oscillator::~Oscillator() = default;

void Oscillator::setWaveform (int wf)
{
    waveform = juce::jlimit (0, 3, wf);
}

void Oscillator::setFrequency (float newFreq)
{
    freq = newFreq;
}

void Oscillator::setFine (float newFine)
{
    fine = newFine;
}

void Oscillator::setSampleRate (double rate)
{
    sampleRate = rate;
}

void Oscillator::reset()
{
    phase = 0.0f;
}

float Oscillator::getNextSample()
{
    float increment = (freq + fine) / (float) sampleRate;
    phase += increment;
    if (phase >= 1.0f)
        phase -= 1.0f;

    switch (waveform)
    {
        case 0:  return std::sin (phase * 2.0f * juce::MathConstants<float>::pi);
        case 1:  return saw (phase);
        case 2:  return square (phase);
        case 3:  return triangle (phase);
        default: return 0.0f;
    }
}

float Oscillator::saw (float p)
{
    return 1.0f - 2.0f * p;
}

float Oscillator::square (float p)
{
    return p < 0.5f ? 1.0f : -1.0f;
}

float Oscillator::triangle (float p)
{
    return 1.0f - 4.0f * std::fabs (p - 0.5f);
}
