#include "MapData.h"
#include <vector>
#include <stdlib.h>

using namespace std;

#define INVALID_TILE_ID 125

typedef struct TileStruct_t {
  PVOID ref;
  TilePoint tile;
} TileStruct;

typedef struct TileConnexStruct_t {
  TileStruct tileStruct;
  uint32_t planeID;
  uint8_t isWater;
} TileConnexStruct;

typedef struct TilePlaneMap_t {
  size_t rowTileCount;
  TileConnexStruct **map;
} TilePlaneMap;

static vector<TileStruct> tiles;
static const size_t mapPointer = 0x530DFC;
static const size_t mapTileCount = 0x195618;
static const size_t tileStructPointer = 0x1955F0;
static const size_t isWaterMethodOffset = 0x12681;
static const size_t mapPointerTileOffset = 0x1C;
static TilePlaneMap planeMap;

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

void map_TileMap_FreePreviousMap() {
  if(!planeMap.rowTileCount || !planeMap.map) {
    return ;
  }
  for(size_t i = 0; i < planeMap.rowTileCount; i++) {
    free(planeMap.map[i]);
  }
  free(planeMap.map);
  planeMap.rowTileCount = 0;
  planeMap.map = NULL;
}

void map_TileMap_Init() {
  map_TileMap_FreePreviousMap();
  PVOID mapPointer = map_GetMapPointer();
  size_t count = map_TileCount(mapPointer);
  planeMap.rowTileCount = count;
  planeMap.map = (TileConnexStruct **)malloc(count * sizeof(TileConnexStruct *));
  for(size_t i = 0; i < count; i++) {
    planeMap.map[i] = (TileConnexStruct *)malloc(count * sizeof(TileConnexStruct));
    memset(planeMap.map[i], 0, count * sizeof(TileConnexStruct));
  }
  for(size_t i = 0; i < count; i++) {
    for(size_t j = 0; j < count; j++) {
      planeMap.map[i][j].planeID = INVALID_TILE_ID;
    }
  }
}

void map_TileMap_FillWithReffs() {
  const size_t count = planeMap.rowTileCount;
  TileConnexStruct **map = planeMap.map;
  for(size_t i = 0; i < tiles.size(); i++) {
    TileStruct tileStruct = tiles[i];
    map[tileStruct.tile.x][tileStruct.tile.y] = (TileConnexStruct) {
      .tileStruct = tileStruct,
      .planeID = 0,
      .isWater = map_Tile_IsWater(tileStruct.tile)
    };
  }
}

void map_ComputerConnexIslands() {
  map_TileMap_Init();
  map_TileMap_FillWithReffs();
}

void map_Init() {
  tiles.clear();
  tiles = map_GetTiles();
  map_ComputerConnexIslands();
}

void map_Delete() {
  tiles.clear();
}

