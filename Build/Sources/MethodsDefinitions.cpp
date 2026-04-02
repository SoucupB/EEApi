#include "MethodsDefinitions.h"
#include "EETwa.h"

void builder_Definition(PVOID remoteAddress, PVOID localAddress) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return ;
  }
  size_t methodsAddress = (size_t)hModule + (size_t)remoteAddress;
  DWORD oldProtect;
  if (!VirtualProtect((void *)methodsAddress, 2 * sizeof(uint32_t), PAGE_READWRITE, &oldProtect)) {
    eeTa_FilePrintf("Couldn't modify method at %p\n", remoteAddress);
    return;
  }
  eeTa_FilePrintf("Address modified at %p\n", remoteAddress);
  util_ModifyTargetProcessCaller((uint8_t *)methodsAddress, (size_t)localAddress);
}

void builder_PrintMemoryLayout(PVOID ref, size_t sz) {
  uint8_t *bff = (uint8_t *)ref;
  eeTa_FilePrintf("-------\n");
  for(size_t i = 0; i < sz; i += sizeof(size_t)) {
    eeTa_FilePrintf("Address %p, value: %p\n", i, *(size_t *)(bff + i));
  }
  eeTa_FilePrintf("+++++++\n");
}