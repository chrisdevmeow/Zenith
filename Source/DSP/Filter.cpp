#include "Filter.h"
#include <cmath>

Filter::Filter() = default;
Filter::~Filter() = default;

void Filter::setCutoff (float newCutoff)
{
    cutoff = juce::jlimit (20.0f, 20000.0f, newCutoff);
    updateCoefficients();
}

void Filter::setResonance (float newRes)
{
    resonance = juce::jlimit (0.1f, 2.0f, newRes);
    updateCoefficients();
}

void Filter::setSampleRate (double rate)
{
    sampleRate = rate;
    updateCoefficients();
}

void Filter::updateCoefficients()
{
    if (sampleRate <= 0.0)
        return;

    float omega = 2.0f * juce::MathConstants<float>::pi * cutoff / sampleRate;
    float sn = std::sin (omega);
    float cs = std::cos (omega);
    float alpha = sn / (2.0f * resonance);

    // Lowpass biquad coefficients
    b0 = (1.0f - cs) / 2.0f;
    b1 = 1.0f - cs;
    b2 = (1.0f - cs) / 2.0f;
    a0 = 1.0f + alpha;
    a1 = -2.0f * cs;
    a2 = 1.0f - alpha;

    // Normalize
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;
}

float Filter::processSample (float input)
{
    float output = b0 * input + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

    x2 = x1;
    x1 = input;
    y2 = y1;
    y1 = output;

    return output;
}

void Filter::reset()
{
    x1 = x2 = y1 = y2 = 0.0f;
}
