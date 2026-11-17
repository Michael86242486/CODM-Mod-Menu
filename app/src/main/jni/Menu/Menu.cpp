#include "Menu.hpp"
#include "../Includes/Logger.h"
#include "../Includes/Utils.hpp"

namespace Menu {

    void ShowToast(const char* message) {
        JNIEnv* env = Utils::GetEnv();
        if (!env) return;

        jclass toastClass = env->FindClass("android/widget/Toast");
        if (!toastClass) return;

        jstring msg = env->NewStringUTF(message);

        jmethodID makeText = env->GetStaticMethodID(toastClass, "makeText",
            "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");

        // Get application context
        jclass activityThread = env->FindClass("android/app/ActivityThread");
        if (!activityThread) {
            env->DeleteLocalRef(msg);
            env->DeleteLocalRef(toastClass);
            return;
        }

        jmethodID currentApp = env->GetStaticMethodID(activityThread, "currentApplication",
            "()Landroid/app/Application;");
        jobject context = env->CallStaticObjectMethod(activityThread, currentApp);

        if (context && makeText) {
            jobject toast = env->CallStaticObjectMethod(toastClass, makeText,
                context, msg, 0);
            if (toast) {
                jmethodID show = env->GetMethodID(toastClass, "show", "()V");
                if (show) {
                    env->CallVoidMethod(toast, show);
                }
                env->DeleteLocalRef(toast);
            }
            env->DeleteLocalRef(context);
        }

        env->DeleteLocalRef(msg);
        env->DeleteLocalRef(toastClass);
        env->DeleteLocalRef(activityThread);
    }

    void UpdateUI(int feature, bool enabled) {
        // Notify Java UI thread via JNI if needed
        LOGI("UI Update: feature %d = %s", feature, enabled ? "ON" : "OFF");
    }

} // namespace Menu