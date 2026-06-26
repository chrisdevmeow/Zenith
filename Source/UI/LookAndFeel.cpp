#include "LookAndFeel.h"

ZenithLookAndFeel::ZenithLookAndFeel()
{
    setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::cyan.darker (0.3f));
    setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
}

ZenithLookAndFeel::~ZenithLookAndFeel() = default;

void ZenithLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                          juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (6.0f);
    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    auto centre = bounds.getCentre();

    // Background
    g.setColour (juce::Colours::darkgrey.brighter (0.1f));
    g.fillEllipse (centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f);

    // Outline
    g.setColour (juce::Colours::black.brighter (0.2f));
    g.drawEllipse (centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f, 1.5f);

    // Arc
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto arcRadius = radius * 0.7f;

    juce::Path arcPath;
    arcPath.addCentredArc (centre.x, centre.y, arcRadius, arcRadius,
                           0.0f, rotaryStartAngle, angle, true);

    g.setColour (getKnobColour (slider));
    g.strokePath (arcPath, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved));

    // Center dot
    g.setColour (juce::Colours::white.withAlpha (0.6f));
    g.fillEllipse (centre.x - 4.0f, centre.y - 4.0f, 8.0f, 8.0f);

    // Value text
    auto text = juce::String (slider.getValue(), 2);
    g.setColour (juce::Colours::white.withAlpha (0.7f));
    g.setFont (12.0f);
    g.drawText (text, x, y + height - 20, width, 18, juce::Justification::centred);
}

juce::Colour ZenithLookAndFeel::getKnobColour (const juce::Slider& slider)
{
    auto colour = slider.findColour (juce::Slider::rotarySliderFillColourId);
    if (slider.isEnabled())
        return colour;
    return colour.withAlpha (0.4f);
}
