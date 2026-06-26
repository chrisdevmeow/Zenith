#pragma once
#include <JuceHeader.h>
#include <memory>
#include "Effect.h"

class EffectFactory
{
public:
    static std::unique_ptr<Effect> create (const juce::String& name);
    static juce::StringArray getAvailableEffects();
};
