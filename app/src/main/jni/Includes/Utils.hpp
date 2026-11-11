#pragma once

#include <jni.h>
#include <cstdint>
#include <unistd.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <string>
#include <vector>

namespace Utils {

    inline JavaVM* gJvm = nullptr;
    inline JNIEnv* gEnv = nullptr;

    inline JNIEnv* GetEnv() {
        if (gEnv) return gEnv;
        if (gJvm) {
            gJvm->GetEnv((void**)&gEnv, JNI_VERSION_1_6);
            if (!gEnv) {
                gJvm->AttachCurrentThread(&gEnv, nullptr);
            }
        }
        return gEnv;
    }

    inline uintptr_t GetModuleBase(const char* moduleName) {
        FILE* fp = fopen("/proc/self/maps", "r");
        if (!fp) return 0;

        char line[512];
        uintptr_t base = 0;
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, moduleName) && strstr(line, "r-xp")) {
                base = strtoul(line, nullptr, 16);
                break;
            }
        }
        fclose(fp);
        return base;
    }

    inline uintptr_t FindPattern(uintptr_t base, const uint8_t* pattern, const char* mask) {
        auto size = 0x1000000; // 16MB scan region
        for (uintptr_t i = 0; i < size; i++) {
            bool found = true;
            for (size_t j = 0; mask[j]; j++) {
                if (mask[j] == 'x' && pattern[j] != *(uint8_t*)(base + i + j)) {
                    found = false;
                    break;
                }
            }
            if (found) return base + i;
        }
        return 0;
    }

    inline bool WriteMemory(void* addr, const void* data, size_t size) {
        size_t pageSize = sysconf(_SC_PAGESIZE);
        uintptr_t pageStart = (uintptr_t)addr & ~(pageSize - 1);

        if (mprotect((void*)pageStart, pageSize * 2, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
            return false;
        }
        memcpy(addr, data, size);
        return true;
    }

    inline bool HookFunction(void* target, void* detour, void** original) {
        // Simple trampoline hook (production would use Dobby or Substructure)
        uint8_t branch[4] = { 0x00, 0x00, 0x00, 0x14 }; // B <addr>
        int32_t offset = (int32_t)((uintptr_t)detour - (uintptr_t)target);
        branch[0] = offset & 0xFF;
        branch[1] = (offset >> 8) & 0xFF;
        branch[2] = (offset >> 16) & 0xFF;
        branch[3] = (offset >> 24) & 0xFF;

        if (original) {
            *original = malloc(4);
            memcpy(*original, target, 4);
        }

        return WriteMemory(target, branch, 4);
    }

} // namespace Utils