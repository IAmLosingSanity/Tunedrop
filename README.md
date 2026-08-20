# Tunedrop

VST3 chromatic guitar tuner with alternate-tuning presets. See
`docs/PROJECT.md` for the full spec.

## Linux dev build

System dependencies (Ubuntu/Debian):

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build git \
  libasound2-dev libjack-jackd2-dev \
  libcurl4-openssl-dev \
  libfreetype-dev libfontconfig1-dev \
  libx11-dev libxcomposite-dev libxcursor-dev libxext-dev \
  libxinerama-dev libxrandr-dev libxrender-dev \
  libglu1-mesa-dev mesa-common-dev
```

Clone with submodules (JUCE is pinned as a submodule under `external/JUCE`):

```bash
git clone --recurse-submodules <repo-url>
cd Tunedrop
```

Build:

```bash
cmake -B build -G Ninja
cmake --build build
```

Artifacts land under:

```
build/Tunedrop_artefacts/Debug/VST3/Tunedrop.vst3
build/Tunedrop_artefacts/Debug/Standalone/Tunedrop
```

Test the VST3 in a Linux host (e.g. Carla) or just run the Standalone
binary directly.

## Windows release build

Handled by CI (`.github/workflows/windows-release.yml`) on a
`windows-latest` GitHub Actions runner -- no local Windows toolchain
required. Every push to `main` produces downloadable `Tunedrop.vst3` and
Standalone `.exe` artifacts under the workflow run's "Artifacts" section.

## Updating the pinned JUCE version

```bash
cd external/JUCE
git fetch --tags
git checkout <new-tag>
cd ../..
git add external/JUCE
git commit -m "Bump JUCE to <new-tag>"
```
