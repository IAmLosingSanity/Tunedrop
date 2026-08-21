#include "PitchDetector.h"
#include "YinPitchDetector.h"
#include "MpmPitchDetector.h"

PitchDetector::PitchDetector (PitchDetectorSettings settingsToUse, Algorithm algorithmToUse)
    : settings (settingsToUse), algorithm (algorithmToUse)
{
    impl = createAlgorithm (algorithm);
    impl->prepare (settings);
}

std::unique_ptr<PitchDetectorAlgorithm> PitchDetector::createAlgorithm (Algorithm algorithmToCreate)
{
    switch (algorithmToCreate)
    {
        case Algorithm::mpm:
            return std::make_unique<MpmPitchDetector>();

        case Algorithm::yin:
        default:
            return std::make_unique<YinPitchDetector>();
    }
}

void PitchDetector::setSettings (const PitchDetectorSettings& newSettings)
{
    settings = newSettings;
    impl->prepare (settings);
}

void PitchDetector::setAlgorithm (Algorithm newAlgorithm)
{
    if (newAlgorithm == algorithm && impl != nullptr)
        return;

    algorithm = newAlgorithm;
    impl = createAlgorithm (algorithm);
    impl->prepare (settings);
}

PitchResult PitchDetector::detectPitch (const float* samples, int numSamples)
{
    return impl->detectPitch (samples, numSamples);
}
