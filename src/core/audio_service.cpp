/**
 * @file    audio_service.cpp
 * @brief   Source file for the AudioService class.
 * @details This file contains the implementation of the AudioService class
 *          which owns a shared SDL audio playback stream for reusable
 *          one-shot sound effects.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-05
 */

#include "cbit/core/audio_service.hpp"

#include <algorithm>
#include <vector>

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_init.h>

#include "cbit/core/logger.hpp"

namespace cbit2d::core {
    namespace {
        constexpr SDL_AudioSpec kPlaybackSpec{
            SDL_AUDIO_F32,
            1,
            48000
        };

        SDL_AudioStream *g_audioStream = nullptr;
        std::vector<float> g_workingBuffer;
    } // namespace

    /**
     * @brief Initializes the shared audio playback stream.
     * @return `true` when the service is ready for playback, otherwise `false`.
     */
    bool AudioService::initialize() {
        if (g_audioStream != nullptr) {
            return true;
        }

        if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
            Logger::error("Couldn't initialize SDL audio subsystem: {}", SDL_GetError());
            return false;
        }

        g_audioStream = SDL_OpenAudioDeviceStream(
            SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
            &kPlaybackSpec,
            nullptr,
            nullptr);
        if (g_audioStream == nullptr) {
            Logger::error("Couldn't open audio playback stream: {}", SDL_GetError());
            return false;
        }

        SDL_SetAudioStreamGain(g_audioStream, 1.0F);
        if (!SDL_ResumeAudioStreamDevice(g_audioStream)) {
            Logger::error("Couldn't resume audio playback stream: {}", SDL_GetError());
            SDL_DestroyAudioStream(g_audioStream);
            g_audioStream = nullptr;
            return false;
        }

        return true;
    }

    /**
     * @brief Shuts down the shared audio playback stream.
     */
    void AudioService::shutdown() {
        if (g_audioStream == nullptr) {
            return;
        }

        if (SDL_WasInit(SDL_INIT_AUDIO) != 0) {
            SDL_DestroyAudioStream(g_audioStream);
        }

        g_audioStream = nullptr;
        g_workingBuffer.clear();
    }

    /**
     * @brief Returns whether the audio service is currently initialized.
     * @return `true` when playback is available, otherwise `false`.
     */
    bool AudioService::isInitialized() {
        return g_audioStream != nullptr;
    }

    /**
     * @brief Queues one short floating-point PCM sound effect for playback.
     * @param samples Mono floating-point samples in the range [-1, 1].
     * @param gain Optional gain multiplier applied before queueing.
     * @return `true` when the sound is queued successfully, otherwise `false`.
     */
    bool AudioService::playOneShot(const std::span<const float> samples, const float gain) {
        if (samples.empty()) {
            return false;
        }

        if (!initialize()) {
            return false;
        }

        g_workingBuffer.assign(samples.begin(), samples.end());
        if (gain != 1.0F) {
            for (float &sample: g_workingBuffer) {
                sample *= gain;
            }
        }

        if (!SDL_PutAudioStreamData(
            g_audioStream,
            g_workingBuffer.data(),
            static_cast<int>(g_workingBuffer.size() * sizeof(float)))) {
            Logger::error("Couldn't queue audio data: {}", SDL_GetError());
            return false;
        }

        return true;
    }
} // namespace cbit2d::core
