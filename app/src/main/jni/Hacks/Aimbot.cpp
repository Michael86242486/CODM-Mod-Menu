#include "Aimbot.hpp"
#include "../Includes/Logger.h"
#include "../Includes/Utils.hpp"
#include "../Includes/Offsets.h"
#include "../Includes/Macros.h"
#include <cmath>
#include <algorithm>
#include <vector>

namespace Aimbot {

    bool g_AimbotEnabled = false;
    float g_AimbotFov = 10.0f;
    float g_AimbotSmooth = 5.0f;
    bool g_AimbotAutoShoot = false;
    static uintptr_t g_Base = 0;

    void Init(uintptr_t moduleBase) {
        g_Base = moduleBase;
        LOGI("Aimbot initialized");
    }

    static float CalculateAngleDiff(float* currentAngles, float* targetAngles) {
        float dx = targetAngles[0] - currentAngles[0];
        float dy = targetAngles[1] - currentAngles[1];
        return sqrtf(dx*dx + dy*dy);
    }

    void Run() {
        if (!g_Base || !g_AimbotEnabled) return;

        uintptr_t localPlayer = READ_QWORD(g_Base + Offsets::LOCAL_PLAYER);
        if (!localPlayer) return;

        float* localPos = (float*)(localPlayer + Offsets::POSITION_OFFSET);
        float* viewAngles = (float*)(localPlayer + Offsets::VIEW_ANGLES);

        uintptr_t entityList = READ_QWORD(g_Base + Offsets::ENTITY_LIST);
        if (!entityList) return;

        std::vector<AimTarget> targets;

        for (int i = 0; i < 100; i++) {
            uintptr_t entity = READ_QWORD(entityList + i * sizeof(uintptr_t));
            if (!entity) continue;

            int health = READ_DWORD(entity + Offsets::HEALTH_OFFSET);
            int team = READ_DWORD(entity + Offsets::TEAM_ID);
            bool dead = READ_BYTE(entity + Offsets::IS_DEAD);

            if (dead || health <= 0) continue;

            // Skip own team
            int localTeam = READ_DWORD(localPlayer + Offsets::TEAM_ID);
            if (team == localTeam) continue;

            float* pos = (float*)(entity + Offsets::POSITION_OFFSET);

            // Calculate angle to target
            float dx = pos[0] - localPos[0];
            float dy = pos[1] - localPos[1];
            float dz = pos[2] - localPos[2];
            float distance = sqrtf(dx*dx + dy*dy + dz*dz);

            // Aim angles (yaw, pitch)
            float targetAngles[2];
            targetAngles[0] = atan2f(dy, dx) * 180.0f / M_PI;   // yaw
            targetAngles[1] = atan2f(-dz, distance) * 180.0f / M_PI; // pitch

            float angleDiff = CalculateAngleDiff(viewAngles, targetAngles);
            if (angleDiff > g_AimbotFov) continue;

            // World to screen
            float* viewMatrix = (float*)(g_Base + Offsets::VIEW_MATRIX);
            int screenW = READ_DWORD(g_Base + Offsets::SCREEN_WIDTH);
            int screenH = READ_DWORD(g_Base + Offsets::SCREEN_HEIGHT);
            float screen[3];

            float w = viewMatrix[12] * pos[0] +
                      viewMatrix[13] * pos[1] +
                      viewMatrix[14] * pos[2] +
                      viewMatrix[15];
            if (w < 0.01f) continue;

            float x = viewMatrix[0] * pos[0] +
                      viewMatrix[1] * pos[1] +
                      viewMatrix[2] * pos[2] +
                      viewMatrix[3];

            float y = viewMatrix[4] * pos[0] +
                      viewMatrix[5] * pos[1] +
                      viewMatrix[6] * pos[2] +
                      viewMatrix[7];

            screen[0] = (screenW / 2.0f) * (1.0f + x / w);
            screen[1] = (screenH / 2.0f) * (1.0f - y / w);

            targets.push_back({
                entity, distance, angleDiff, screen[0], screen[1]
            });
        }

        if (targets.empty()) return;

        // Sort by angle difference (closest to crosshair first)
        std::sort(targets.begin(), targets.end(),
            [](const AimTarget& a, const AimTarget& b) {
                return a.distance < b.distance;
            });

        // Use closest target
        AimTarget& best = targets[0];

        // Calculate smooth aim step
        float* targetPos = (float*)(best.address + Offsets::POSITION_OFFSET);
        float dx = targetPos[0] - localPos[0];
        float dy = targetPos[1] - localPos[1];
        float dz = targetPos[2] - localPos[2];
        float dist = sqrtf(dx*dx + dy*dy + dz*dz);

        float targetYaw = atan2f(dy, dx) * 180.0f / M_PI;
        float targetPitch = atan2f(-dz, dist) * 180.0f / M_PI;

        // Smoothly interpolate
        float smoothFactor = 1.0f / g_AimbotSmooth;
        float newYaw = viewAngles[0] + (targetYaw - viewAngles[0]) * smoothFactor;
        float newPitch = viewAngles[1] + (targetPitch - viewAngles[1]) * smoothFactor;

        // Write new angles
        WRITE_FLOAT(localPlayer + Offsets::VIEW_ANGLES, newYaw);
        WRITE_FLOAT(localPlayer + Offsets::VIEW_ANGLES + 4, newPitch);

        // Auto shoot if enabled and aiming at target
        if (g_AimbotAutoShoot && best.angleDiff < 3.0f) {
            // Trigger fire (would need fire offset)
            // WRITE_BYTE(localPlayer + FIRE_OFFSET, 1);
        }
    }

} // namespace Aimbot