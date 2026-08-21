#include <juce_core/juce_core.h>

// Runs every juce::UnitTest registered anywhere in the linked sources
// (each test file registers itself via a static instance -- see
// NoteUtilsTests.cpp for the pattern). Exits non-zero on any failure so
// this plugs cleanly into CI.
int main (int, char**)
{
    juce::UnitTestRunner runner;
    runner.runAllTests();

    for (int i = 0; i < runner.getNumResults(); ++i)
    {
        const auto* result = runner.getResult (i);

        if (result != nullptr && result->failures > 0)
            return 1;
    }

    return 0;
}
