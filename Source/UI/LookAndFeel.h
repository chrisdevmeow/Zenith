#pragma once
#include <JuceHeader.h>

class ZenithLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ZenithLookAndFeel();
    ~ZenithLookAndFeel() override;

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider& slider) override;

private:
    juce::Colour getKnobColour (const juce::Slider& slider);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZenithLookAndFeel)
};
