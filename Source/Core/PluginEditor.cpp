#include "PluginEditor.h"

ZenithAudioEditor::ZenithAudioEditor (ZenithAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // ========== SYNTH SECTION ==========
    addAndMakeVisible (waveSelector);
    waveSelector.addItemList ({ "Sine", "Saw", "Square", "Triangle" }, 1);
    waveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.apvts, "waveform", waveSelector);

    addSlider (freqSlider, freqAttachment, "freq", "Freq");
    addSlider (fineSlider, fineAttachment, "fine", "Fine");
    addSlider (cutoffSlider, cutoffAttachment, "cutoff", "Cutoff");
    addSlider (resonanceSlider, resonanceAttachment, "resonance", "Res");
    addSlider (filterEnvSlider, filterEnvAttachment, "filterEnv", "F.Env");

    addSlider (attackSlider, attackAttachment, "attack", "A");
    addSlider (decaySlider, decayAttachment, "decay", "D");
    addSlider (sustainSlider, sustainAttachment, "sustain", "S");
    addSlider (releaseSlider, releaseAttachment, "release", "R");

    // ========== MODULATION ==========
    addSlider (lfoRateSlider, lfoRateAttachment, "lfoRate", "Rate");
    addSlider (lfoDepthSlider, lfoDepthAttachment, "lfoDepth", "Depth");
    addComboBox (lfoTargetSelector, lfoTargetAttachment, "lfoTarget", { "Pitch", "Filter", "Amp" });

    // ========== EFFECTS ==========
    addSlider (chorusSlider, chorusAttachment, "chorusMix", "Chorus");
    addSlider (delaySlider, delayAttachment, "delayMix", "Delay");
    addSlider (reverbSlider, reverbAttachment, "reverbMix", "Reverb");

    // ========== MASTER ==========
    addSlider (masterSlider, masterAttachment, "masterGain", "Master");

    // ========== CHORD ==========
    addComboBox (chordRootSelector, chordRootAttachment, "chordRoot", { "C","C#","D","D#","E","F","F#","G","G#","A","A#","B" });
    addComboBox (chordTypeSelector, chordTypeAttachment, "chordType", { "Major","Minor","7","Maj7","Sus2","Sus4" });

    chordPlayButton.onClick = [this] { audioProcessor.playChord(); };
    addAndMakeVisible (chordPlayButton);

    // ========== PRESETS ==========
    presetSaveButton.onClick = [this] { audioProcessor.savePreset(); };
    presetLoadButton.onClick = [this] { audioProcessor.loadPreset(); };
    addAndMakeVisible (presetSaveButton);
    addAndMakeVisible (presetLoadButton);

    // ========== LOOK & FEEL ==========
    setLookAndFeel (&lookAndFeel);
    lookAndFeel.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::cyan.darker (0.3f));

    setSize (1100, 600);
    startTimerHz (24);
}

ZenithAudioEditor::~ZenithAudioEditor()
{
    setLookAndFeel (nullptr);
}

void ZenithAudioEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff1a1a2e)); // deep dark purple
}

void ZenithAudioEditor::resized()
{
    auto area = getLocalBounds().reduced (16);

    // Section widths: Synth 50%, FX + Mod 30%, Master + Presets 20%
    auto synthArea = area.removeFromLeft (area.getWidth() * 0.5);
    auto fxArea = area.removeFromLeft (area.getWidth() * 0.5); // remaining split
    auto masterArea = area;

    // --- Synth Section ---
    auto synthRow = synthArea.removeFromTop (30);
    waveSelector.setBounds (synthRow.removeFromLeft (120));

    auto synthGrid = synthArea;
    auto gridCols = 5;
    auto cellW = synthGrid.getWidth() / gridCols;

    freqSlider.setBounds (synthGrid.removeFromLeft (cellW).reduced (6));
    fineSlider.setBounds (synthGrid.removeFromLeft (cellW).reduced (6));
    cutoffSlider.setBounds (synthGrid.removeFromLeft (cellW).reduced (6));
    resonanceSlider.setBounds (synthGrid.removeFromLeft (cellW).reduced (6));
    filterEnvSlider.setBounds (synthGrid.removeFromLeft (cellW).reduced (6));

    // ADSR row
    auto adsrArea = synthArea.removeFromBottom (90);
    attackSlider.setBounds (adsrArea.removeFromLeft (cellW).reduced (6));
    decaySlider.setBounds (adsrArea.removeFromLeft (cellW).reduced (6));
    sustainSlider.setBounds (adsrArea.removeFromLeft (cellW).reduced (6));
    releaseSlider.setBounds (adsrArea.removeFromLeft (cellW).reduced (6));

    // --- FX + Modulation ---
    auto fxGrid = fxArea;
    auto fxCols = 4;
    auto fxW = fxGrid.getWidth() / fxCols;

    chorusSlider.setBounds (fxGrid.removeFromLeft (fxW).reduced (6));
    delaySlider.setBounds (fxGrid.removeFromLeft (fxW).reduced (6));
    reverbSlider.setBounds (fxGrid.removeFromLeft (fxW).reduced (6));

    // LFO row (under FX)
    auto lfoArea = fxArea.removeFromBottom (90);
    lfoRateSlider.setBounds (lfoArea.removeFromLeft (fxW).reduced (6));
    lfoDepthSlider.setBounds (lfoArea.removeFromLeft (fxW).reduced (6));
    lfoTargetSelector.setBounds (lfoArea.removeFromLeft (fxW).reduced (6));

    // --- Master + Presets + Chord ---
    auto masterRow = masterArea.removeFromTop (40);
    masterSlider.setBounds (masterRow.removeFromLeft (120).reduced (4));
    presetSaveButton.setBounds (masterRow.removeFromLeft (80).reduced (4));
    presetLoadButton.setBounds (masterRow.removeFromLeft (80).reduced (4));

    auto chordRow = masterArea.reduced (8);
    chordRootSelector.setBounds (chordRow.removeFromLeft (80).reduced (4));
    chordTypeSelector.setBounds (chordRow.removeFromLeft (100).reduced (4));
    chordPlayButton.setBounds (chordRow.removeFromLeft (80).reduced (4));
}

void ZenithAudioEditor::timerCallback()
{
    // update any dynamic displays if needed
}

// ----- helper to add sliders -----
void ZenithAudioEditor::addSlider (juce::Slider& slider,
                                   std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
                                   const juce::String& paramID,
                                   const juce::String& labelText)
{
    addAndMakeVisible (slider);
    slider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 16);
    slider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::cyan.darker (0.4f));
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        audioProcessor.apvts, paramID, slider);
}

void ZenithAudioEditor::addComboBox (juce::ComboBox& box,
                                     std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& attachment,
                                     const juce::String& paramID,
                                     const juce::StringArray& items)
{
    addAndMakeVisible (box);
    box.addItemList (items, 1);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        audioProcessor.apvts, paramID, box);
}
