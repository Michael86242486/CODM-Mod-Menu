#pragma once

// Feature indices (must match Java switch IDs order)
#define FEATURE_AIMBOT        0
#define FEATURE_ESP            1
#define FEATURE_WALLHACK      2
#define FEATURE_NO_RECOIL     3
#define FEATURE_NO_SPREAD     4
#define FEATURE_MAGIC_BULLET  5
#define FEATURE_RADAR         6
#define FEATURE_SPEEDHACK     7
#define FEATURE_GOD_MODE      8
#define FEATURE_UNLIMITED_AMMO 9

#define NUM_FEATURES 10

// Inline hooks
#define HOOK(orig, hook)   Utils::HookFunction((void*)orig, (void*)hook, nullptr)
#define HOOK_SAVE(orig, hook, save) Utils::HookFunction((void*)orig, (void*)hook, (void**)save)

// Read / Write helpers
#define READ_BYTE(addr)    (*(volatile uint8_t*)(addr))
#define READ_WORD(addr)    (*(volatile uint16_t*)(addr))
#define READ_DWORD(addr)   (*(volatile uint32_t*)(addr))
#define READ_QWORD(addr)   (*(volatile uint64_t*)(addr))
#define READ_FLOAT(addr)   (*(volatile float*)(addr))
#define READ_DOUBLE(addr)  (*(volatile double*)(addr))

#define WRITE_BYTE(addr, val)   do { uint8_t v = (val); Utils::WriteMemory((void*)(addr), &v, 1); } while(0)
#define WRITE_WORD(addr, val)   do { uint16_t v = (val); Utils::WriteMemory((void*)(addr), &v, 2); } while(0)
#define WRITE_DWORD(addr, val)  do { uint32_t v = (val); Utils::WriteMemory((void*)(addr), &v, 4); } while(0)
#define WRITE_QWORD(addr, val)  do { uint64_t v = (val); Utils::WriteMemory((void*)(addr), &v, 8); } while(0)
#define WRITE_FLOAT(addr, val)  do { float v = (val); Utils::WriteMemory((void*)(addr), &v, 4); } while(0)
#define WRITE_DOUBLE(addr, val) do { double v = (val); Utils::WriteMemory((void*)(addr), &v, 8); } while(0)

// NOP patch
#define NOP_PATCH(addr, count) do { \
    uint32_t nop = 0xD503201F; \
    for (int i = 0; i < (count); i++) { \
        Utils::WriteMemory((void*)((uintptr_t)(addr) + i*4), &nop, 4); \
    } \
} while(0)

// BRK / RET
#define RET_PATCH(addr) do { \
    uint32_t ret = 0xD65F03C0; \
    Utils::WriteMemory((void*)(addr), &ret, 4); \
} while(0)
