#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "../src/AudioPlayer/AudioPlayer.h"

static void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

int main(int argc, char* argv[]) {
    if (argc != 2) return 1;
    ma_engine engine{};
    ma_engine_config config = ma_engine_config_init();
    config.noDevice = MA_TRUE;
    config.channels = 2;
    config.sampleRate = 44100;
    // Decode synchronously for deterministic offline rendering.
    ma_resource_manager manager{};
    ma_resource_manager_config resources = ma_resource_manager_config_init();
    resources.jobThreadCount = 0;
    resources.flags = MA_RESOURCE_MANAGER_FLAG_NO_THREADING;
    if (ma_resource_manager_init(&resources, &manager) != MA_SUCCESS) return 1;
    config.pResourceManager = &manager;
    if (ma_engine_init(&config, &engine) != MA_SUCCESS) {
        ma_resource_manager_uninit(&manager);
        return 1;
    }
    int result = 0;
    try {
        AudioPlayer player(argv[1], &engine);
        require(player.isPlaying(), "Playback did not start");
        require(player.getDuration() > 3.9f && player.getDuration() < 4.2f,
                "Unexpected MP3 duration");
        std::vector<float> samples(512 * 2);
        auto render = [&]() {
            ma_job job;
            while (ma_resource_manager_next_job(&manager, &job) == MA_SUCCESS) {
                ma_job_process(&job);
            }
            require(ma_engine_read_pcm_frames(&engine, samples.data(), 512,
                                              nullptr) == MA_SUCCESS,
                    "Offline rendering failed");
        };
        bool audible = false;
        for (int i = 0; i < 50; ++i) {
            render();
            for (float sample : samples) audible |= std::abs(sample) > 0.0001f;
        }
        require(audible, "MP3 decoded to silence");
        require(player.getPosition() > 0, "Playback cursor did not advance");
        player.togglePause();
        float paused = player.getPosition();
        for (int i = 0; i < 10; ++i) render();
        require(!player.isPlaying() && player.getPosition() == paused,
                "Paused playback advanced");
        player.togglePause();
        for (int i = 0; i < 500 && !player.isFinished(); ++i) render();
        require(player.isFinished(), "Playback did not finish");
        player.togglePause();
        for (int i = 0; i < 4; ++i) render();
        require(player.isPlaying() && !player.isFinished() &&
                    player.getPosition() < 0.1f,
                "Finished track did not restart");
        bool rejected = false;
        try {
            AudioPlayer missing(std::string(argv[1]) + ".missing", &engine);
        } catch (const std::exception&) {
            rejected = true;
        }
        require(rejected, "Missing audio was accepted");
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        result = 1;
    }
    ma_engine_uninit(&engine);
    ma_resource_manager_uninit(&manager);
    return result;
}
