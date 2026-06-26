#pragma once
#include <JuceHeader.h>

namespace MathHelpers
{
    inline float mapRange (float value, float inMin, float inMax, float outMin, float outMax)
    {
        return outMin + (outMax - outMin) * ((value - inMin) / (inMax - inMin));
    }

    inline float dbToGain (float db)
    {
        return std::pow (10.0f, db / 20.0f);
    }

    inline float gainToDb (float gain)
    {
        return 20.0f * std::log10 (gain);
    }

    inline float midiNoteToFrequency (int note, float tuning = 440.0f)
    {
        return tuning * std::pow (2.0f, (note - 69.0f) / 12.0f);
    }

    inline int frequencyToMidiNote (float freq, float tuning = 440.0f)
    {
        return (int) std::round (69.0f + 12.0f * std::log2 (freq / tuning));
    }

    inline float panToBalance (float pan) // -1..1 -> 0..1
    {
        return juce::jlimit (0.0f, 1.0f, (pan + 1.0f) * 0.5f);
    }

    inline float lerp (float a, float b, float t)
    {
        return a + (b - a) * t;
    }
}
