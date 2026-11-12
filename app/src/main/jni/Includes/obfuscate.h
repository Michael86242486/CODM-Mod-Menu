#pragma once

// Simple XOR compile-time string obfuscation to evade static analysis
#include <cstdint>
#include <cstring>
#include <utility>

template <size_t N>
struct ObfuscatedString {
    char buffer[N];
    constexpr ObfuscatedString(const char (&str)[N]) : buffer{} {
        for (size_t i = 0; i < N; i++) {
            buffer[i] = str[i] ^ 0x5A;
        }
    }
    const char* decrypt() const {
        static char decrypted[N];
        for (size_t i = 0; i < N; i++) {
            decrypted[i] = buffer[i] ^ 0x5A;
        }
        return decrypted;
    }
};

#define OBFUSCATE(str) (ObfuscatedString(str).decrypt())

// Macro for hiding string constants
#define HIDE_STR(str) []() -> const char* { \
    static constexpr auto obf = ObfuscatedString(str); \
    return obf.decrypt(); \
}()