#pragma once

#include <cstdint>

namespace ESP {
    void Init(uintptr_t moduleBase);
    void Run();

    struct EspEntity {
        uintptr_t address;
        float x, y, z;
        float distance;
        char name[64];
        int health;
        int team;
        bool isVisible;
        bool isDead;

        bool IsValid() const {
            return address != 0 && !isDead && health > 0;
        }
    };

    // World to screen projection
    bool WorldToScreen(float* v3, float* out, float* viewMatrix,
                       int screenW, int screenH);

    // Rendering (called from overlay thread)
    void DrawEsp(const EspEntity& entity);

    extern bool g_EspEnabled;
    extern float g_EspMaxDistance;
}