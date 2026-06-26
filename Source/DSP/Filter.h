#pragma once
#include <JuceHeader.h>

class Filter
{
public:
    Filter();
    ~Filter();

    void setCutoff (float cutoff);
    void setResonance (float resonance);
    void setSampleRate (double rate);

    float processSample (float input);
    void reset();

private:
    float cutoff = 8000.0f;
    float resonance = 0.5f;
    float sampleRate = 44100.0f;

    // Biquad coefficients
    float a0 = 1.0f, a1 = 0.0f, a2 = 0.0f;
    float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f;

    // Delay lines
    float x1 = 0.0f, x2 = 0.0f;
    float y1 = 0.0f, y2 = 0.0f;

    void updateCoefficients();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};
