#pragma once
#include <JuceHeader.h>
#include "Effect.h"
#include <vector>
#include <memory>

class EffectRack
{
public:
    EffectRack() = default;
    ~EffectRack() = default;

    void addEffect (std::unique_ptr<Effect> effect);
    void removeEffect (int index);
    void clear();
    int size() const;
    void prepare (double sampleRate, int samplesPerBlock);
    void process (juce::AudioBuffer<float>& buffer);
    void reset();

    Effect* getEffect (int index);
    juce::StringArray getEffectNames() const;

    juce::var toVar() const;
    void fromVar (const juce::var& data);

private:
    std::vector<std::unique_ptr<Effect>> effects;
};
