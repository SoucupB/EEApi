#pragma once
#include <windows.h>
#include "Game.h"
#include "Geometry.h"

typedef struct Resource_t {
  PVOID _payload;
} Resource;

vector<Resource> res_All();
PVOID res_Reference(Resource resource);
void res_InitResource(PVOID resource);
char *res_Name(Resource self);
NeutralUnitType res_Type(Resource unit);
Resource res_Null();
TilePoint res_Tile_Position(Resource self);
Point res_Point_Position(Resource self);
void res_OnRelease(Resource resource);
uint8_t res_IsPresent(Resource resource);