LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Menu
LOCAL_CPPFLAGS := -std=c++17 -O2 -Wall -fvisibility=hidden
LOCAL_LDFLAGS := -Wl,--no-undefined

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Includes

LOCAL_SRC_FILES := \
    Main.cpp \
    Menu/Menu.cpp \
    Menu/Jni.cpp \
    Hacks/ESP.cpp \
    Hacks/Aimbot.cpp \
    Hacks/Memory.cpp \
    Includes/Dobby/dummy.c

LOCAL_LDLIBS := -llog -landroid

include $(BUILD_SHARED_LIBRARY)