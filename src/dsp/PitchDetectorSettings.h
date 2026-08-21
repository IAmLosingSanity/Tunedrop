#pragma once

// Shared configuration for any pitch-detection algorithm. Both YIN and MPM
// operate over a fixed-size analysis window and produce a lag (tau) value
// converted to a frequency -- the window-size-vs-lowest-detectable-frequency
// tradeoff applies equally to both (bigger window = can detect lower notes,
// costs more compute and adds latency before a result is available).
// Algorithm choice mainly changes robustness (octave errors, noise
// handling), not this fundamental tradeoff.
struct PitchDetectorSettings
{
    double sampleRate = 44100.0;
    int windowSize = 2048;
    double minFrequencyHz = 70.0;   // below standard low E (~82.4 Hz) with margin
    double maxFrequencyHz = 1400.0; // comfortably above guitar's fundamental range

    // YIN's "d'" absolute threshold (de Cheveigne & Kawahara, 2002).
    // Lower = stricter, fewer false positives on noisy input.
    float yinThreshold = 0.15f;

    // MPM's clarity threshold (McLeod & Wyvill, 2005): among NSDF peaks,
    // pick the smallest-lag peak whose value is at least this fraction of
    // the global peak. Higher = stricter, fewer octave errors but more
    // likely to miss quiet/noisy signals.
    float mpmClarityThreshold = 0.93f;
};
