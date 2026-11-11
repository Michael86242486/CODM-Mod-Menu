#pragma once

#include <cstdint>

namespace Memory {
    void Init(uintptr_t moduleBase);
    void Wallhack();
    void NoRecoil();
    void NoSpread();
    void MagicBullet();
    void RadarHack();
    void SpeedHack();
    void GodMode();
    void UnlimitedAmmo();

    extern bool g_WallhackEnabled;
    extern bool g_NoRecoilEnabled;
    extern bool g_NoSpreadEnabled;
    extern bool g_MagicBulletEnabled;
    extern bool g_RadarEnabled;
    extern bool g_SpeedEnabled;
    extern bool g_GodModeEnabled;
    extern bool g_UnlimitedAmmoEnabled;
    extern float g_SpeedMultiplier;
    extern int g_UnlimitedAmmoValue;
}