#include "Jni.hpp"
#include "../Includes/Logger.h"

static JavaVM* g_Jvm = nullptr;

namespace Jni {

    void Init(JavaVM* vm) {
        g_Jvm = vm;
        LOGI("JNI Helper initialized");
    }

    JNIEnv* GetEnv() {
        JNIEnv* env = nullptr;
        if (g_Jvm) {
            g_Jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
            if (!env) {
                g_Jvm->AttachCurrentThread(&env, nullptr);
            }
        }
        return env;
    }

} // namespace Jni