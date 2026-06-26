cmake_minimum_required(VERSION 3.22)
project(Zenith VERSION 1.0.0)

# ============================================================================
# JUCE
# ============================================================================

set(JUCE_VERSION 7.0.5)
set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "Disable JUCE examples")
set(JUCE_BUILD_JUCE_TESTS OFF CACHE BOOL "Disable JUCE tests")

add_subdirectory(JUCE)

# ============================================================================
# Plugin Configuration
# ============================================================================

juce_add_plugin(Zenith
    PLUGIN_CODE               "Znt1"
    VERSION                   1.0.0

    PLUGIN_VST3_CATEGORY "Synth"

    IS_SYNTH          1
    IS_MIDI_EFFECT    0
    EDITOR_WANTS_KEYBOARD_FOCUS 1

    VST3                ON
    Standalone          ON
    AUv3                OFF  # macOS only — turn on if you're on Mac
    AAX                 OFF
)

# ============================================================================
# Source Files
# ============================================================================

target_sources(Zenith PRIVATE
    Source/Core/PluginProcessor.cpp
    Source/Core/PluginProcessor.h
    Source/Core/PluginEditor.cpp
    Source/Core/PluginEditor.h

    Source/Synth/SynthVoice.cpp
    Source/Synth/SynthVoice.h
    Source/Synth/SynthSound.cpp
    Source/Synth/SynthSound.h

    Source/DSP/Oscillator.cpp
    Source/DSP/Oscillator.h
    Source/DSP/Filter.cpp
    Source/DSP/Filter.h
    Source/DSP/Envelope.cpp
    Source/DSP/Envelope.h

    Source/UI/Knob.cpp
    Source/UI/Knob.h
    Source/UI/LookAndFeel.cpp
    Source/UI/LookAndFeel.h

    Source/Utils/MathHelpers.h
    Source/Utils/ParameterHelpers.h
)

# ============================================================================
# Include Directories
# ============================================================================

target_include_directories(Zenith PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Synth
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/DSP
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utils
)

# ============================================================================
# Resources
# ============================================================================

juce_add_binary_data(Zenith_BinaryData
    Resources/Fonts/
    Resources/Presets/
)

target_sources(Zenith PRIVATE ${Zenith_BinaryData})

# ============================================================================
# Compiler Flags
# ============================================================================

target_compile_definitions(Zenith PRIVATE
    JUCE_USE_CURL=0
    JUCE_DISABLE_MIDI_INPUT=0
)

if (MSVC)
    target_compile_options(Zenith PRIVATE /W4 /WX-)
else()
    target_compile_options(Zenith PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor)
endif()

# ============================================================================
# Post-build (Windows: copy to your VST3 folder)
# ============================================================================

if (WIN32 AND EXISTS "$ENV{USERPROFILE}/Documents/VST3")
    add_custom_command(TARGET Zenith POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Zenith>
            "$ENV{USERPROFILE}/Documents/VST3/"
        COMMENT "Copying VST3 to user folder"
    )
endif()
