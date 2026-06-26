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
    AUv3                OFF
    AAX                 OFF
)

# ============================================================================
# Source Files
# ============================================================================

target_sources(Zenith PRIVATE
    # Core
    Source/Core/PluginProcessor.cpp
    Source/Core/PluginProcessor.h
    Source/Core/PluginEditor.cpp
    Source/Core/PluginEditor.h

    # Synth
    Source/Synth/SynthVoice.cpp
    Source/Synth/SynthVoice.h
    Source/Synth/SynthSound.cpp
    Source/Synth/SynthSound.h

    # DSP Core
    Source/DSP/Core/Effect.cpp
    Source/DSP/Core/Effect.h
    Source/DSP/Core/EffectRack.cpp
    Source/DSP/Core/EffectRack.h
    Source/DSP/Core/EffectFactory.cpp
    Source/DSP/Core/EffectFactory.h

    # DSP Effects — Dynamics
    Source/DSP/Core/Effects/Dynamics/Dynamics.cpp
    Source/DSP/Core/Effects/Dynamics/Dynamics.h
    Source/DSP/Core/Effects/Dynamics/DynamicsExtra.cpp
    Source/DSP/Core/Effects/Dynamics/DynamicsExtra.h

    # DSP Effects — EQ
    Source/DSP/Core/Effects/EQ/EQ.cpp
    Source/DSP/Core/Effects/EQ/EQ.h
    Source/DSP/Core/Effects/EQ/EQExtra.cpp
    Source/DSP/Core/Effects/EQ/EQExtra.h

    # DSP Effects — Delay
    Source/DSP/Core/Effects/Delay/Delay.cpp
    Source/DSP/Core/Effects/Delay/Delay.h
    Source/DSP/Core/Effects/Delay/DelayExtra.cpp
    Source/DSP/Core/Effects/Delay/DelayExtra.h

    # DSP Effects — Reverb
    Source/DSP/Core/Effects/Reverb/Reverb.cpp
    Source/DSP/Core/Effects/Reverb/Reverb.h
    Source/DSP/Core/Effects/Reverb/ReverbExtra.cpp
    Source/DSP/Core/Effects/Reverb/ReverbExtra.h

    # DSP Effects — Modulation
    Source/DSP/Core/Effects/Modulation/Modulation.cpp
    Source/DSP/Core/Effects/Modulation/Modulation.h
    Source/DSP/Core/Effects/Modulation/ModulationExtra.cpp
    Source/DSP/Core/Effects/Modulation/ModulationExtra.h

    # DSP Effects — Distortion
    Source/DSP/Core/Effects/Distortion/Distortion.cpp
    Source/DSP/Core/Effects/Distortion/Distortion.h
    Source/DSP/Core/Effects/Distortion/DistortionExtra.cpp
    Source/DSP/Core/Effects/Distortion/DistortionExtra.h

    # DSP Effects — Spatial
    Source/DSP/Core/Effects/Spatial/Spatial.cpp
    Source/DSP/Core/Effects/Spatial/Spatial.h
    Source/DSP/Core/Effects/Spatial/SpatialExtra.cpp
    Source/DSP/Core/Effects/Spatial/SpatialExtra.h

    # DSP Effects — Creative
    Source/DSP/Core/Effects/Creative/Creative.cpp
    Source/DSP/Core/Effects/Creative/Creative.h
    Source/DSP/Core/Effects/Creative/CreativeExtra.cpp
    Source/DSP/Core/Effects/Creative/CreativeExtra.h

    # DSP Effects — MIDI
    Source/DSP/Core/Effects/MIDI/MIDI.cpp
    Source/DSP/Core/Effects/MIDI/MIDI.h
    Source/DSP/Core/Effects/MIDI/MIDIExtra.cpp
    Source/DSP/Core/Effects/MIDI/MIDIExtra.h

    # DSP Effects — Utility
    Source/DSP/Core/Effects/Utility/Utility.cpp
    Source/DSP/Core/Effects/Utility/Utility.h
    Source/DSP/Core/Effects/Utility/UtilityExtra.cpp
    Source/DSP/Core/Effects/Utility/UtilityExtra.h

    # UI
    Source/UI/Knob.cpp
    Source/UI/Knob.h
    Source/UI/LookAndFeel.cpp
    Source/UI/LookAndFeel.h
    Source/UI/EffectPanel.cpp
    Source/UI/EffectPanel.h

    # Utils
    Source/Utils/MathHelpers.h
    Source/Utils/ParameterHelpers.h
)

# ============================================================================
# Include Directories — FIXED (JUCE modules added)
# ============================================================================

target_include_directories(Zenith PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/JUCE/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Synth
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/DSP
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/DSP/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/DSP/Core/Effects
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utils
)

# ============================================================================
# Resources
# ============================================================================

# No binary data yet — skip to avoid errors
# juce_add_binary_data(Zenith_BinaryData
#     Resources/Fonts/
#     Resources/Presets/
# )
# target_sources(Zenith PRIVATE ${Zenith_BinaryData})

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
# Post-build (Windows: copy to VST3 folder)
# ============================================================================

if (WIN32 AND EXISTS "$ENV{USERPROFILE}/Documents/VST3")
    add_custom_command(TARGET Zenith POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Zenith>
            "$ENV{USERPROFILE}/Documents/VST3/"
        COMMENT "Copying VST3 to user folder"
    )
endif()
