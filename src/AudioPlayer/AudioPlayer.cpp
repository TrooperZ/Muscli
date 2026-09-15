/**
 * @file AudioPlayer.cpp
 * @brief Local file playback using miniaudio.
 */
#include "AudioPlayer.h"

#include <stdexcept>

static void checkAudio(ma_result result, const char* message) {
    if (result != MA_SUCCESS) {
        throw std::runtime_error(std::string(message) + ": " +
                                 ma_result_description(result));
    }
}

AudioPlayer::AudioPlayer(const std::string& filepath,
                         ma_engine* externalEngine) {
    try {
        if (!externalEngine) {
            checkAudio(ma_engine_init(nullptr, &engine),
                       "Could not initialize audio output");
            engineReady = true;
            externalEngine = &engine;
        }
        checkAudio(ma_sound_init_from_file(
                       externalEngine, filepath.c_str(),
                       MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_NO_SPATIALIZATION,
                       nullptr, nullptr, &sound),
                   "Could not open audio file");
        soundReady = true;
        checkAudio(ma_sound_start(&sound), "Could not start playback");
    } catch (...) {
        if (soundReady) ma_sound_uninit(&sound);
        if (engineReady) ma_engine_uninit(&engine);
        throw;
    }
}

AudioPlayer::~AudioPlayer() {
    if (soundReady) ma_sound_uninit(&sound);
    if (engineReady) ma_engine_uninit(&engine);
}

void AudioPlayer::togglePause() {
    if (isPlaying() && !isFinished()) {
        checkAudio(ma_sound_stop(&sound), "Could not pause playback");
    } else {
        if (isFinished()) {
            checkAudio(ma_sound_stop(&sound),
                       "Could not stop finished playback");
        }
        // Starting a finished sound also rewinds it.
        checkAudio(ma_sound_start(&sound), "Could not resume playback");
    }
}

bool AudioPlayer::isPlaying() const { return ma_sound_is_playing(&sound); }
bool AudioPlayer::isFinished() const { return ma_sound_at_end(&sound); }

float AudioPlayer::getPosition() {
    float seconds = 0;
    checkAudio(ma_sound_get_cursor_in_seconds(&sound, &seconds),
               "Could not read playback position");
    return seconds;
}

float AudioPlayer::getDuration() {
    float seconds = 0;
    // Some decoders cannot provide a duration.
    if (ma_sound_get_length_in_seconds(&sound, &seconds) != MA_SUCCESS)
        return 0;
    return seconds;
}
