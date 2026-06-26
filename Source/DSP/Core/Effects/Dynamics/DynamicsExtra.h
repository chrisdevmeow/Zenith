#pragma once
#include "../../Core/Effect.h"

class MultibandCompressor : public Effect
{
public:
    MultibandCompressor();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "MultibandCompressor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 6; }

private:
    juce::dsp::ProcessorChain<juce::dsp::Compressor<float>, juce::dsp::Compressor<float>, juce::dsp::Compressor<float>> compressorChain;
    float lowThresh = -12.0f, midThresh = -12.0f, highThresh = -12.0f;
    float lowRatio = 4.0f, midRatio = 4.0f, highRatio = 4.0f;
};

class MultibandExpander : public Effect
{
public:
    MultibandExpander();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "MultibandExpander"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 6; }

private:
    juce::dsp::ProcessorChain<juce::dsp::Expander<float>, juce::dsp::Expander<float>, juce::dsp::Expander<float>> expanderChain;
    float lowThresh = -20.0f, midThresh = -20.0f, highThresh = -20.0f;
    float lowRatio = 2.0f, midRatio = 2.0f, highRatio = 2.0f;
};

class PeakCompressor : public Effect
{
public:
    PeakCompressor();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "PeakCompressor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::Compressor<float> compressor;
    float threshold = -12.0f, ratio = 4.0f, attack = 1.0f, release = 100.0f;
};

class RMSCompressor : public Effect
{
public:
    RMSCompressor();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "RMSCompressor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::Compressor<float> compressor;
    float threshold = -12.0f, ratio = 4.0f, attack = 10.0f, release = 100.0f;
};

class OpticalCompressor : public Effect
{
public:
    OpticalCompressor();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "OpticalCompressor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::Compressor<float> compressor;
    float threshold = -10.0f, ratio = 3.0f, attack = 15.0f, release = 80.0f;
};

class FETCompressor : public Effect
{
public:
    FETCompressor();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "FETCompressor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::Compressor<float> compressor;
    float threshold = -12.0f, ratio = 4.0f, attack = 5.0f, release = 50.0f;
};

class VCACompressor : public Effect
{
public:
    VCACompressor();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "VCACompressor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::Compressor<float> compressor;
    float threshold = -12.0f, ratio = 4.0f, attack = 10.0f, release = 100.0f;
};

class VariMuCompressor : public Effect
{
public:
    VariMuCompressor();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "VariMuCompressor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::Compressor<float> compressor;
    float threshold = -8.0f, ratio = 3.0f, attack = 20.0f, release = 150.0f;
};

class DiodeCompressor : public Effect
{
public:
    DiodeCompressor();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "DiodeCompressor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::Compressor<float> compressor;
    float threshold = -10.0f, ratio = 4.0f, attack = 8.0f, release = 80.0f;
};

class FeedbackCompressor : public Effect
{
public:
    FeedbackCompressor();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "FeedbackCompressor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::Compressor<float> compressor;
    float threshold = -12.0f, ratio = 4.0f, attack = 10.0f, release = 100.0f;
};

class FeedForwardCompressor : public Effect
{
public:
    FeedForwardCompressor();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "FeedForwardCompressor"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 4; }

private:
    juce::dsp::Compressor<float> compressor;
    float threshold = -12.0f, ratio = 4.0f, attack = 10.0f, release = 100.0f;
};

class EnvelopeFollower : public Effect
{
public:
    EnvelopeFollower();
    void prepare (double sampleRate, int samplesPerBlock) override;
    void process (juce::AudioBuffer<float>& buffer) override;
    void reset() override;
    juce::String getName() const override { return "EnvelopeFollower"; }
    juce::StringArray getParameterNames() const override;
    float getParameter (int index) const override;
    void setParameter (int index, float value) override;
    int getNumParameters() const override { return 3; }

private:
    float attack = 10.0f, release = 100.0f, output = 1.0f;
    double sampleRate = 44100.0;
    float envelope = 0.0f;
};
