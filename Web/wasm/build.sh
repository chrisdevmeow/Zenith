#!/bin/bash
em++ -O3 \
    -I../../Source \
    -I../../Source/DSP \
    ../../Source/DSP/Oscillator.cpp \
    ../../Source/DSP/Filter.cpp \
    ../../Source/DSP/Envelope.cpp \
    zenith_dsp.cpp \
    -o zenith_dsp.js \
    -s WASM=1 \
    -s EXPORT_NAME='ZenithModule' \
    -s MODULARIZE=1 \
    -s EXPORT_ES6=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ENVIRONMENT='web' \
    --bind
