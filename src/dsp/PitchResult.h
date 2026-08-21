#pragma once

// Result of a single pitch-detection pass over one analysis window.
// valid == false means "no reliable pitch found" -- callers (UI, tuning
// logic) must treat this as silence/noise/unvoiced input, never fall back
// to a stale or guessed frequency (see PROJECT.md section 20).
struct PitchResult
{
    bool valid = false;
    double frequencyHz = 0.0;
    float confidence = 0.0f; // 0..1, higher = more reliable
};
