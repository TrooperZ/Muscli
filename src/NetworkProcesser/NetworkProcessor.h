/**
 * @file NetworkProcessor.h
 * @author Amin Karic
 * @brief Network processor class header file
 * @version 0.1
 * @date 2025-12-08
 *
 * @copyright Copyright (c) 2025
 *
 * @details The NetworkProcessor class provides functionalities for handling
 * network requests within the music player such as downloading album art,
 * fetching metadata, and streaming audio content.
 *
 */

#pragma once
#include <string>

/**
 * @brief NetworkProcessor class
 *
 */
class NetworkProcessor {
   public:
    NetworkProcessor();
    ~NetworkProcessor();
    void downloadImageToMemory(const std::string& url, char* buffer,
                               size_t bufferSize);
							   
							   

    // Future methods for network processing can be added here
};