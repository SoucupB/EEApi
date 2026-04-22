#pragma once
#include "EETwa.h"
#include <stdint.h>

typedef struct Unit_t Unit;

vector<Unit> unit_GetBuildings(int8_t player);
vector<Unit> unit_GetUnits(int8_t player);
int32_t unit_CurrentlyBuilding(Unit building);
int8_t unit_IsBuilding(Unit unit);
TilePoint unit_Tile_Position(Unit unit);
uint16_t unit_GetPlaneID(Unit unit);