#include <jni.h>
#include <thread>
#include <chrono>
#include <vector>
#include "Includes/Logger.h"
#include "Includes/Utils.hpp"
#include "Includes/Macros.h"
#include "Menu/Menu.hpp"
#include "Menu/Jni.hpp"
#include "Hacks/ESP.hpp"
#include "Hacks/Aimbot.hpp"
#include "Hacks/Memory.hpp"

// Global feature toggle array
bool g_Features[NUM_FEATURES] = { false };

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    Utils::gJvm = vm;
    JNIEnv* env;
    vm->GetEnv((void**)&env, JNI_VERSION_1_6);
    Utils::gEnv = env;

    LOGI("JNI_OnLoad: Mod Menu loaded successfully");

    // Initialize memory scanning on a background thread
    std::thread([]() {
        // Wait for libUE4.so to be loaded
        uintptr_t ue4base = 0;
        while (ue4base == 0) {
            ue4base = Utils::GetModuleBase("libUE4.so");
            if (ue4base == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        LOGI("libUE4.so found at 0x%lX", ue4base);

        Memory::Init(ue4base);
        Aimbot::Init(ue4base);
        ESP::Init(ue4base);

        // Main hack loop
        while (true) {
            if (g_Features[FEATURE_AIMBOT])      Aimbot::Run();
            if (g_Features[FEATURE_ESP])          ESP::Run();
            if (g_Features[FEATURE_WALLHACK])     Memory::Wallhack();
            if (g_Features[FEATURE_NO_RECOIL])    Memory::NoRecoil();
            if (g_Features[FEATURE_NO_SPREAD])    Memory::NoSpread();
            if (g_Features[FEATURE_MAGIC_BULLET]) Memory::MagicBullet();
            if (g_Features[FEATURE_RADAR])        Memory::RadarHack();
            if (g_Features[FEATURE_SPEEDHACK])    Memory::SpeedHack();
            if (g_Features[FEATURE_GOD_MODE])     Memory::GodMode();
            if (g_Features[FEATURE_UNLIMITED_AMMO]) Memory::UnlimitedAmmo();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }).detach();

    return JNI_VERSION_1_6;
}

// Called from Java overlay switches
extern "C" JNIEXPORT void JNICALL
Java_com_codm_menu_OverlayView_NativeToggle(JNIEnv* env, jclass clazz,
                                             jint feature, jboolean enabled) {
    if (feature >= 0 && feature < NUM_FEATURES) {
        g_Features[feature] = enabled;
        LOGI("Feature %d toggled: %s", feature, enabled ? "ON" : "OFF");
    }
}