/**
 * @file    audio_service.hpp
 * @brief   Header file for the AudioService class.
 * @details This file contains the definition of the AudioService class which
 *          provides reusable one-shot audio playback for games built on
 *          Cbit2D.
 * @author  Nur Akmal bin Jalil
 * @date    2026-04-05
 */

#pragma once

#include <span>

namespace cbit2d::core {

/**
 * @brief Provides reusable engine-level one-shot audio playback.
 * @details This service owns a shared playback stream and exposes a small API
 *          for short non-positional sound effects. Games can queue procedural
 *          or preloaded sample buffers without managing SDL audio objects
 *          directly.
 */
class AudioService {
public:
    /**
     * @brief Initializes the shared audio playback stream.
     * @return `true` when the service is ready for playback, otherwise `false`.
     */
    static bool initialize();

    /**
     * @brief Shuts down the shared audio playback stream.
     */
    static void shutdown();

    /**
     * @brief Returns whether the audio service is currently initialized.
     * @return `true` when playback is available, otherwise `false`.
     */
    static bool isInitialized();

    /**
     * @brief Queues one short floating-point PCM sound effect for playback.
     * @param samples Mono floating-point samples in the range [-1, 1].
     * @param gain Optional gain multiplier applied before queueing.
     * @return `true` when the sound is queued successfully, otherwise `false`.
     */
    static bool playOneShot(std::span<const float> samples, float gain = 1.0F);
};

} // namespace cbit2d::core
