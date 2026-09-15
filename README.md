# Muscli
A command line music player written in C++.

## Build and run

Requires CMake 3.16+, a C++17 compiler, and a UTF-8 terminal on macOS or Linux.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/muscli tests/test-tone.mp3
```

Pass your own local MP3 with `./build/muscli "/path/to/song.mp3"`.
Space pauses/resumes or restarts a finished song, and `q` quits.

The existing 80x24 layout, album art, control positions, and input row remain.
Time and the seek bar reflect playback. The filename replaces the demo title;
artist metadata is not read, and the artwork remains the original demo image.
The S, previous, next, and L labels remain decorative.
Non-ASCII filename bytes display as `?`, without changing the path used to open
files. Smaller terminals display a resize prompt.

## Checks

```sh
ctest --test-dir build --output-on-failure
python3 tests/terminal_test.py
```

The offline audio test checks MP3 decoding and playback controls without a device.
The terminal test requires an audio device and checks the original UI elements,
controls, errors, resizing, and terminal restoration. Linux playback still needs
verification on a Linux desktop.

The included test MP3 is a generated four-second 440 Hz tone with fades.
Regenerate with FFmpeg:

```sh
ffmpeg -f lavfi -i 'sine=frequency=440:duration=4:sample_rate=44100' \
  -af 'volume=0.15,afade=t=in:d=0.1,afade=t=out:st=3.5:d=0.5' \
  -codec:a libmp3lame -q:a 5 tests/test-tone.mp3
```

Audio uses vendored miniaudio 0.11.22 with its license in `src/miniaudio/LICENSE`.
