#include "MIDIExtra.h"

// ===== StepSequencer =====
StepSequencer::StepSequencer() = default;

void StepSequencer::prepare(double sampleRate, int) { 
    this->sampleRate = sampleRate; 
    step = 0; 
    active = false;
    sequence = {60, 62, 64, 65, 67, 69, 71, 72}; // Default C major scale
}

void StepSequencer::process(juce::AudioBuffer<float>&) {}

void StepSequencer::reset() { step = 0; active = false; }

juce::StringArray StepSequencer::getParameterNames() const { 
    return {"Rate", "Gate"}; 
}

float StepSequencer::getParameter(int index) const { 
    return index == 0 ? rate : gate; 
}

void StepSequencer::setParameter(int index, float value) {
    if (index == 0) rate = value;
    else if (index == 1) gate = value;
}

void StepSequencer::processMidi (juce::MidiBuffer& midiMessages, juce::MidiBuffer& outputBuffer, double bpm)
{
    double secondsPerBeat = 60.0 / bpm;
    double stepInterval = secondsPerBeat / (rate * 8.0 + 1.0);

    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        int samplePos = metadata.samplePosition;

        if (message.isNoteOn() && !active)
        {
            active = true;
            step = 0;
        }
        else if (message.isNoteOff() && active)
        {
            // Check if any notes are still held
            bool anyHeld = false;
            for (const auto m : midiMessages)
            {
                if (m.getMessage().isNoteOn())
                    anyHeld = true;
            }
            if (!anyHeld)
                active = false;
        }
    }

    if (active && sequence.size() > 0)
    {
        int note = sequence[step % sequence.size()];
        int channel = 1;

        juce::MidiMessage on = juce::MidiMessage::noteOn(channel, note, (juce::uint8)100);
        outputBuffer.addEvent(on, 0);

        int gateSamples = (int)(stepInterval * gate * sampleRate);
        juce::MidiMessage off = juce::MidiMessage::noteOff(channel, note);
        outputBuffer.addEvent(off, gateSamples);

        step++;
    }
}

// ===== ScaleFilter =====
ScaleFilter::ScaleFilter() = default;

void ScaleFilter::prepare(double, int) {}

void ScaleFilter::process(juce::AudioBuffer<float>&) {}

void ScaleFilter::reset() {}

juce::StringArray ScaleFilter::getParameterNames() const { 
    return {"ScaleType"}; 
}

float ScaleFilter::getParameter(int index) const { 
    return index == 0 ? (float)scaleType : 0.0f; 
}

void ScaleFilter::setParameter(int index, float value) {
    if (index == 0) scaleType = (int)value;
}

void ScaleFilter::processMidi (juce::MidiBuffer& midiMessages, juce::MidiBuffer& outputBuffer)
{
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        int samplePos = metadata.samplePosition;

        if (message.isNoteOn())
        {
            int note = message.getNoteNumber();
            int velocity = message.getVelocity();
            int channel = message.getChannel();

            // Find the root note (C = 0)
            int root = 0;
            int octave = note / 12;
            int noteInOctave = note % 12;

            // Map to scale
            int mappedNote = noteInOctave;
            switch (scaleType)
            {
                case 0: // Major
                {
                    int nearest = 0;
                    for (int i = 0; i < 7; ++i)
                    {
                        if (majorScale[i] <= noteInOctave)
                            nearest = majorScale[i];
                    }
                    mappedNote = nearest;
                    break;
                }
                case 1: // Minor
                {
                    int nearest = 0;
                    for (int i = 0; i < 7; ++i)
                    {
                        if (minorScale[i] <= noteInOctave)
                            nearest = minorScale[i];
                    }
                    mappedNote = nearest;
                    break;
                }
                case 2: // Pentatonic
                {
                    int nearest = 0;
                    for (int i = 0; i < 5; ++i)
                    {
                        if (pentatonicScale[i] <= noteInOctave)
                            nearest = pentatonicScale[i];
                    }
                    mappedNote = nearest;
                    break;
                }
                default:
                    mappedNote = noteInOctave;
            }

            int newNote = octave * 12 + mappedNote;
            if (newNote >= 0 && newNote < 128)
            {
                juce::MidiMessage newOn = juce::MidiMessage::noteOn(channel, newNote, (juce::uint8)velocity);
                outputBuffer.addEvent(newOn, samplePos);
            }
        }
        else
        {
            // Pass through non-note messages and note-offs
            outputBuffer.addEvent(message, samplePos);
        }
    }
}
