#pragma once

#include <cstdint>

namespace Aimbot {
    void Init(uintptr_t moduleBase);
    void Run();

    struct AimTarget {
        uintptr_t address;
        float distance;
        float angleDiff;
        float screenX, screenY;

        bool operator<(const AimTarget& other) const {
            return distance < other.distance;
        }
    };

    extern bool g_AimbotEnabled;
    extern float g_AimbotFov;
    extern float g_AimbotSmooth;
    extern bool g_AimbotAutoShoot;
}