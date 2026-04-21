#include "MapData.h"
#include <vector>
#include <stdlib.h>

using namespace std;

#define INVALID_TILE_ID UINT16_MAX
#define PLANE_MARK_BIT (1<<15)

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

static vector<TileStruct> tiles;
static const size_t mapPointer = 0x530DFC;
static const size_t mapTileCount = 0x195618;
static const size_t tileStructPointer = 0x1955F0;
static const size_t isWaterMethodOffset = 0x12681;
static const size_t mapPointerTileOffset = 0x1C;
static TilePlaneMap planeMap;

static inline uint8_t map_TileConnex_IsMarked(TileConnexStruct self);
static inline void map_TileConnex_Mark(TileConnexStruct *self);
static inline uint16_t map_TileConnex_PlaneID(TileConnexStruct self);

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
  // for(size_t i = 0; i < tiles.size(); i++) {
  //   TilePoint tile = tiles[i].tile;
  //   if(map_Tile_IsWater(tile)) {
  //     map[tile.x][tile.y] = 2;
  //   }
  //   else {
  //     map[tile.x][tile.y] = 1;
  //   }
  // }
  for(size_t i = 0; i < mapSizeInTiles; i++) {
    for(size_t j = 0; j < mapSizeInTiles; j++) {
      uint16_t currentID = map_Tile_GetPlaneID((TilePoint) {
        .x = (int32_t)i,
        .y = (int32_t)j
      });
      if(currentID == INVALID_TILE_ID) {
        map[i][j] = 0;
      }
      else {
        map[i][j] = currentID;
      }
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

static inline uint8_t map_TileConnex_IsMarked(TileConnexStruct self) {
  return (self.planeID & PLANE_MARK_BIT) != 0;
}

static inline uint16_t map_TileConnex_PlaneID(TileConnexStruct self) {
  if(map_TileConnex_IsMarked(self)) {
    return (self.planeID ^ PLANE_MARK_BIT);
  }
  return self.planeID;
}

static inline void map_TileConnex_Mark(TileConnexStruct *self) {
  self->planeID ^= PLANE_MARK_BIT;
}

static inline uint8_t map_TileMap_Fill(size_t i, size_t j, uint16_t currentPlaneID) {
  if(map_TileConnex_IsMarked(planeMap.map[i][j])) {
    return 0;
  }
  TileConnexStruct **tileMap = planeMap.map;
  TileConnexStruct *initialConnexTile = &planeMap.map[i][j];
  const size_t tileCount = planeMap.rowTileCount;
  int32_t xPos[] = {0, 1, 0, -1};
  int32_t yPos[] = {1, 0, -1, 0};
  initialConnexTile->planeID = currentPlaneID;
  map_TileConnex_Mark(initialConnexTile);
  vector<TileConnexStruct *> queue;
  queue.push_back(initialConnexTile);
  size_t head = 0;
  while(head != queue.size()) {
    TileConnexStruct *currentTileConnex = queue[head];
    TilePoint pnt = currentTileConnex->tileStruct.tile;
    for(size_t i = 0; i < sizeof(xPos) / sizeof(int32_t); i++) {
      const int32_t nextTileX = pnt.x + xPos[i];
      const int32_t nextTileY = pnt.y + yPos[i];
      if((nextTileX >= (int32_t)tileCount || nextTileX < 0) || (nextTileY >= (int32_t)tileCount || nextTileY < 0)) {
        continue;
      }
      TileConnexStruct *nextTile = &tileMap[nextTileX][nextTileY];
      if(nextTile->planeID == INVALID_TILE_ID || map_TileConnex_IsMarked(*nextTile) || nextTile->isWater != initialConnexTile->isWater) {
        continue;
      }
      queue.push_back(nextTile);
      map_TileConnex_Mark(nextTile);
    }
    head++;
  }
  return 1;
}

uint16_t map_Tile_GetPlaneID(TilePoint tile) {
  const int32_t tileCount = (int32_t)planeMap.rowTileCount;
  if((tile.x < 0 || tile.x >= tileCount) || (tile.y < 0 || tile.y >= tileCount)) {
    return INVALID_TILE_ID;
  }
  TileConnexStruct initialConnexTile = planeMap.map[tile.x][tile.y];
  if(initialConnexTile.planeID == INVALID_TILE_ID || !map_TileConnex_IsMarked(initialConnexTile)) {
    return INVALID_TILE_ID;
  }
  return map_TileConnex_PlaneID(initialConnexTile);
}

void map_TileMap_FillPlaneIDs() {
  const size_t count = planeMap.rowTileCount;
  uint16_t planeID = 0;
  for(size_t i = 0; i < count; i++) {
    for(size_t j = 0; j < count; j++) {
      TileConnexStruct connexTile = planeMap.map[i][j];
      if(connexTile.planeID == INVALID_TILE_ID) {
        continue;
      }
      if(map_TileMap_Fill(i, j, planeID)) {
        planeID++;
      }
    }
  }
}

void map_ComputerConnexIslands() {
  map_TileMap_Init();
  map_TileMap_FillWithReffs();
  map_TileMap_FillPlaneIDs();
}

void map_Init() {
  tiles.clear();
  tiles = map_GetTiles();
  map_ComputerConnexIslands();
}

void map_Delete() {
  tiles.clear();
}

