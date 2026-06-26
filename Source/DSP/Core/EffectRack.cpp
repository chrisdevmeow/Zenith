#include "EffectRack.h"
#include "EffectFactory.h"

void EffectRack::addEffect (std::unique_ptr<Effect> effect)
{
    effects.push_back (std::move (effect));
}

void EffectRack::removeEffect (int index)
{
    if (index >= 0 && index < (int)effects.size())
        effects.erase (effects.begin() + index);
}

void EffectRack::clear()
{
    effects.clear();
}

int EffectRack::size() const
{
    return (int)effects.size();
}

void EffectRack::prepare (double sampleRate, int samplesPerBlock)
{
    for (auto& effect : effects)
        effect->prepare (sampleRate, samplesPerBlock);
}

void EffectRack::process (juce::AudioBuffer<float>& buffer)
{
    for (auto& effect : effects)
        effect->process (buffer);
}

void EffectRack::reset()
{
    for (auto& effect : effects)
        effect->reset();
}

Effect* EffectRack::getEffect (int index)
{
    if (index >= 0 && index < (int)effects.size())
        return effects[index].get();
    return nullptr;
}

juce::StringArray EffectRack::getEffectNames() const
{
    juce::StringArray names;
    for (const auto& effect : effects)
        names.add (effect->getName());
    return names;
}

juce::var EffectRack::toVar() const
{
    juce::Array<juce::var> effectData;
    for (const auto& effect : effects)
    {
        juce::DynamicObject::Ptr obj = new juce::DynamicObject();
        obj->setProperty ("name", effect->getName());
        juce::Array<juce::var> params;
        for (int i = 0; i < effect->getNumParameters(); ++i)
            params.add (effect->getParameter (i));
        obj->setProperty ("params", params);
        effectData.add (juce::var (obj.get()));
    }
    return juce::var (effectData);
}

void EffectRack::fromVar (const juce::var& data)
{
    clear();
    auto array = data.getArray();
    if (!array) return;
    for (auto& item : *array)
    {
        auto obj = item.getDynamicObject();
        if (!obj) continue;
        auto name = obj->getProperty ("name").toString();
        auto effect = EffectFactory::create (name);
        if (!effect) continue;
        auto params = obj->getProperty ("params").getArray();
        if (params)
        {
            for (int i = 0; i < juce::jmin (effect->getNumParameters(), (int)params->size()); ++i)
                effect->setParameter (i, (float)(*params)[i]);
        }
        addEffect (std::move (effect));
    }
}
