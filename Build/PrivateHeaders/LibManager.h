#pragma once
#include <windows.h>
#include <stdint.h>

PVOID lib_BaseAddress();
PVOID lib_LowLevelEngine();
uint8_t lib_IsLoaded();