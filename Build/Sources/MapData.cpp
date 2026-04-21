#include "MapData.h"
#include <vector>
#include <stdlib.h>

using namespace std;

typedef struct TileStruct_t {
  PVOID ref;
  TilePoint tile;
} TileStruct;

static vector<TileStruct> tiles;
static const size_t mapPointer = 0x530DFC;
static const size_t mapTileCount = 0x195618;
static const size_t tileStructPointer = 0x1955F0;
static const size_t isWaterMethodOffset = 0x12681;
static const size_t mapPointerTileOffset = 0x1C;

size_t map_TileCount(PVOID mapPointer) {
  return *(size_t *)((size_t)mapPointer + mapTileCount);
}

PVOID map_TilePointer(PVOID mapPointer) {
  return (PVOID)*(size_t *)((size_t)mapPointer + tileStructPointer);
}

PVOID map_GetMapPointer() {
  PVOID basePointer = (PVOID)((size_t)GetModuleHandleA("EE-AOC.exe") + mapPointer);
  return (PVOID)*(size_t *)basePointer;
}

vector<TileStruct> map_GetTiles() {
  vector<TileStruct> response;
  PVOID mapPointer = map_GetMapPointer();
  size_t count = map_TileCount(mapPointer);
  PVOID tileRef = map_TilePointer(mapPointer);
  for(size_t i = 0; i < count; i++) {
    for(size_t j = 0; j < count; j++) {
      size_t currentTile = *(size_t *)((size_t)tileRef + (count * i + j) * 4);
      if(currentTile) {
        response.push_back((TileStruct) {
          .ref = (PVOID)currentTile,
          .tile = (TilePoint) {
            .x = (int32_t)j,
            .y = (int32_t)i
          }
        });
      }
    }
  }
  return response;
}

char **map_GetBitMap(size_t *tileCount) {
  PVOID mapPointer = map_GetMapPointer();
  size_t mapSizeInTiles = map_TileCount(mapPointer);
  char **map = (char **)malloc(mapSizeInTiles * sizeof(char *));
  for(size_t i = 0; i < mapSizeInTiles; i++) {
    map[i] = (char *)malloc(mapSizeInTiles * sizeof(char));
    memset(map[i], 0, mapSizeInTiles * sizeof(char));
  }
  for(size_t i = 0; i < tiles.size(); i++) {
    TilePoint tile = tiles[i].tile;
    if(map_Tile_IsWater(tile)) {
      map[tile.x][tile.y] = 2;
    }
    else {
      map[tile.x][tile.y] = 1;
    }
  }
  if(tileCount) {
    *tileCount = mapSizeInTiles;
  }
  return map;
}

void map_BitMapDelete(char **map, size_t mapSizeInTiles) {
  for(size_t i = 0; i < mapSizeInTiles; i++) {
    free(map[i]);
  }
  free(map);
}

vector<TileStruct> map_GetTilesArray() {
  return tiles;
}

uint8_t map_Tile_IsWater(TilePoint self) {
  PVOID mapPointer = map_GetMapPointer();
  HMODULE lowLevelDLL = GetModuleHandleA("Low-Level Engine.dll");
  if(!lowLevelDLL) {
    return 0;
  }
  size_t isWaterTileMethodOffset = (size_t)lowLevelDLL + isWaterMethodOffset;
  uint8_t __thiscall (*method)(PVOID, PVOID, PVOID) = (uint8_t __thiscall (*)(PVOID, PVOID, PVOID))isWaterTileMethodOffset;
  return method((PVOID)((size_t)mapPointer + mapPointerTileOffset), (PVOID)self.x, (PVOID)self.y);
}

void map_Init() {
  tiles = map_GetTiles();
}

void map_Delete() {
  tiles.clear();
}

