#pragma once
#include <JuceHeader.h>

class Effect
{
public:
    virtual ~Effect() = default;
    virtual void prepare (double sampleRate, int samplesPerBlock) = 0;
    virtual void process (juce::AudioBuffer<float>& buffer) = 0;
    virtual void reset() = 0;
    virtual juce::String getName() const = 0;
    virtual juce::StringArray getParameterNames() const = 0;
    virtual float getParameter (int index) const = 0;
    virtual void setParameter (int index, float value) = 0;
    virtual int getNumParameters() const = 0;
};
