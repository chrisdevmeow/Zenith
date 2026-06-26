#include "Envelope.h"
#include <cmath>

Envelope::Envelope() = default;
Envelope::~Envelope() = default;

void Envelope::setAttack (float a)
{
    attack = juce::jlimit (0.001f, 10.0f, a);
}

void Envelope::setDecay (float d)
{
    decay = juce::jlimit (0.001f, 10.0f, d);
}

void Envelope::setSustain (float s)
{
    sustain = juce::jlimit (0.0f, 1.0f, s);
}

void Envelope::setRelease (float r)
{
    release = juce::jlimit (0.001f, 10.0f, r);
}

void Envelope::setSampleRate (double rate)
{
    sampleRate = rate;
}

void Envelope::noteOn()
{
    state = Attack;
    currentLevel = 0.0f;
    updateStep (attack, 1.0f);
}

void Envelope::noteOff()
{
    if (state != Idle)
    {
        state = Release;
        updateStep (release, 0.0f);
    }
}

float Envelope::getNextSample()
{
    if (state == Idle)
        return 0.0f;

    currentLevel += step;

    switch (state)
    {
        case Attack:
            if (currentLevel >= 1.0f)
            {
                currentLevel = 1.0f;
                state = Decay;
                updateStep (decay, sustain);
            }
            break;

        case Decay:
            if (currentLevel <= sustain)
            {
                currentLevel = sustain;
                state = Sustain;
                step = 0.0f;
            }
            break;

        case Sustain:
            break;

        case Release:
            if (currentLevel <= 0.0f)
            {
                currentLevel = 0.0f;
                state = Idle;
                step = 0.0f;
            }
            break;

        default: break;
    }

    return currentLevel;
}

bool Envelope::isActive() const
{
    return state != Idle;
}

void Envelope::reset()
{
    state = Idle;
    currentLevel = 0.0f;
    step = 0.0f;
}

void Envelope::updateStep (float time, float target)
{
    if (time <= 0.001f)
    {
        step = 0.0f;
        currentLevel = target;
        return;
    }

    float samples = time * (float) sampleRate;
    step = (target - currentLevel) / samples;
}
