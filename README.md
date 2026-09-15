# Muscli
A command line music player written in C++ for macOS and Linux.

## Build and run

Requires CMake 3.16+, a C++17 compiler, and a UTF-8 terminal supporting ANSI
colors. Audio playback uses the system's default output device.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/muscli "tests/test-tone.mp3"
```

Pass a local MP3, WAV, or FLAC file. Space pauses/resumes playback and restarts a
finished track. `q` or Ctrl+C exits. The display shows the filename, playback
state, elapsed time, duration, and progress; small terminals get a compact view.
Filename display currently substitutes non-ASCII bytes with `?`; the original
path is still used to open the file. Duration displays `--:--` when unavailable.

There is no playlist, streaming-service integration, metadata extraction, or
album-art requirement yet. The existing artwork component remains available.

## Checks

```sh
ctest --test-dir build --output-on-failure
python3 tests/terminal_test.py
```

The CTest check decodes the included MP3 offline without an audio device and
checks non-silent samples, duration, pause/resume, completion, restart, and a
missing file. The Python terminal check requires a working audio device and
checks controls, resizing, errors, signals, and terminal restoration.
The build workflow runs the offline check on macOS and Ubuntu. Verify audible
output on each platform separately; an offline test does not test speakers.

## Test audio

`tests/test-tone.mp3` is a generated four-second, quiet 440 Hz tone with fades.
It contains no recorded music. To regenerate it with FFmpeg:

```sh
ffmpeg -f lavfi -i 'sine=frequency=440:duration=4:sample_rate=44100' \
  -af 'volume=0.15,afade=t=in:d=0.1,afade=t=out:st=3.5:d=0.5' \
  -codec:a libmp3lame -q:a 5 tests/test-tone.mp3
```

## Dependency

Audio uses vendored [miniaudio 0.11.22](https://github.com/mackron/miniaudio/tree/0.11.22).
Its license is in `src/miniaudio/LICENSE`. Building does not download dependencies.
