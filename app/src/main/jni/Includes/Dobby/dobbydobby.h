#pragma once

// Dobby lightweight hooking library header stub
// In production, include the real Dobby header from:
// https://github.com/jmpews/Dobby

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* DobbyInst_t;

DobbyInst_t DobbyHook(void* target, void* replacement, void** origin);
int DobbyUnhook(DobbyInst_t inst);
void* DobbySymbolResolver(const char* imageName, const char* symbolName);
int DobbyPatch(void* address, uint8_t* buffer, uint32_t size);

#ifdef __cplusplus
}
#endif