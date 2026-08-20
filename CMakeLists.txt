cmake_minimum_required(VERSION 3.22)

project(Tunedrop VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/JUCE)

juce_add_plugin(Tunedrop
    COMPANY_NAME "YourName"
    PLUGIN_MANUFACTURER_CODE Tund
    PLUGIN_CODE Tun1
    FORMATS VST3 Standalone
    PRODUCT_NAME "Tunedrop"
    VST3_CATEGORIES Fx Tools
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE)

target_sources(Tunedrop PRIVATE
    src/plugin/PluginProcessor.cpp
    src/plugin/PluginEditor.cpp)

target_include_directories(Tunedrop PRIVATE src)

# Trim features we don't need in Milestone 0 -- cuts build time and
# removes dependencies (e.g. webkit2gtk) that a tuner has no use for.
target_compile_definitions(Tunedrop PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0)

target_link_libraries(Tunedrop
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
