#pragma once
#include <JuceHeader.h>

class Oscillator
{
public:
    Oscillator();
    ~Oscillator();

    void setWaveform (int waveform); // 0=sine, 1=saw, 2=square, 3=triangle
    void setFrequency (float freq);
    void setFine (float fine);
    void setSampleRate (double rate);

    float getNextSample();
    void reset();

private:
    float phase = 0.0f;
    float freq = 440.0f;
    float fine = 0.0f;
    float sampleRate = 44100.0f;
    int waveform = 0; // sine default

    float saw (float phase);
    float square (float phase);
    float triangle (float phase);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
};
