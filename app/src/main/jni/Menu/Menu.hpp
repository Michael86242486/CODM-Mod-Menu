#pragma once

#include <jni.h>

namespace Menu {
    void ShowToast(const char* message);
    void UpdateUI(int feature, bool enabled);
}