#include "ESP.hpp"
#include "../Includes/Logger.h"
#include "../Includes/Utils.hpp"
#include "../Includes/Offsets.h"
#include "../Includes/Macros.h"
#include <cmath>

namespace ESP {

    bool g_EspEnabled = false;
    float g_EspMaxDistance = 300.0f;
    static uintptr_t g_Base = 0;

    void Init(uintptr_t moduleBase) {
        g_Base = moduleBase;
        LOGI("ESP initialized at base 0x%lX", g_Base);
    }

    bool WorldToScreen(float* v3, float* out, float* viewMatrix,
                       int screenW, int screenH) {
        float w = viewMatrix[12] * v3[0] +
                  viewMatrix[13] * v3[1] +
                  viewMatrix[14] * v3[2] +
                  viewMatrix[15];

        if (w < 0.01f) return false;

        float x = viewMatrix[0] * v3[0] +
                  viewMatrix[1] * v3[1] +
                  viewMatrix[2] * v3[2] +
                  viewMatrix[3];

        float y = viewMatrix[4] * v3[0] +
                  viewMatrix[5] * v3[1] +
                  viewMatrix[6] * v3[2] +
                  viewMatrix[7];

        out[0] = (screenW / 2.0f) * (1.0f + x / w);
        out[1] = (screenH / 2.0f) * (1.0f - y / w);
        out[2] = w;

        return true;
    }

    void Run() {
        if (!g_Base) return;

        uintptr_t localPlayer = READ_QWORD(g_Base + Offsets::LOCAL_PLAYER);
        if (!localPlayer) return;

        float* localPos = (float*)(localPlayer + Offsets::POSITION_OFFSET);
        float* viewMatrix = (float*)(g_Base + Offsets::VIEW_MATRIX);
        int screenW = READ_DWORD(g_Base + Offsets::SCREEN_WIDTH);
        int screenH = READ_DWORD(g_Base + Offsets::SCREEN_HEIGHT);

        uintptr_t entityList = READ_QWORD(g_Base + Offsets::ENTITY_LIST);
        if (!entityList) return;

        for (int i = 0; i < 100; i++) {
            uintptr_t entity = READ_QWORD(entityList + i * sizeof(uintptr_t));
            if (!entity) continue;

            EspEntity ent;
            ent.address = entity;
            ent.health = READ_DWORD(entity + Offsets::HEALTH_OFFSET);
            ent.team = READ_DWORD(entity + Offsets::TEAM_ID);
            ent.isDead = READ_BYTE(entity + Offsets::IS_DEAD);
            ent.isVisible = READ_BYTE(entity + Offsets::IS_VISIBLE);

            if (!ent.IsValid()) continue;

            float* pos = (float*)(entity + Offsets::POSITION_OFFSET);
            ent.x = pos[0];
            ent.y = pos[1];
            ent.z = pos[2];

            // Calculate distance
            float dx = pos[0] - localPos[0];
            float dy = pos[1] - localPos[1];
            float dz = pos[2] - localPos[2];
            ent.distance = sqrtf(dx*dx + dy*dy + dz*dz);

            if (ent.distance > g_EspMaxDistance) continue;

            // World to screen
            float screen[3];
            if (WorldToScreen(pos, screen, viewMatrix, screenW, screenH)) {
                DrawEsp(ent);
            }
        }
    }

    void DrawEsp(const EspEntity& entity) {
        // In a real implementation, this would use OpenGL ES or
        // Android Canvas overlay drawing. This is a placeholder
        // that logs ESP data instead.
        LOGI("ESP: %s HP=%d D=%.1f [%.0f,%.0f,%.0f]",
             entity.name, entity.health, entity.distance,
             entity.x, entity.y, entity.z);
    }

} // namespace ESP