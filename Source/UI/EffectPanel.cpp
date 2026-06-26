#include "EffectPanel.h"

EffectPanel::EffectPanel (EffectRack& rack) : effectRack (rack)
{
    // Effect selector
    addAndMakeVisible (effectSelector);
    effectSelector.addListener (this);
    refreshEffectList();

    // Add button
    addAndMakeVisible (addButton);
    addButton.addListener (this);

    // Remove button
    addAndMakeVisible (removeButton);
    removeButton.addListener (this);

    // Move buttons
    addAndMakeVisible (moveUpButton);
    moveUpButton.addListener (this);
    addAndMakeVisible (moveDownButton);
    moveDownButton.addListener (this);

    // Effect type selector (for adding new effects)
    addAndMakeVisible (effectTypeSelector);
    auto effectNames = EffectFactory::getAvailableEffects();
    effectTypeSelector.addItemList (effectNames, 1);
    effectTypeSelector.setSelectedId (1);

    refreshParameters();
}

EffectPanel::~EffectPanel() = default;

void EffectPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey.brighter (0.1f));
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("Effect Rack", getLocalBounds().reduced (10, 5),
                juce::Justification::topLeft, false);
}

void EffectPanel::resized()
{
    auto area = getLocalBounds().reduced (10);

    // Top row: selector + buttons
    auto topRow = area.removeFromTop (30);
    effectSelector.setBounds (topRow.removeFromLeft (150));
    topRow.removeFromLeft (10);
    addButton.setBounds (topRow.removeFromLeft (50));
    removeButton.setBounds (topRow.removeFromLeft (60));
    moveUpButton.setBounds (topRow.removeFromLeft (30));
    moveDownButton.setBounds (topRow.removeFromLeft (30));
    topRow.removeFromLeft (20);
    effectTypeSelector.setBounds (topRow.removeFromLeft (150));

    // Parameter sliders
    area.removeFromTop (10);
    int sliderHeight = 60;
    int spacing = 10;
    int totalHeight = paramSliders.size() * (sliderHeight + spacing);

    int x = 10;
    int y = area.getY();
    int width = (area.getWidth() - (paramSliders.size() - 1) * spacing) / (paramSliders.size() > 0 ? paramSliders.size() : 1);

    for (int i = 0; i < paramSliders.size(); ++i)
    {
        paramSliders[i]->setBounds (x, y, width, sliderHeight);
        paramLabels[i]->setBounds (x, y + sliderHeight - 20, width, 20);
        x += width + spacing;
    }
}

void EffectPanel::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &effectSelector)
    {
        refreshParameters();
    }
}

void EffectPanel::buttonClicked (juce::Button* button)
{
    if (button == &addButton)
    {
        int selectedIndex = effectTypeSelector.getSelectedId() - 1;
        auto effectNames = EffectFactory::getAvailableEffects();
        if (selectedIndex >= 0 && selectedIndex < effectNames.size())
        {
            auto effect = EffectFactory::create (effectNames[selectedIndex]);
            if (effect)
            {
                effectRack.addEffect (std::move (effect));
                refreshEffectList();
                effectSelector.setSelectedId (effectRack.size());
                refreshParameters();
            }
        }
    }
    else if (button == &removeButton)
    {
        int selected = effectSelector.getSelectedId() - 1;
        if (selected >= 0 && selected < effectRack.size())
        {
            effectRack.removeEffect (selected);
            refreshEffectList();
            if (effectRack.size() > 0)
            {
                int newSelected = juce::jmin (selected, effectRack.size() - 1);
                effectSelector.setSelectedId (newSelected + 1);
                refreshParameters();
            }
            else
            {
                paramSliders.clear();
                paramLabels.clear();
                attachments.clear();
                resized();
            }
        }
    }
    else if (button == &moveUpButton)
    {
        int selected = effectSelector.getSelectedId() - 1;
        if (selected > 0 && selected < effectRack.size())
        {
            effectRack.moveEffect (selected, selected - 1);
            refreshEffectList();
            effectSelector.setSelectedId (selected);
            refreshParameters();
        }
    }
    else if (button == &moveDownButton)
    {
        int selected = effectSelector.getSelectedId() - 1;
        if (selected >= 0 && selected < effectRack.size() - 1)
        {
            effectRack.moveEffect (selected, selected + 1);
            refreshEffectList();
            effectSelector.setSelectedId (selected + 2);
            refreshParameters();
        }
    }
}

void EffectPanel::refreshEffectList()
{
    effectSelector.clear();
    for (int i = 0; i < effectRack.size(); ++i)
    {
        auto* effect = effectRack.getEffect (i);
        if (effect)
            effectSelector.addItem (effect->getName(), i + 1);
    }
    if (effectRack.size() > 0)
        effectSelector.setSelectedId (1);
}

void EffectPanel::refreshParameters()
{
    updateSlidersForCurrentEffect();
    resized();
}

void EffectPanel::updateSlidersForCurrentEffect()
{
    paramSliders.clear();
    paramLabels.clear();
    attachments.clear();

    int selected = effectSelector.getSelectedId() - 1;
    auto* effect = effectRack.getEffect (selected);
    if (!effect) return;

    int numParams = effect->getNumParameters();
    auto paramNames = effect->getParameterNames();

    for (int i = 0; i < numParams; ++i)
    {
        auto* slider = new juce::Slider();
        slider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
        slider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
        slider->setRange (0.0f, 1.0f, 0.01f);
        slider->setValue (effect->getParameter (i));
        slider->onValueChange = [this, i, effect, slider] {
            effect->setParameter (i, (float)slider->getValue());
        };
        addAndMakeVisible (slider);
        paramSliders.add (slider);

        auto* label = new juce::Label();
        label->setText (paramNames[i], juce::dontSendNotification);
        label->setJustificationType (juce::Justification::centred);
        addAndMakeVisible (label);
        paramLabels.add (label);
    }
}
