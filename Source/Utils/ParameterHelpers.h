#pragma once
#include <JuceHeader.h>

namespace ParameterHelpers
{
    inline void attachSlider (juce::AudioProcessorValueTreeState& apvts,
                              const juce::String& paramID,
                              juce::Slider& slider,
                              std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment)
    {
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, paramID, slider);
    }

    inline void attachComboBox (juce::AudioProcessorValueTreeState& apvts,
                                const juce::String& paramID,
                                juce::ComboBox& comboBox,
                                std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& attachment)
    {
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, paramID, comboBox);
    }

    inline void attachButton (juce::AudioProcessorValueTreeState& apvts,
                              const juce::String& paramID,
                              juce::Button& button,
                              std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>& attachment)
    {
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, paramID, button);
    }

    inline void addParameter (juce::AudioProcessorValueTreeState::ParameterLayout& layout,
                              std::unique_ptr<juce::RangedAudioParameter> param)
    {
        layout.add (std::move (param));
    }

    // Create a normalized range for frequency
    inline juce::NormalisableRange<float> freqRange (float min = 20.0f, float max = 20000.0f)
    {
        return juce::NormalisableRange<float> (min, max, 1.0f, 0.5f);
    }

    // Create a normalized range for time (seconds)
    inline juce::NormalisableRange<float> timeRange (float min = 0.001f, float max = 10.0f)
    {
        return juce::NormalisableRange<float> (min, max, 0.001f, 0.5f);
    }

    // Create a normalized range for gain
    inline juce::NormalisableRange<float> gainRange (float min = 0.0f, float max = 1.0f)
    {
        return juce::NormalisableRange<float> (min, max, 0.01f, 1.0f);
    }
}
