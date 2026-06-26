#pragma once
#include <JuceHeader.h>
#include "../DSP/Core/EffectRack.h"
#include "../DSP/Core/EffectFactory.h"

class EffectPanel : public juce::Component,
                    public juce::ComboBox::Listener,
                    public juce::Button::Listener
{
public:
    EffectPanel (EffectRack& rack);
    ~EffectPanel() override;

    void resized() override;
    void paint (juce::Graphics& g) override;

    // ComboBox::Listener
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;

    // Button::Listener
    void buttonClicked (juce::Button* button) override;

    void refreshEffectList();
    void refreshParameters();

private:
    EffectRack& effectRack;

    juce::ComboBox effectSelector;
    juce::TextButton addButton{ "Add" };
    juce::TextButton removeButton{ "Remove" };
    juce::TextButton moveUpButton{ "↑" };
    juce::TextButton moveDownButton{ "↓" };
    juce::ComboBox effectTypeSelector;

    juce::OwnedArray<juce::Slider> paramSliders;
    juce::OwnedArray<juce::Label> paramLabels;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> attachments;

    void updateSlidersForCurrentEffect();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectPanel)
};
