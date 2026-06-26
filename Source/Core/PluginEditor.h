#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ZenithAudioEditor : public juce::AudioProcessorEditor,
                          private juce::Timer
{
public:
    explicit ZenithAudioEditor (ZenithAudioProcessor&);
    ~ZenithAudioEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ZenithAudioProcessor& audioProcessor;
    ZenithLookAndFeel lookAndFeel;

    // ========== SYNTH CONTROLS ==========
    juce::ComboBox waveSelector;
    juce::Slider freqSlider, fineSlider;
    juce::Slider cutoffSlider, resonanceSlider, filterEnvSlider;
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;

    // ========== MODULATION ==========
    juce::Slider lfoRateSlider, lfoDepthSlider;
    juce::ComboBox lfoTargetSelector;

    // ========== EFFECTS ==========
    juce::Slider chorusSlider, delaySlider, reverbSlider;

    // ========== MASTER ==========
    juce::Slider masterSlider;

    // ========== CHORD ==========
    juce::ComboBox chordRootSelector, chordTypeSelector;
    juce::TextButton chordPlayButton{ "▶ Play" };

    // ========== PRESETS ==========
    juce::TextButton presetSaveButton{ "Save" };
    juce::TextButton presetLoadButton{ "Load" };

    // ========== ATTACHMENTS ==========
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fineAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterEnvAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfoTargetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> chordRootAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> chordTypeAttachment;

    // ========== HELPERS ==========
    void addSlider (juce::Slider& slider,
                    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
                    const juce::String& paramID,
                    const juce::String& labelText);
    void addComboBox (juce::ComboBox& box,
                      std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& attachment,
                      const juce::String& paramID,
                      const juce::StringArray& items);

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZenithAudioEditor)
};
