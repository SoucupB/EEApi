#include "Resource.h"
#include "LibManager.h"
#include "Game.h"
#include "EETwa.h"

uint8_t res_IsUnitResource(PVOID simpleUnit);

void res_InitResource(PVOID unit) {
  PResourceManager manager = game_GetResourcesManager();
  if(!res_IsUnitResource(unit)) {
    return ;
  }
  (*manager->resourcesRefs)[unit] = 1;
}

// PVOID res_GetNeutralPlayer() {
//   size_t *resourceManager = (size_t *)((size_t)((size_t)lib_BaseAddress() + 0x530D40) + 0x74);
//   return (PVOID)*resourceManager;
// }

// size_t res_TotalNetralUnits() {
//   PVOID manager = res_GetNeutralPlayer();

//   return *(size_t *)((size_t)manager + 0x460);
// }

PVOID res_Reference(Resource resource) {
  return resource._payload;
}

// Resource res_GetResource(PVOID manager, size_t index) {
//   return (Resource) {
//     ._payload = (PVOID)*(size_t *)((size_t)manager + 0x38 + index * 0x8)
//   };
// }

// uint8_t res_IsValid(PVOID manager, size_t index) {
//   Resource currentRes = res_GetResource(manager, index);
//   if(!res_Reference(currentRes)) {
//     return 0;
//   }
//   return 1;
// }

vector<Resource> res_All() {
  vector<Resource> response;
  PResourceManager manager = game_GetResourcesManager();
  for(auto &it : *manager->resourcesRefs) {
    response.push_back((Resource) {
      ._payload = it.first
    });
  }

  return response;
}

uint8_t res_IsUnitResource(PVOID simpleUnit) {
  size_t currentClass = *(size_t *)simpleUnit;
  if(currentClass != 0x447A20 + (size_t)lib_BaseAddress()) {
    return 0;
  }
  size_t resourceClass = *(size_t *)((size_t)simpleUnit + 0x2C);
  if(*(size_t *)resourceClass != 0x449608 + (size_t)lib_BaseAddress()) {
    return 0;
  }
  return 1;
}