#include "Resource.h"
#include "LibManager.h"
#include "Game.h"
#include "EETwa.h"

uint8_t res_IsUnitResource(PVOID simpleUnit);
uint8_t res_IsUnitComplexUnitResource(PVOID simpleUnit);

void res_InitResource(PVOID unit) {
  PResourceManager manager = game_GetResourcesManager();
  if(!res_IsUnitResource(unit) && !res_IsUnitComplexUnitResource(unit)) {
    return ;
  }
  (*manager->resourcesRefs)[unit] = 1;
}

PVOID res_Reference(Resource resource) {
  return resource._payload;
}

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

char *res_Name(Resource self) {
  PVOID ref = res_Reference(self);
  size_t classOffset = *(size_t *)((size_t)ref + 0x2C);
  
  return (char *)(*(size_t *)(classOffset + 0x1C));
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

uint8_t res_IsUnitComplexUnitResource(PVOID simpleUnit) {
  size_t currentClass = *(size_t *)simpleUnit;
  if(currentClass != 0x446DDC + (size_t)lib_BaseAddress()) {
    return 0;
  }
  size_t resourceClass = *(size_t *)((size_t)simpleUnit + 0x2C);
  if(*(size_t *)resourceClass != 0x449350 + (size_t)lib_BaseAddress()) {
    return 0;
  }
  return 1;
}