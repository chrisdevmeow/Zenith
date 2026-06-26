import init, { ZenithDSP } from './wasm/zenith_dsp.js';

let audioCtx, dsp, scriptNode;
let activeNotes = new Set();

const status = document.getElementById('status');
const noteOnBtn = document.getElementById('noteOn');
const noteOffBtn = document.getElementById('noteOff');
const panicBtn = document.getElementById('panicBtn');
const pianoContainer = document.getElementById('piano');

// ----- piano -----
const noteNames = ['C4', 'C#4', 'D4', 'D#4', 'E4', 'F4', 'F#4', 'G4', 'G#4', 'A4', 'A#4', 'B4'];
const midiNotes = [60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71];

function buildPiano() {
    const wrapper = document.createElement('div');
    wrapper.style.cssText = 'display:flex; width:100%; height:100%; position:relative;';
    noteNames.forEach((name, i) => {
        const isBlack = name.includes('#');
        const key = document.createElement('div');
        key.className = `piano-key ${isBlack ? 'black' : 'white'}`;
        key.dataset.midi = midiNotes[i];
        key.dataset.note = name;
        key.style.flex = isBlack ? '0 0 0' : '1';
        key.style.position = isBlack ? 'relative' : 'static';
        key.style.marginLeft = isBlack ? '-12%' : '0';
        key.style.marginRight = isBlack ? '-12%' : '0';
        key.style.zIndex = isBlack ? '2' : '1';
        key.style.height = isBlack ? '60%' : '100%';
        key.style.alignSelf = isBlack ? 'flex-start' : 'stretch';
        key.style.borderRadius = isBlack ? '0 0 4px 4px' : '0 0 6px 6px';
        key.addEventListener('mousedown', (e) => { e.preventDefault(); startNote(midiNotes[i]); });
        key.addEventListener('mouseup', (e) => { e.preventDefault(); stopNote(midiNotes[i]); });
        key.addEventListener('mouseleave', () => { stopNote(midiNotes[i]); });
        key.addEventListener('touchstart', (e) => { e.preventDefault(); startNote(midiNotes[i]); });
        key.addEventListener('touchend', (e) => { e.preventDefault(); stopNote(midiNotes[i]); });
        key.addEventListener('touchcancel', (e) => { e.preventDefault(); stopNote(midiNotes[i]); });
        const label = document.createElement('div');
        label.className = 'piano-key-label';
        label.textContent = name.replace('#', '♯');
        key.appendChild(label);
        wrapper.appendChild(key);
    });
    pianoContainer.appendChild(wrapper);
}

// ----- DSP -----
async function initDSP() {
    try {
        await init();
        audioCtx = new AudioContext();
        dsp = new ZenithDSP();
        dsp.setSampleRate(audioCtx.sampleRate);
        syncParams();

        scriptNode = audioCtx.createScriptProcessor(256, 0, 1);
        scriptNode.onaudioprocess = (e) => {
            if (!dsp) return;
            const output = e.outputBuffer.getChannelData(0);
            for (let i = 0; i < output.length; i++) {
                output[i] = dsp.getNextSample();
            }
        };
        scriptNode.connect(audioCtx.destination);
        status.textContent = '✅ Ready — tap piano keys';
    } catch (e) {
        status.textContent = '❌ Failed to load WASM';
        console.error(e);
    }
}

function syncParams() {
    if (!dsp) return;
    dsp.setWaveform(parseInt(document.getElementById('waveform').value));
    dsp.setFrequency(parseFloat(document.getElementById('freq').value));
    dsp.setCutoff(parseFloat(document.getElementById('cutoff').value));
    dsp.setResonance(parseFloat(document.getElementById('resonance').value));
    dsp.setAttack(parseFloat(document.getElementById('attack').value));
    dsp.setRelease(parseFloat(document.getElementById('release').value));
}

function startNote(midi) {
    if (!dsp || !audioCtx) return;
    if (audioCtx.state === 'suspended') audioCtx.resume();
    const freq = 440 * Math.pow(2, (midi - 69) / 12);
    dsp.setFrequency(freq);
    dsp.noteOn();
    activeNotes.add(midi);
    document.querySelectorAll('.piano-key').forEach(el => {
        if (parseInt(el.dataset.midi) === midi) el.classList.add('active');
    });
    status.textContent = `🔊 Note ON ${midi}`;
}

function stopNote(midi) {
    if (!dsp) return;
    dsp.noteOff();
    activeNotes.delete(midi);
    document.querySelectorAll('.piano-key').forEach(el => {
        if (parseInt(el.dataset.midi) === midi) el.classList.remove('active');
    });
    status.textContent = `🔇 Note OFF ${midi}`;
}

function panic() {
    if (!dsp) return;
    dsp.noteOff();
    activeNotes.clear();
    document.querySelectorAll('.piano-key').forEach(el => el.classList.remove('active'));
    status.textContent = '⛔ Panic! All notes off';
}

// ----- UI bindings -----
document.querySelectorAll('input, select').forEach(el => {
    el.addEventListener('input', syncParams);
    el.addEventListener('change', syncParams);
});
document.querySelectorAll('input[type="range"]').forEach(el => {
    el.addEventListener('input', () => {
        const val = document.getElementById(el.id + 'Val');
        if (val) val.textContent = parseFloat(el.value).toFixed(2);
    });
});

noteOnBtn.onclick = () => { if (dsp) { dsp.noteOn(); status.textContent = '🔊 Note ON (manual)'; } };
noteOffBtn.onclick = () => { if (dsp) { dsp.noteOff(); status.textContent = '🔇 Note OFF'; } };
panicBtn.onclick = panic;

buildPiano();
initDSP();
