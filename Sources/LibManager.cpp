#include <windows.h>
#include "LibManager.h"

static const char *procName = "EE-AOC.exe";
static const char *lowLevelEngineDLLName = "Low-Level Engine.dll";
static PVOID baseAddress;
static PVOID lowLevelBaseAddress;

PVOID lib_BaseAddress() {
  if(baseAddress) {
    return baseAddress;
  }
  baseAddress = (PVOID)GetModuleHandleA(procName);
  return baseAddress;
}

PVOID lib_LowLevelEngine() {
  if(lowLevelBaseAddress) {
    return lowLevelBaseAddress;
  }
  lowLevelBaseAddress = GetModuleHandleA(lowLevelEngineDLLName);
  return lowLevelBaseAddress;
}

uint8_t lib_IsLoaded() {
  return lib_BaseAddress() != NULL && lib_LowLevelEngine() != NULL;
}