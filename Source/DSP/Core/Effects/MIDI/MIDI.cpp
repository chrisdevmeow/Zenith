#include "MIDI.h"

// ===== ChordGenerator =====
ChordGenerator::ChordGenerator() = default;

void ChordGenerator::prepare(double, int) { 
    heldNotes.clear(); 
    chordActive = false; 
    lastRootNote = 0;
}

void ChordGenerator::process(juce::AudioBuffer<float>&) {}

void ChordGenerator::reset() { 
    heldNotes.clear(); 
    chordActive = false; 
}

juce::StringArray ChordGenerator::getParameterNames() const { 
    return {"Root", "ChordType"}; 
}

float ChordGenerator::getParameter(int index) const { 
    return index == 0 ? (float)root : (float)chordType; 
}

void ChordGenerator::setParameter(int index, float value) {
    if (index == 0) root = (int)value;
    else if (index == 1) chordType = (int)value;
}

void ChordGenerator::processMidi (juce::MidiBuffer& midiMessages, juce::MidiBuffer& outputBuffer)
{
    // Chord intervals: major, minor, 7th, maj7, sus4
    const int chordIntervals[5][4] = {
        {0, 4, 7, 12},   // major
        {0, 3, 7, 12},   // minor
        {0, 4, 7, 10},   // 7th
        {0, 4, 7, 11},   // maj7
        {0, 5, 7, 12}    // sus4
    };

    juce::Array<int> notesToRemove;

    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        int samplePos = metadata.samplePosition;

        if (message.isNoteOn())
        {
            int note = message.getNoteNumber();
            int velocity = message.getVelocity();
            int channel = message.getChannel();

            if (!heldNotes.contains(note))
                heldNotes.add(note);

            // Use the lowest held note as root
            if (heldNotes.size() > 0)
            {
                int rootNote = heldNotes.getFirst();
                if (rootNote != lastRootNote || !chordActive)
                {
                    // Turn off old chord
                    if (chordActive)
                    {
                        auto intervals = chordIntervals[chordType];
                        for (int i = 0; i < 4; ++i)
                        {
                            int chordNote = lastRootNote + intervals[i];
                            if (chordNote >= 0 && chordNote < 128)
                            {
                                juce::MidiMessage off = juce::MidiMessage::noteOff(channel, chordNote);
                                outputBuffer.addEvent(off, samplePos);
                            }
                        }
                    }

                    // Play new chord
                    auto intervals = chordIntervals[chordType];
                    for (int i = 0; i < 4; ++i)
                    {
                        int chordNote = rootNote + intervals[i];
                        if (chordNote >= 0 && chordNote < 128)
                        {
                            juce::MidiMessage on = juce::MidiMessage::noteOn(channel, chordNote, (juce::uint8)velocity);
                            outputBuffer.addEvent(on, samplePos);
                        }
                    }
                    lastRootNote = rootNote;
                    chordActive = true;
                }
            }
        }
        else if (message.isNoteOff())
        {
            int note = message.getNoteNumber();
            heldNotes.removeAllInstancesOf(note);

            if (heldNotes.size() == 0 && chordActive)
            {
                // Turn off all chord notes
                auto intervals = chordIntervals[chordType];
                for (int i = 0; i < 4; ++i)
                {
                    int chordNote = lastRootNote + intervals[i];
                    if (chordNote >= 0 && chordNote < 128)
                    {
                        juce::MidiMessage off = juce::MidiMessage::noteOff(message.getChannel(), chordNote);
                        outputBuffer.addEvent(off, samplePos);
                    }
                }
                chordActive = false;
            }
        }
    }

    // Pass through non-note MIDI messages
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        if (!message.isNoteOn() && !message.isNoteOff())
        {
            outputBuffer.addEvent(message, metadata.samplePosition);
        }
    }
}

// ===== Arpeggiator =====
Arpeggiator::Arpeggiator() = default;

void Arpeggiator::prepare(double sampleRate, int) { 
    this->sampleRate = sampleRate; 
    heldNotes.clear(); 
    arpeggioNotes.clear();
    stepIndex = 0;
    stepTime = 0.0;
    arpeggioActive = false;
}

void Arpeggiator::process(juce::AudioBuffer<float>&) {}

void Arpeggiator::reset() { 
    heldNotes.clear(); 
    arpeggioNotes.clear();
    stepIndex = 0;
    stepTime = 0.0;
    arpeggioActive = false;
}

juce::StringArray Arpeggiator::getParameterNames() const { 
    return {"Rate", "Gate"}; 
}

float Arpeggiator::getParameter(int index) const { 
    return index == 0 ? rate : gate; 
}

void Arpeggiator::setParameter(int index, float value) {
    if (index == 0) rate = value;
    else if (index == 1) gate = value;
}

void Arpeggiator::processMidi (juce::MidiBuffer& midiMessages, juce::MidiBuffer& outputBuffer, double bpm)
{
    double secondsPerBeat = 60.0 / bpm;
    double stepInterval = secondsPerBeat / (rate * 8.0 + 1.0);

    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        int samplePos = metadata.samplePosition;

        if (message.isNoteOn())
        {
            int note = message.getNoteNumber();
            int velocity = message.getVelocity();

            if (!heldNotes.contains(note))
            {
                heldNotes.add(note);
                // Sort held notes
                heldNotes.sort();
                // Build arpeggio from held notes
                arpeggioNotes.clear();
                for (int n : heldNotes)
                    arpeggioNotes.add(n);
                arpeggioActive = true;
                stepIndex = 0;
                stepTime = 0.0;
            }
        }
        else if (message.isNoteOff())
        {
            int note = message.getNoteNumber();
            heldNotes.removeAllInstancesOf(note);

            if (heldNotes.size() == 0)
            {
                arpeggioActive = false;
                arpeggioNotes.clear();
                // Turn off all arpeggio notes
                for (int n : arpeggioNotes)
                {
                    juce::MidiMessage off = juce::MidiMessage::noteOff(message.getChannel(), n);
                    outputBuffer.addEvent(off, samplePos);
                }
            }
            else
            {
                // Rebuild arpeggio from remaining notes
                arpeggioNotes.clear();
                for (int n : heldNotes)
                    arpeggioNotes.add(n);
                stepIndex = stepIndex % arpeggioNotes.size();
            }
        }
    }

    // Generate arpeggio steps
    if (arpeggioActive && arpeggioNotes.size() > 0)
    {
        double samplesPerStep = stepInterval * sampleRate;
        int stepSamples = (int)samplesPerStep;

        // Simplified arpeggio step triggering
        // In a real implementation, this would be driven by the audio callback timing
        // For now, we send a single arpeggio note per block
        int noteToPlay = arpeggioNotes[stepIndex % arpeggioNotes.size()];
        int channel = 1; // Default channel

        // Send note on
        juce::MidiMessage on = juce::MidiMessage::noteOn(channel, noteToPlay, (juce::uint8)100);
        outputBuffer.addEvent(on, 0);

        // Gate length: turn off after gate * step interval
        int gateSamples = (int)(stepInterval * gate * sampleRate);
        juce::MidiMessage off = juce::MidiMessage::noteOff(channel, noteToPlay);
        outputBuffer.addEvent(off, gateSamples);

        stepIndex++;
        if (stepIndex >= arpeggioNotes.size())
            stepIndex = 0;
    }
}
