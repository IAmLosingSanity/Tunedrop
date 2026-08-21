# Console app running juce::UnitTest-based tests. Uses JUCE's own unit test
# framework (juce_core) rather than pulling in a separate testing library --
# it's already a dependency of the project and is sufficient for this scale.

juce_add_console_app(TunedropTests
    PRODUCT_NAME "TunedropTests")

target_sources(TunedropTests PRIVATE
    RunAllTests.cpp
    NoteUtilsTests.cpp
    ${CMAKE_SOURCE_DIR}/src/dsp/NoteUtils.cpp)

target_include_directories(TunedropTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(TunedropTests
    PRIVATE
        juce::juce_core
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
