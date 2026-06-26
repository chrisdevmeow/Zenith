#include <emscripten/bind.h>
#include <cmath>
#include <vector>

// ============================================================
// Independent DSP — no JUCE dependencies
// ============================================================

class Oscillator {
public:
    void setSampleRate(double rate) { sampleRate = rate; }
    void setWaveform(int wf) { waveform = wf; }
    void setFrequency(float freq) { this->freq = freq; }
    void setFine(float fine) { this->fine = fine; }
    void reset() { phase = 0.0f; }

    float getNextSample() {
        float increment = (freq + fine) / (float)sampleRate;
        phase += increment;
        if (phase >= 1.0f) phase -= 1.0f;

        switch (waveform) {
            case 0: return sinf(phase * 2.0f * 3.14159265f);
            case 1: return 1.0f - 2.0f * phase; // saw
            case 2: return phase < 0.5f ? 1.0f : -1.0f; // square
            case 3: return 1.0f - 4.0f * fabsf(phase - 0.5f); // triangle
            default: return 0.0f;
        }
    }

private:
    float phase = 0.0f;
    float freq = 440.0f;
    float fine = 0.0f;
    double sampleRate = 44100.0;
    int waveform = 0;
};

class Filter {
public:
    void setSampleRate(double rate) { sampleRate = rate; }
    void setCutoff(float cutoff) { this->cutoff = cutoff; updateCoeffs(); }
    void setResonance(float res) { resonance = res; updateCoeffs(); }
    void reset() { x1 = x2 = y1 = y2 = 0.0f; }

    float processSample(float input) {
        float output = b0 * input + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
        x2 = x1; x1 = input;
        y2 = y1; y1 = output;
        return output;
    }

private:
    void updateCoeffs() {
        float omega = 2.0f * 3.14159265f * cutoff / sampleRate;
        float sn = sinf(omega);
        float cs = cosf(omega);
        float alpha = sn / (2.0f * resonance);
        b0 = (1.0f - cs) / 2.0f;
        b1 = 1.0f - cs;
        b2 = (1.0f - cs) / 2.0f;
        a0 = 1.0f + alpha;
        a1 = -2.0f * cs;
        a2 = 1.0f - alpha;
        b0 /= a0; b1 /= a0; b2 /= a0;
        a1 /= a0; a2 /= a0;
    }

    float cutoff = 8000.0f, resonance = 0.5f, sampleRate = 44100.0f;
    float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f, a1 = 0.0f, a2 = 0.0f;
    float x1 = 0.0f, x2 = 0.0f, y1 = 0.0f, y2 = 0.0f;
};

class Envelope {
public:
    void setSampleRate(double rate) { sampleRate = rate; }
    void setAttack(float a) { attack = a; }
    void setRelease(float r) { release = r; }
    void noteOn() { state = 1; level = 0.0f; }
    void noteOff() { state = 2; }
    void reset() { state = 0; level = 0.0f; }

    float getNextSample() {
        if (state == 0) return 0.0f;
        if (state == 1) {
            level += 1.0f / (attack * sampleRate);
            if (level >= 1.0f) { level = 1.0f; state = 3; }
        } else if (state == 2) {
            level -= 1.0f / (release * sampleRate);
            if (level <= 0.0f) { level = 0.0f; state = 0; }
        }
        return level;
    }

private:
    float attack = 0.1f, release = 0.3f, sampleRate = 44100.0f;
    float level = 0.0f;
    int state = 0; // 0=idle, 1=attack, 2=release, 3=sustain
};

// ============================================================
// Zenith DSP Wrapper
// ============================================================

class ZenithDSP {
public:
    ZenithDSP() {
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

    void setWaveform(int wf) { osc.setWaveform(wf); }
    void setFrequency(float freq) { osc.setFrequency(freq); }
    void setFine(float fine) { osc.setFine(fine); }
    void setCutoff(float cutoff) { filter.setCutoff(cutoff); }
    void setResonance(float res) { filter.setResonance(res); }
    void setAttack(float a) { env.setAttack(a); }
    void setRelease(float r) { env.setRelease(r); }

    void noteOn() { env.noteOn(); }
    void noteOff() { env.noteOff(); }

    float getNextSample() {
        float s = osc.getNextSample();
        float f = filter.processSample(s);
        float e = env.getNextSample();
        return f * e * 0.5f;
    }

    void reset() {
        osc.reset();
        filter.reset();
        env.reset();
    }

private:
    float sampleRate = 44100.0f;
    Oscillator osc;
    Filter filter;
    Envelope env;
};

// ============================================================
// Emscripten Bindings
// ============================================================

using namespace emscripten;

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
        .function("setRelease", &ZenithDSP::setRelease)
        .function("noteOn", &ZenithDSP::noteOn)
        .function("noteOff", &ZenithDSP::noteOff)
        .function("getNextSample", &ZenithDSP::getNextSample)
        .function("reset", &ZenithDSP::reset);
}
