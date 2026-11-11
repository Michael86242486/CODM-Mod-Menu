#pragma once

#include <jni.h>

namespace Jni {
    void Init(JavaVM* vm);
    JNIEnv* GetEnv();
}