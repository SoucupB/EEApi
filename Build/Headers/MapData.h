#pragma once
#include "Helpers.h"

void map_Init();
void map_Delete();
uint8_t map_Tile_IsWater(TilePoint self);
char **map_GetBitMap(size_t *tileCount);
void map_BitMapDelete(char **map, size_t mapSizeInTiles);