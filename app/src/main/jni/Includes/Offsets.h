#pragma once

#include <cstdint>

// ============================================================
// CODM (Call of Duty Mobile) libUE4.so offsets
// These MUST be updated for each game version
// ============================================================

namespace Offsets {

    // Player / Entity
    constexpr uintptr_t LOCAL_PLAYER    = 0x12345678;
    constexpr uintptr_t ENTITY_LIST     = 0x87654321;
    constexpr uintptr_t ENTITY_SIZE     = 0x1A0;
    constexpr uintptr_t HEALTH_OFFSET   = 0x1C8;
    constexpr uintptr_t ARMOR_OFFSET    = 0x1CC;
    constexpr uintptr_t POSITION_OFFSET = 0x1E0;
    constexpr uintptr_t VIEW_ANGLES     = 0x234;
    constexpr uintptr_t TEAM_ID         = 0x0F8;
    constexpr uintptr_t IS_VISIBLE      = 0x2A4;
    constexpr uintptr_t IS_DEAD         = 0x2B0;
    constexpr uintptr_t NAME_OFFSET     = 0x2C8;

    // Weapon
    constexpr uintptr_t CURRENT_WEAPON  = 0x3A0;
    constexpr uintptr_t WEAPON_AMMO     = 0x3C0;
    constexpr uintptr_t WEAPON_RESERVE  = 0x3C8;
    constexpr uintptr_t RECOIL_OFFSET   = 0x4D0;
    constexpr uintptr_t SPREAD_OFFSET   = 0x4D8;

    // Movement
    constexpr uintptr_t SPEED_OFFSET    = 0x4F0;
    constexpr uintptr_t JUMP_OFFSET     = 0x4F8;

    // Game globals
    constexpr uintptr_t GAME_INSTANCE   = 0xABCD1234;
    constexpr uintptr_t MINIMAP_PTR     = 0xDCBA4321;

    // Rendering / ESP
    constexpr uintptr_t CAMERA_MANAGER  = 0x5678ABCD;
    constexpr uintptr_t VIEW_MATRIX     = 0x5678ABD0;
    constexpr uintptr_t SCREEN_WIDTH    = 0xDEADBEEF;
    constexpr uintptr_t SCREEN_HEIGHT   = 0xDEADBEEF + 4;

} // namespace Offsets