#pragma once
#include <JuceHeader.h>

class Envelope
{
public:
    Envelope();
    ~Envelope();

    void setAttack (float attack);
    void setDecay (float decay);
    void setSustain (float sustain);
    void setRelease (float release);
    void setSampleRate (double rate);

    void noteOn();
    void noteOff();
    float getNextSample();
    bool isActive() const;
    void reset();

private:
    enum State { Idle, Attack, Decay, Sustain, Release };
    State state = Idle;

    float attack = 0.1f;
    float decay = 0.2f;
    float sustain = 0.8f;
    float release = 0.3f;
    float sampleRate = 44100.0f;

    float currentLevel = 0.0f;
    float step = 0.0f;

    void updateStep (float time, float target);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope)
};
