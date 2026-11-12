#pragma once

#include <android/log.h>
#include <cstring>
#include <cstdio>

#define LOG_TAG "CODM-Menu"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

namespace Logger {
    inline void HexDump(const char* tag, const void* data, size_t len) {
        char buffer[512];
        size_t offset = 0;
        const uint8_t* bytes = static_cast<const uint8_t*>(data);
        for (size_t i = 0; i < len && offset < sizeof(buffer) - 4; i++) {
            offset += snprintf(buffer + offset, sizeof(buffer) - offset,
                               "%02X ", bytes[i]);
        }
        __android_log_print(ANDROID_LOG_DEBUG, tag, "%s", buffer);
    }
}