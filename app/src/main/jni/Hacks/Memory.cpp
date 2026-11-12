#include "Memory.hpp"
#include "../Includes/Logger.h"
#include "../Includes/Utils.hpp"
#include "../Includes/Offsets.h"
#include "../Includes/Macros.h"

namespace Memory {

    bool g_WallhackEnabled = false;
    bool g_NoRecoilEnabled = false;
    bool g_NoSpreadEnabled = false;
    bool g_MagicBulletEnabled = false;
    bool g_RadarEnabled = false;
    bool g_SpeedEnabled = false;
    bool g_GodModeEnabled = false;
    bool g_UnlimitedAmmoEnabled = false;
    float g_SpeedMultiplier = 2.0f;
    int g_UnlimitedAmmoValue = 999;

    static uintptr_t g_Base = 0;

    void Init(uintptr_t moduleBase) {
        g_Base = moduleBase;
        LOGI("Memory hacks initialized");
    }

    void Wallhack() {
        if (!g_Base || !g_WallhackEnabled) return;

        uintptr_t entityList = READ_QWORD(g_Base + Offsets::ENTITY_LIST);
        if (!entityList) return;

        for (int i = 0; i < 100; i++) {
            uintptr_t entity = READ_QWORD(entityList + i * sizeof(uintptr_t));
            if (!entity) continue;

            // Set visibility to always visible (chams/glow)
            WRITE_BYTE(entity + Offsets::IS_VISIBLE, 1);
        }
    }

    void NoRecoil() {
        if (!g_Base || !g_NoRecoilEnabled) return;

        uintptr_t localPlayer = READ_QWORD(g_Base + Offsets::LOCAL_PLAYER);
        if (!localPlayer) return;

        // Zero out recoil values
        WRITE_FLOAT(localPlayer + Offsets::RECOIL_OFFSET, 0.0f);
        WRITE_FLOAT(localPlayer + Offsets::RECOIL_OFFSET + 4, 0.0f);
        WRITE_FLOAT(localPlayer + Offsets::RECOIL_OFFSET + 8, 0.0f);
    }

    void NoSpread() {
        if (!g_Base || !g_NoSpreadEnabled) return;

        uintptr_t localPlayer = READ_QWORD(g_Base + Offsets::LOCAL_PLAYER);
        if (!localPlayer) return;

        // Zero out spread values
        WRITE_FLOAT(localPlayer + Offsets::SPREAD_OFFSET, 0.0f);
        WRITE_FLOAT(localPlayer + Offsets::SPREAD_OFFSET + 4, 0.0f);
    }

    void MagicBullet() {
        if (!g_Base || !g_MagicBulletEnabled) return;

        uintptr_t localPlayer = READ_QWORD(g_Base + Offsets::LOCAL_PLAYER);
        if (!localPlayer) return;

        uintptr_t weapon = READ_QWORD(localPlayer + Offsets::CURRENT_WEAPON);
        if (!weapon) return;

        // Set bullet trace to always hit nearest enemy
        // This would redirect bullet traces to target entities
        // Implementation depends on game engine internals
        uintptr_t entityList = READ_QWORD(g_Base + Offsets::ENTITY_LIST);
        if (!entityList) return;

        // Find closest enemy for bullet redirection
        float* localPos = (float*)(localPlayer + Offsets::POSITION_OFFSET);
        float closestDist = 999999.0f;
        uintptr_t closestTarget = 0;

        for (int i = 0; i < 100; i++) {
            uintptr_t entity = READ_QWORD(entityList + i * sizeof(uintptr_t));
            if (!entity) continue;

            int health = READ_DWORD(entity + Offsets::HEALTH_OFFSET);
            if (health <= 0) continue;

            int team = READ_DWORD(entity + Offsets::TEAM_ID);
            int localTeam = READ_DWORD(localPlayer + Offsets::TEAM_ID);
            if (team == localTeam) continue;

            float* pos = (float*)(entity + Offsets::POSITION_OFFSET);
            float dx = pos[0] - localPos[0];
            float dy = pos[1] - localPos[1];
            float dz = pos[2] - localPos[2];
            float dist = dx*dx + dy*dy + dz*dz;

            if (dist < closestDist) {
                closestDist = dist;
                closestTarget = entity;
            }
        }

        if (closestTarget) {
            // Redirect bullet trace to closest target
            // (Offset needs to be found per game version)
            float* targetPos = (float*)(closestTarget + Offsets::POSITION_OFFSET);
            // WRITE_VECTOR3(weapon + BULLET_TARGET_OFFSET, targetPos);
        }
    }

    void RadarHack() {
        if (!g_Base || !g_RadarEnabled) return;

        // Show all enemies on minimap
        uintptr_t minimap = READ_QWORD(g_Base + Offsets::MINIMAP_PTR);
        if (!minimap) return;

        uintptr_t entityList = READ_QWORD(g_Base + Offsets::ENTITY_LIST);
        if (!entityList) return;

        uintptr_t localPlayer = READ_QWORD(g_Base + Offsets::LOCAL_PLAYER);
        if (!localPlayer) return;

        int localTeam = READ_DWORD(localPlayer + Offsets::TEAM_ID);

        for (int i = 0; i < 100; i++) {
            uintptr_t entity = READ_QWORD(entityList + i * sizeof(uintptr_t));
            if (!entity) continue;

            int team = READ_DWORD(entity + Offsets::TEAM_ID);
            if (team == localTeam) continue;

            // Set entity as revealed on minimap
            // (Offset needs to be found per game version)
            // WRITE_BYTE(entity + MINIMAP_REVEAL_OFFSET, 1);
        }
    }

    void SpeedHack() {
        if (!g_Base || !g_SpeedEnabled) return;

        uintptr_t localPlayer = READ_QWORD(g_Base + Offsets::LOCAL_PLAYER);
        if (!localPlayer) return;

        // Apply speed multiplier
        float baseSpeed = READ_FLOAT(localPlayer + Offsets::SPEED_OFFSET);
        WRITE_FLOAT(localPlayer + Offsets::SPEED_OFFSET,
                    baseSpeed * g_SpeedMultiplier);
    }

    void GodMode() {
        if (!g_Base || !g_GodModeEnabled) return;

        uintptr_t localPlayer = READ_QWORD(g_Base + Offsets::LOCAL_PLAYER);
        if (!localPlayer) return;

        // Lock health at max
        WRITE_DWORD(localPlayer + Offsets::HEALTH_OFFSET, 100);
        WRITE_DWORD(localPlayer + Offsets::ARMOR_OFFSET, 100);

        // Set damage multiplier to 0 for incoming damage
        // (Offset needs to be found per game version)
        // WRITE_FLOAT(localPlayer + DAMAGE_MULTIPLIER_OFFSET, 0.0f);
    }

    void UnlimitedAmmo() {
        if (!g_Base || !g_UnlimitedAmmoEnabled) return;

        uintptr_t localPlayer = READ_QWORD(g_Base + Offsets::LOCAL_PLAYER);
        if (!localPlayer) return;

        uintptr_t weapon = READ_QWORD(localPlayer + Offsets::CURRENT_WEAPON);
        if (!weapon) return;

        // Set ammo to max
        WRITE_DWORD(weapon + Offsets::WEAPON_AMMO, g_UnlimitedAmmoValue);
        WRITE_DWORD(weapon + Offsets::WEAPON_RESERVE, g_UnlimitedAmmoValue);
    }

} // namespace Memory