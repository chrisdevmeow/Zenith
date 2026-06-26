#include "PluginEditor.h"

ZenithAudioEditor::ZenithAudioEditor (ZenithAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), effectPanel (p.getEffectRack())
{
    addAndMakeVisible (waveSelector);
    waveSelector.addItemList ({ "Sine", "Saw", "Square", "Triangle" }, 1);
    waveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.apvts, "waveform", waveSelector);

    addKnob (freqKnob, freqAttachment, "freq", "Freq");
    addKnob (fineKnob, fineAttachment, "fine", "Fine");
    addKnob (cutoffKnob, cutoffAttachment, "cutoff", "Cutoff");
    addKnob (resonanceKnob, resonanceAttachment, "resonance", "Res");
    addKnob (filterEnvKnob, filterEnvAttachment, "filterEnv", "F.Env");
    addKnob (attackKnob, attackAttachment, "attack", "A");
    addKnob (decayKnob, decayAttachment, "decay", "D");
    addKnob (sustainKnob, sustainAttachment, "sustain", "S");
    addKnob (releaseKnob, releaseAttachment, "release", "R");
    addKnob (lfoRateKnob, lfoRateAttachment, "lfoRate", "Rate");
    addKnob (lfoDepthKnob, lfoDepthAttachment, "lfoDepth", "Depth");

    addAndMakeVisible (lfoTargetSelector);
    lfoTargetSelector.addItemList ({ "Pitch", "Filter", "Amp" }, 1);
    lfoTargetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.apvts, "lfoTarget", lfoTargetSelector);

    addKnob (chorusKnob, chorusAttachment, "chorusMix", "Chorus");
    addKnob (delayKnob, delayAttachment, "delayMix", "Delay");
    addKnob (reverbKnob, reverbAttachment, "reverbMix", "Reverb");
    addKnob (masterKnob, masterAttachment, "masterGain", "Master");

    addAndMakeVisible (chordRootSelector);
    chordRootSelector.addItemList ({ "C","C#","D","D#","E","F","F#","G","G#","A","A#","B" }, 1);
    addAndMakeVisible (chordTypeSelector);
    chordTypeSelector.addItemList ({ "Major","Minor","7","Maj7","Sus2","Sus4" }, 1);
    addAndMakeVisible (chordPlayButton);
    chordPlayButton.onClick = [this] { handleChordPlay(); };

    addAndMakeVisible (presetSaveButton);
    addAndMakeVisible (presetLoadButton);
    presetSaveButton.onClick = [this] { handleSavePreset(); };
    presetLoadButton.onClick = [this] { handleLoadPreset(); };

    addAndMakeVisible (effectPanel);

    setLookAndFeel (&lookAndFeel);
    lookAndFeel.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::cyan.darker (0.3f));

    setSize (1200, 700);
    startTimerHz (30);
}

ZenithAudioEditor::~ZenithAudioEditor() { setLookAndFeel (nullptr); }

void ZenithAudioEditor::paint (juce::Graphics& g) { g.fillAll (juce::Colour (0xff1a1a2e)); }

void ZenithAudioEditor::resized()
{
    auto area = getLocalBounds().reduced (16);
    auto topSection = area.removeFromTop (area.getHeight() * 0.65);
    auto synthArea = topSection.removeFromLeft (topSection.getWidth() * 0.6);

    auto waveRow = synthArea.removeFromTop (30);
    waveSelector.setBounds (waveRow.removeFromLeft (120));

    auto oscRow = synthArea.removeFromTop (80);
    freqKnob.setBounds (oscRow.removeFromLeft (80).reduced (6));
    fineKnob.setBounds (oscRow.removeFromLeft (80).reduced (6));

    auto filterRow = synthArea.removeFromTop (80);
    cutoffKnob.setBounds (filterRow.removeFromLeft (80).reduced (6));
    resonanceKnob.setBounds (filterRow.removeFromLeft (80).reduced (6));
    filterEnvKnob.setBounds (filterRow.removeFromLeft (80).reduced (6));

    auto adsrRow = synthArea.removeFromTop (80);
    attackKnob.setBounds (adsrRow.removeFromLeft (80).reduced (6));
    decayKnob.setBounds (adsrRow.removeFromLeft (80).reduced (6));
    sustainKnob.setBounds (adsrRow.removeFromLeft (80).reduced (6));
    releaseKnob.setBounds (adsrRow.removeFromLeft (80).reduced (6));

    auto lfoRow = synthArea.removeFromTop (80);
    lfoRateKnob.setBounds (lfoRow.removeFromLeft (80).reduced (6));
    lfoDepthKnob.setBounds (lfoRow.removeFromLeft (80).reduced (6));
    lfoTargetSelector.setBounds (lfoRow.removeFromLeft (120).reduced (6));

    auto fxArea = topSection;
    auto fxRow = fxArea.removeFromTop (80);
    chorusKnob.setBounds (fxRow.removeFromLeft (80).reduced (6));
    delayKnob.setBounds (fxRow.removeFromLeft (80).reduced (6));
    reverbKnob.setBounds (fxRow.removeFromLeft (80).reduced (6));

    auto masterRow = fxArea.removeFromTop (80);
    masterKnob.setBounds (masterRow.removeFromLeft (80).reduced (6));

    auto chordRow = fxArea.removeFromTop (40);
    chordRootSelector.setBounds (chordRow.removeFromLeft (80).reduced (4));
    chordTypeSelector.setBounds (chordRow.removeFromLeft (100).reduced (4));
    chordPlayButton.setBounds (chordRow.removeFromLeft (80).reduced (4));

    auto presetRow = fxArea.removeFromTop (40);
    presetSaveButton.setBounds (presetRow.removeFromLeft (80).reduced (4));
    presetLoadButton.setBounds (presetRow.removeFromLeft (80).reduced (4));

    effectPanel.setBounds (area);
}

void ZenithAudioEditor::timerCallback() {}

void ZenithAudioEditor::addKnob (Knob& knob,
                                 std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
                                 const juce::String& paramID,
                                 const juce::String& labelText)
{
    addAndMakeVisible (knob);
    knob.setTextValueSuffix (labelText);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.apvts, paramID, knob);
}

void ZenithAudioEditor::handleChordPlay()
{
    int root = chordRootSelector.getSelectedId() - 1;
    int type = chordTypeSelector.getSelectedId() - 1;
    audioProcessor.playChord (root, type);
}

void ZenithAudioEditor::handleSavePreset()
{
    juce::FileChooser chooser ("Save Preset",
                               juce::File::getSpecialLocation (juce::File::userDocumentsDirectory)
                                   .getChildFile ("ZenithPreset.zenith"),
                               "*.zenith");
    if (chooser.browseForFileToSave (true))
    {
        auto file = chooser.getResult();
        juce::MemoryBlock block;
        audioProcessor.getStateInformation (block);
        file.replaceWithData (block.getData(), block.getSize());
    }
}

void ZenithAudioEditor::handleLoadPreset()
{
    juce::FileChooser chooser ("Load Preset",
                               juce::File::getSpecialLocation (juce::File::userDocumentsDirectory),
                               "*.zenith");
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto data = file.loadFileAsData();
        audioProcessor.setStateInformation (data.getData(), (int)data.getSize());
    }
}
