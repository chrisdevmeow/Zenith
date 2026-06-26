#pragma once
#include <JuceHeader.h>

class Knob : public juce::Slider
{
public:
    Knob();
    ~Knob() override;

    void setColour (juce::Colour colour);
    void setValueText (const juce::String& text);

private:
    juce::Colour fillColour = juce::Colours::cyan.darker (0.3f);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Knob)
};
