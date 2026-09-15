/**
 * @file AudioPlayer.h
 * @brief Owns playback of one local audio file
 */
#pragma once

#include <string>

#include "../miniaudio/miniaudio.h"

class AudioPlayer {
   private:
    ma_engine engine{};
    ma_sound sound{};
    bool engineReady = false;
    bool soundReady = false;

   public:
    // An external engine is used by the offline playback check
    explicit AudioPlayer(const std::string& filepath,
                         ma_engine* externalEngine = nullptr);
    ~AudioPlayer();
    AudioPlayer(const AudioPlayer&) = delete;
    AudioPlayer& operator=(const AudioPlayer&) = delete;

    void togglePause();
    bool isPlaying() const;
    bool isFinished() const;
    float getPosition();
    float getDuration();
};
