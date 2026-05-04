#pragma once
#include "Helpers.h"

#define INVALID_TILE_ID UINT16_MAX

typedef struct TileStruct_t {
  PVOID ref;
  TilePoint tile;
} TileStruct;

typedef struct TileConnexStruct_t {
  TileStruct tileStruct;
  uint16_t planeID;
  uint8_t isWater;
} TileConnexStruct;

typedef struct TilePlaneMap_t {
  size_t rowTileCount;
  TileConnexStruct **map;
} TilePlaneMap;

void map_Init();
uint8_t map_Tile_IsWater(TilePoint self);
char **map_GetBitMap(size_t *tileCount);
void map_BitMapDelete(char **map, size_t mapSizeInTiles);
uint16_t map_Tile_GetPlaneID(TilePoint tile);