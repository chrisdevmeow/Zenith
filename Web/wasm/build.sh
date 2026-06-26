#!/bin/bash
em++ -O3 \
    --bind \
    -s WASM=1 \
    -s EXPORT_NAME='ZenithModule' \
    -s MODULARIZE=1 \
    -s EXPORT_ES6=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ENVIRONMENT='web' \
    -s SINGLE_FILE=0 \
    -o zenith_dsp.js \
    zenith_dsp.cpp
