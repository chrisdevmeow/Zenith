#pragma once
#include "../../Core/Effect.h"

class Chorus : public Effect
{
public:
    Chorus();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Chorus"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 3; }
private:
    juce::dsp::Chorus<float> chorus;
    float rate = 1.5f, depth = 0.5f, mix = 0.3f;
};

class Flanger : public Effect
{
public:
    Flanger();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Flanger"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 3; }
private:
    juce::dsp::Flanger<float> flanger;
    float rate = 0.5f, depth = 0.7f, mix = 0.4f;
};

class Phaser : public Effect
{
public:
    Phaser();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Phaser"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 3; }
private:
    juce::dsp::Phaser<float> phaser;
    float rate = 0.3f, depth = 0.6f, mix = 0.3f;
};

class Tremolo : public Effect
{
public:
    Tremolo();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Tremolo"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float rate = 5.0f, depth = 0.5f;
    double sampleRate = 44100.0;
    float phase = 0.0f;
};

class Vibrato : public Effect
{
public:
    Vibrato();
    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "Vibrato"; }
    juce::StringArray getParameterNames() const override;
    float getParameter(int index) const override;
    void setParameter(int index, float value) override;
    int getNumParameters() const override { return 2; }
private:
    float rate = 5.0f, depth = 0.3f;
    double sampleRate = 44100.0;
    float phase = 0.0f;
};
