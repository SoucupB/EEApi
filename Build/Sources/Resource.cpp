#include "Resource.h"
#include "LibManager.h"

void res_OnInit() {
  // 
}

PVOID res_GetNeutralPlayer() {
  size_t *resourceManager = (size_t *)((size_t)((size_t)lib_BaseAddress() + 0x530D40) + 0x74);
  return (PVOID)*resourceManager;
}

size_t res_TotalNetralUnits() {
  PVOID manager = res_GetNeutralPlayer();

  return *(size_t *)((size_t)manager + 0x30);
}

PVOID res_Reference(Resource resource) {
  return resource._payload;
}

Resource res_GetResource(PVOID manager, size_t index) {
  return (Resource) {
    ._payload = (PVOID)*(size_t *)((size_t)manager + 0x38 + index * 0x8)
  };
}

uint8_t res_IsValid(PVOID manager, size_t index) {
  Resource currentRes = res_GetResource(manager, index);
  if(!res_Reference(currentRes)) {
    return 0;
  }
  size_t validFlag = *(size_t *)((size_t)manager + 0x38 + index * 0x8 + 0x4);
  return validFlag == 0x1;
}

vector<Resource> res_All() {
  PVOID manager = res_GetNeutralPlayer();
  size_t count = res_TotalNetralUnits();
  vector<Resource> response;
  for(size_t i = 0; i < count; i++) {
    if(res_IsValid(manager, i)) {
      continue;
    }
    response.push_back(res_GetResource(manager, i));
  }

  return response;
}