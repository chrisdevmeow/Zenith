#include "Knob.h"

Knob::Knob()
{
    setSliderStyle (juce::Slider::RotaryVerticalDrag);
    setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    setColour (juce::Slider::rotarySliderFillColourId, fillColour);
    setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
}

Knob::~Knob() = default;

void Knob::setColour (juce::Colour colour)
{
    fillColour = colour;
    setColour (juce::Slider::rotarySliderFillColourId, fillColour);
}

void Knob::setValueText (const juce::String& text)
{
    setTextValueSuffix (" " + text);
}
