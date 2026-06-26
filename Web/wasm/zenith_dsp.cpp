#include <emscripten/bind.h>
#include "../../Source/DSP/Oscillator.h"
#include "../../Source/DSP/Filter.h"
#include "../../Source/DSP/Envelope.h"

using namespace emscripten;

class ZenithDSP {
public:
    ZenithDSP() : sampleRate(44100.0f) {
        osc.setSampleRate(sampleRate);
        filter.setSampleRate(sampleRate);
        env.setSampleRate(sampleRate);
    }

    void setSampleRate(float rate) {
        sampleRate = rate;
        osc.setSampleRate(rate);
        filter.setSampleRate(rate);
        env.setSampleRate(rate);
    }

    // Oscillator
    void setWaveform(int wf) { osc.setWaveform(wf); }
    void setFrequency(float freq) { osc.setFrequency(freq); }
    void setFine(float fine) { osc.setFine(fine); }

    // Filter
    void setCutoff(float cutoff) { filter.setCutoff(cutoff); }
    void setResonance(float res) { filter.setResonance(res); }

    // Envelope
    void setAttack(float a) { env.setAttack(a); }
    void setDecay(float d) { env.setDecay(d); }
    void setSustain(float s) { env.setSustain(s); }
    void setRelease(float r) { env.setRelease(r); }

    void noteOn() { env.noteOn(); }
    void noteOff() { env.noteOff(); }

    float getNextSample() {
        float sample = osc.getNextSample();
        float filtered = filter.processSample(sample);
        float amp = env.getNextSample();
        return filtered * amp * 0.5f;
    }

    void reset() {
        osc.reset();
        filter.reset();
        env.reset();
    }

private:
    float sampleRate;
    Oscillator osc;
    Filter filter;
    Envelope env;
};

EMSCRIPTEN_BINDINGS(zenith_module) {
    class_<ZenithDSP>("ZenithDSP")
        .constructor()
        .function("setSampleRate", &ZenithDSP::setSampleRate)
        .function("setWaveform", &ZenithDSP::setWaveform)
        .function("setFrequency", &ZenithDSP::setFrequency)
        .function("setFine", &ZenithDSP::setFine)
        .function("setCutoff", &ZenithDSP::setCutoff)
        .function("setResonance", &ZenithDSP::setResonance)
        .function("setAttack", &ZenithDSP::setAttack)
        .function("setDecay", &ZenithDSP::setDecay)
        .function("setSustain", &ZenithDSP::setSustain)
        .function("setRelease", &ZenithDSP::setRelease)
        .function("noteOn", &ZenithDSP::noteOn)
        .function("noteOff", &ZenithDSP::noteOff)
        .function("getNextSample", &ZenithDSP::getNextSample)
        .function("reset", &ZenithDSP::reset);
}
