#include "Resource.h"
#include "LibManager.h"
#include "Game.h"
#include "EETwa.h"
#include "Unit.h"
#include "ResourcePrivate.h"
#include "PlayerPrivate.h"
#include "EETypesStructPrivate.h"
#include "MethodsDefinitions.h"
#include "Offset.h"

uint8_t res_IsUnitResource(PVOID simpleUnit);

void res_InitResource(PVOID unit) {
  PResourceManager manager = game_GetResourcesManager();
  if(!res_IsUnitResource(unit)) {
    return ;
  }
  (*manager->resourcesRefs)[unit] = 1;
  ply_Inits(unit);
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
  size_t classOffset = *(size_t *)((size_t)ref + RESOURCE_CLASS);
  
  return (char *)(*(size_t *)(classOffset + RESOURCE_CLASS_NAME));
}

NeutralUnitType res_Type(Resource unit) {
  size_t *unitMetaData = (size_t *)util_Pointer((PVOID)unit._payload, RESOURCE_CLASS, POINTER_TYPE);
  return (NeutralUnitType)*(int32_t *)util_Pointer((PVOID)unitMetaData, RESOURCE_CLASS_TYPE, INT32_T_TYPE);
}

uint8_t res_IsUnitResource(PVOID simpleUnit) {
  UnitType type = unit_Type((Unit) {
    ._payload = simpleUnit
  });
  if(type == UNIT_UNDEFINED && eeTypes_Neutral_IsResource(res_Type((Resource) {
    ._payload = simpleUnit
  }))) {
    return 1;
  }
  return 0;
}

Resource res_Null() {
  return (Resource) {
    ._payload = NULL
  };
}

TilePoint res_Tile_Position(Resource self) {
  int32_t x = *(int32_t *)util_Pointer(res_Reference(self), RESOURCE_TILE_X, INT32_T_TYPE);
  int32_t y = *(int32_t *)util_Pointer(res_Reference(self), RESOURCE_TILE_Y, INT32_T_TYPE);
  return (TilePoint) {
    .x = x,
    .y = y
  };
}

Point res_Point_Position(Resource self) {
  TilePoint tilePoint = res_Tile_Position(self);
  return (Point) {
    .x = (float)tilePoint.x,
    .y = (float)tilePoint.y,
  };
}

uint8_t res_IsPresent(Resource resource) {
  PResourceManager manager = game_GetResourcesManager();
  return manager->resourcesRefs->find(res_Reference(resource)) != manager->resourcesRefs->end();
}

void res_OnRelease(Resource resource) {
  PResourceManager manager = game_GetResourcesManager();
  manager->resourcesRefs->erase(res_Reference(resource));
}

int32_t res_GetResource(Player ply, ResourceIndex res) {
  return *(int32_t *)((size_t)ply_Reference(ply) + RESOURCE_PLAYER_RES_OFFSET + res * sizeof(int32_t));
}

void res_SetResource(Player ply, ResourceIndex res, int32_t value) {
  *(int32_t *)((size_t)ply_Reference(ply) + RESOURCE_PLAYER_RES_OFFSET + res * sizeof(int32_t)) = value;
}