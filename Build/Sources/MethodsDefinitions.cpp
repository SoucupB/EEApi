#include "MethodsDefinitions.h"
#include "EETwa.h"
#include <map>
#include "LibManager.h"

std::map<size_t, std::pair<size_t, std::vector<size_t> > > storeBuilder;
uint8_t isMemoryValid(PVOID addr);
static size_t old_NewHandle;
static size_t old_FreeHandle;
static uint8_t mmu_Replaced;
static int32_t allocCount;
static uint8_t magicKey[] = {0xCC, 0xFF, 0xAA, 0x00, 0xBB, 0x12, 0x43, 0x27, 0x19, 0x91, 0x01, 0x02};
PVOID _cdecl builder_EnchantedNew(const size_t memSize);
PVOID _cdecl builder_EnchantedFree(const PVOID buffer);

typedef struct MMUHeader_t {
  uint8_t magicKey[0xC];
  size_t bufferSize;
} MMUHeader;

void builder_Definition(PVOID remoteAddress, PVOID localAddress) {
  size_t methodsAddress = (size_t)lib_BaseAddress() + (size_t)remoteAddress;
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

void builder_AllowRules(PVOID handle, size_t sz) {
  DWORD oldProtect;
  if (!VirtualProtect((void *)handle, sz, PAGE_EXECUTE_READWRITE, &oldProtect)) {
    eeTa_FilePrintf("Couldn't modify method at %p\n", handle);
    return;
  }
}

void builder_ReplaceNew() {
  size_t newModule = (size_t)lib_BaseAddress() + 0x4386DC;
  old_NewHandle = *(size_t *)newModule;
  size_t *comp = (size_t *)builder_EnchantedNew;
  builder_AllowRules((PVOID)newModule, sizeof(size_t) * 2);
  memcpy((PVOID)newModule, (PVOID)&comp, sizeof(size_t));
}

void builder_ReplaceFree() {
  size_t freeModule = (size_t)lib_BaseAddress() + 0x4386E4;
  old_FreeHandle = *(size_t *)freeModule;
  size_t *comp = (size_t *)builder_EnchantedFree;
  builder_AllowRules((PVOID)freeModule, sizeof(size_t) * 2);
  memcpy((PVOID)freeModule, (PVOID)&comp, sizeof(size_t));
}

PVOID _cdecl builder_EnchantedNew(const size_t memSize) {
  PVOID _cdecl (*method)(size_t) = (PVOID _cdecl (*)(size_t))old_NewHandle;
  const size_t newSize = memSize + sizeof(MMUHeader);
  PVOID buffer = method(newSize);
  allocCount++;
  memcpy(buffer, magicKey, sizeof(magicKey));
  memcpy((PVOID)((size_t)buffer + sizeof(magicKey)), &memSize, sizeof(size_t));
  PVOID responseRef = (PVOID)((size_t)buffer + sizeof(MMUHeader));
  memset(responseRef, 0, memSize);
  return responseRef;
}

uint8_t isMemoryValid(PVOID addr) {
  MEMORY_BASIC_INFORMATION mbi;
  SIZE_T result = VirtualQuery(addr, &mbi, sizeof(mbi));
  if (result == 0) {
    return 0;
  }
  return (mbi.State == MEM_COMMIT) &&
    !(mbi.Protect & PAGE_NOACCESS) &&
    !(mbi.Protect & PAGE_EXECUTE);
}

size_t bufferSize(PVOID buffer) {
  return ((size_t *)((size_t)buffer - sizeof(size_t)))[0];
}

uint8_t hasBeenAllocked(PVOID buffer) {
  PVOID offsetBuffer = (PVOID)((size_t)buffer - sizeof(MMUHeader));
  if(!isMemoryValid(offsetBuffer)) {
    return 0;
  }
  return memcmp(offsetBuffer, magicKey, sizeof(magicKey)) == 0;
}

PVOID _cdecl builder_EnchantedFree(const PVOID buffer) {
  PVOID _cdecl (*method)(PVOID) = (PVOID _cdecl (*)(PVOID))old_FreeHandle;
  PVOID offsetBuffer = (PVOID)((size_t)buffer - sizeof(MMUHeader));
  if(!isMemoryValid(offsetBuffer)) {
    return method(buffer);
  }
  if(!memcmp(offsetBuffer, magicKey, sizeof(magicKey))) {
    memset(offsetBuffer, 0, sizeof(MMUHeader));
    if(allocCount) {
      allocCount--;
    }
    return method(offsetBuffer);
  }
  return method(buffer);
}

void builder_PrintMemoryTree_t(PVOID mem, uint8_t offset, std::map<size_t, uint8_t > &memory) {
  if(offset > 2) {
    return ;
  }
  if(!hasBeenAllocked(mem) || (memory.find((size_t)mem) != memory.end())) {
    return ;
  }
  memory[(size_t)mem] = 1;
  for(size_t i = 0, c = bufferSize(mem); i < c; i += sizeof(size_t)) {
    PVOID pntBuffer = (PVOID)*(size_t *)((size_t)mem + i);
    if(!pntBuffer) {
      continue;
    }
    for(size_t p = 0; p < offset; p++) {
      eeTa_FilePrintf(" ");
    }
    eeTa_FilePrintf("%p - %p\n", i, pntBuffer);
    builder_PrintMemoryTree_t(pntBuffer, offset + 2, memory);
  }
}

void builder_PrintMemoryTree(PVOID mem) {
  std::map<size_t, uint8_t > memory;
  builder_PrintMemoryTree_t(mem, 0, memory);
}

void builder_ReplaceMMUMethods() {
  builder_ReplaceNew();
  builder_ReplaceFree();
  mmu_Replaced = 1;
}