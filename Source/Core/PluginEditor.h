#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../UI/EffectPanel.h"
#include "../UI/Knob.h"
#include "../UI/LookAndFeel.h"

class ZenithAudioEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit ZenithAudioEditor (ZenithAudioProcessor&);
    ~ZenithAudioEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ZenithAudioProcessor& audioProcessor;
    ZenithLookAndFeel lookAndFeel;

    juce::ComboBox waveSelector;
    Knob freqKnob, fineKnob, cutoffKnob, resonanceKnob, filterEnvKnob;
    Knob attackKnob, decayKnob, sustainKnob, releaseKnob;
    Knob lfoRateKnob, lfoDepthKnob;
    juce::ComboBox lfoTargetSelector;
    Knob chorusKnob, delayKnob, reverbKnob;
    Knob masterKnob;

    juce::ComboBox chordRootSelector, chordTypeSelector;
    juce::TextButton chordPlayButton{ "▶ Play" };
    juce::TextButton presetSaveButton{ "Save" }, presetLoadButton{ "Load" };

    EffectPanel effectPanel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqAttachment, fineAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment, resonanceAttachment, filterEnvAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment, decayAttachment, sustainAttachment, releaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoRateAttachment, lfoDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfoTargetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusAttachment, delayAttachment, reverbAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterAttachment;

    void addKnob (Knob&, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>&, const juce::String&, const juce::String&);
    void timerCallback() override;
    void handleChordPlay();
    void handleSavePreset();
    void handleLoadPreset();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZenithAudioEditor)
};
