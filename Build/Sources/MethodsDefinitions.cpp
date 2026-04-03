#include "MethodsDefinitions.h"
#include "EETwa.h"
#include <map>

std::map<size_t, std::pair<size_t, std::vector<size_t> > > storeBuilder;

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

void builder_FillValue(PVOID ref, size_t offset, size_t value) {
  PVOID address = (PVOID)((size_t)ref + offset);
  memcpy(address, &value, sizeof(size_t));
}

void builder_Store(PVOID ref, size_t sz) {
  uint8_t *bff = (uint8_t *)ref;
  storeBuilder[(size_t)ref].first = sz;
  storeBuilder[(size_t)ref].second.clear();
  for(size_t i = 0; i < sz; i += sizeof(size_t)) {
    storeBuilder[(size_t)ref].second.push_back(*(size_t *)(bff + i));
  }
}

void builder_CheckChanges(PVOID ref) {
  std::pair<size_t, std::vector<size_t> > response = storeBuilder[(size_t)ref];
  if(!response.second.size()) {
    return ;
  }
  uint8_t *bff = (uint8_t *)ref;
  eeTa_FilePrintf("-------\n");
  eeTa_FilePrintf("For ref %p size is %p, arr size is %p\n", ref, response.first, response.second.size());
  for(size_t i = 0, p = 0, c = response.second.size(); p < c; p++, i += sizeof(size_t)) {
    size_t currentValue = *(size_t *)(bff + i);
    if(currentValue != response.second[p]) {
      eeTa_FilePrintf("Difference at %p then: %p, now: %p\n", i, response.second[p], currentValue);
      continue;
    }
    eeTa_FilePrintf("Equal at %p - %p - %p\n", i, response.second[p], currentValue);
  }
  eeTa_FilePrintf("+++++++\n");
}