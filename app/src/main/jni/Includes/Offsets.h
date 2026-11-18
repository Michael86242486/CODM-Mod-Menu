#pragma once

#include <cstdint>

// ============================================================
// CODM (Call of Duty Mobile) libUE4.so offsets
// Verified for Version 1.0.49.0
// ============================================================

namespace Offsets {

    // Unreal Engine Globals (Version 1.0.49.0)
    constexpr uintptr_t G_WORLD           = 0x13F4B8C0;
    constexpr uintptr_t G_NAMES           = 0x1308A4B8;
    constexpr uintptr_t U_WORLD           = 0x13F5C6E0;

    // Unreal Engine Property Offsets (Version 1.0.49.0)
    constexpr uintptr_t PERSISTENT_LEVEL  = 0x30;
    constexpr uintptr_t LOCAL_PLAYERS     = 0x38;
    constexpr uintptr_t PLAYER_CONTROLLER = 0x30;
    constexpr uintptr_t ACKNOWLEDGED_PAWN = 0x248;
    constexpr uintptr_t ROOT_COMPONENT    = 0x150;

    // Player / Entity Mapping
    constexpr uintptr_t LOCAL_PLAYER    = 0x13F4B8C0; // Mapped to GWorld base
    constexpr uintptr_t ENTITY_LIST     = 0x98;       // UE4 Actor array offset inside PersistentLevel
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
