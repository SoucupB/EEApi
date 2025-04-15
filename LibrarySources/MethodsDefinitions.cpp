#include "MethodsDefinitions.h"

void builder_Definition(PVOID remoteAddress, PVOID localAddress) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return ;
  }
  size_t methodsAddress = (size_t)hModule + (size_t)remoteAddress;
  DWORD oldProtect;
  if (!VirtualProtect((void *)methodsAddress, 2 * sizeof(uint32_t), PAGE_READWRITE, &oldProtect)) {
    return;
  }
  util_ModifyTargetProcessCaller((uint8_t *)methodsAddress, (size_t)localAddress);
}