#include "MapData.h"
#include <vector>
#include <stdlib.h>
#include "LibManager.h"
#include "Game.h"
#include "EETypesStructPrivate.h"
#include "Offset.h"

using namespace std;

#define PLANE_MARK_BIT (1<<15)

static const size_t mapPointer = MAPDATA_REMOTE_METHOD_MAP_POINTER;
static const size_t mapTileCount = MAPDATA_TILE_COUNT;
static const size_t tileStructPointer = MAPDATA_TILE_STRUCT_POINTER;
static const size_t isWaterMethodOffset = MAPDATA_REMOTE_METHOD_IS_WATER_TILE;
static const size_t isSpaceMethodOffset = MAPDATA_REMOTE_METHOD_IS_SPACE_TILE;
static const size_t isSpaceMap = MAPDATA_REMOVE_METHOD_IS_SPACE;
static const size_t mapPointerTileOffset = MAPDATA_MAP_POINTER_TILE_OFFSET;
static const size_t isEdgeMethodOffset = MAPDATA_REMOTE_METHOD_IS_EDGE_TILE;
static const size_t zTileMethodOffset = MAPDATA_REMOTE_METHOD_TILE_GET_Z;

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
  PVOID basePointer = (PVOID)((size_t)lib_BaseAddress() + mapPointer);
  return (PVOID)*(size_t *)basePointer;
}

void map_FillTiles() {
  PMapData md = game_GetMapData();
  vector<TileStruct> *tiles = md->tiles;

  PVOID mapPointer = map_GetMapPointer();
  size_t count = map_TileCount(mapPointer);
  PVOID tileRef = map_TilePointer(mapPointer);
  for(size_t i = 0; i < count; i++) {
    for(size_t j = 0; j < count; j++) {
      size_t currentTile = *(size_t *)((size_t)tileRef + (count * i + j) * sizeof(int32_t));
      if(currentTile) {
        tiles->push_back((TileStruct) {
          .ref = (PVOID)currentTile,
          .tile = (TilePoint) {
            .x = (int32_t)j,
            .y = (int32_t)i
          }
        });
      }
    }
  }
}

char **map_GetBitMap(size_t *tileCount) {
  PVOID mapPointer = map_GetMapPointer();
  size_t mapSizeInTiles = map_TileCount(mapPointer);
  char **map = (char **)malloc(mapSizeInTiles * sizeof(char *));
  for(size_t i = 0; i < mapSizeInTiles; i++) {
    map[i] = (char *)malloc(mapSizeInTiles * sizeof(char));
    memset(map[i], 0, mapSizeInTiles * sizeof(char));
  }
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
        map[i][j] = currentID + 1;
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

vector<TileStruct> map_FillTilesArray() {
  PMapData md = game_GetMapData();
  return *md->tiles;
}

uint8_t map_Tile_IsWater(TilePoint self) {
  PVOID mapPointer = map_GetMapPointer();
  size_t isWaterTileMethodOffset = (size_t)lib_LowLevelEngine() + isWaterMethodOffset;
  uint8_t __thiscall (*method)(PVOID, PVOID, PVOID) = (uint8_t __thiscall (*)(PVOID, PVOID, PVOID))isWaterTileMethodOffset;
  return method((PVOID)((size_t)mapPointer + mapPointerTileOffset), (PVOID)self.x, (PVOID)self.y);
}

uint8_t map_Tile_IsEdge(TilePoint self) {
  PVOID mapPointer = map_GetMapPointer();
  size_t isEdgeTileMethodOffset = (size_t)lib_LowLevelEngine() + isEdgeMethodOffset;
  uint8_t __thiscall (*method)(PVOID, PVOID, PVOID) = (uint8_t __thiscall (*)(PVOID, PVOID, PVOID))isEdgeTileMethodOffset;
  return method((PVOID)((size_t)mapPointer + mapPointerTileOffset), (PVOID)self.x, (PVOID)self.y);
}

float map_Tile_GetZ(TilePoint self) {
  PVOID mapPointer = map_GetMapPointer();
  size_t isEdgeTileMethodOffset = (size_t)lib_LowLevelEngine() + zTileMethodOffset;
  float __thiscall (*method)(PVOID, PVOID, PVOID) = (float __thiscall (*)(PVOID, PVOID, PVOID))isEdgeTileMethodOffset;
  return method((PVOID)((size_t)mapPointer + mapPointerTileOffset), (PVOID)self.x, (PVOID)self.y);
}

void map_TileMap_FreePreviousMap() {
  PMapData md = game_GetMapData();
  TilePlaneMap *planeMap = md->planeMap;

  if(!planeMap->rowTileCount || !planeMap->map) {
    return ;
  }
  for(size_t i = 0; i < planeMap->rowTileCount; i++) {
    free(planeMap->map[i]);
  }
  free(planeMap->map);
  planeMap->rowTileCount = 0;
  planeMap->map = NULL;
}

void map_TileMap_Init() {
  map_TileMap_FreePreviousMap();
  PVOID mapPointer = map_GetMapPointer();
  size_t count = map_TileCount(mapPointer);
  PMapData md = game_GetMapData();
  TilePlaneMap *planeMap = md->planeMap;

  planeMap->rowTileCount = count;
  planeMap->map = (TileConnexStruct **)malloc(count * sizeof(TileConnexStruct *));
  for(size_t i = 0; i < count; i++) {
    planeMap->map[i] = (TileConnexStruct *)malloc(count * sizeof(TileConnexStruct));
    memset(planeMap->map[i], 0, count * sizeof(TileConnexStruct));
  }
  for(size_t i = 0; i < count; i++) {
    for(size_t j = 0; j < count; j++) {
      planeMap->map[i][j].planeID = INVALID_TILE_ID;
    }
  }
}

void map_TileMap_FillWithReffs() {
  PMapData md = game_GetMapData();
  TilePlaneMap *planeMap = md->planeMap;
  TileConnexStruct **map = planeMap->map;
  vector<TileStruct> *tiles = md->tiles;

  for(size_t i = 0; i < tiles->size(); i++) {
    TileStruct tileStruct = (*tiles)[i];
    map[tileStruct.tile.x][tileStruct.tile.y] = (TileConnexStruct) {
      .tileStruct = tileStruct,
      .planeID = 0,
      .isSpecialTerrain = map_Tile_IsSpecialTerrain(tileStruct.tile)
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

uint8_t map_TileMap_Fill(size_t i, size_t j, uint16_t currentPlaneID) {
  PMapData md = game_GetMapData();
  TilePlaneMap *planeMap = md->planeMap;

  if(map_TileConnex_IsMarked(planeMap->map[i][j])) {
    return 0;
  }
  TileConnexStruct **tileMap = planeMap->map;
  TileConnexStruct *initialConnexTile = &planeMap->map[i][j];
  const size_t tileCount = planeMap->rowTileCount;
  int32_t xPos[] = {0, 1, 0, -1, 1, 1, -1, -1};
  int32_t yPos[] = {1, 0, -1, 0, 1, -1, 1, -1};
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
      if(nextTile->planeID == INVALID_TILE_ID || map_TileConnex_IsMarked(*nextTile) || nextTile->isSpecialTerrain != initialConnexTile->isSpecialTerrain) {
        continue;
      }
      queue.push_back(nextTile);
      nextTile->planeID = currentPlaneID;
      map_TileConnex_Mark(nextTile);
    }
    head++;
  }
  return 1;
}

uint16_t map_Tile_GetPlaneID(TilePoint tile) {
  PMapData md = game_GetMapData();
  TilePlaneMap *planeMap = md->planeMap;

  const int32_t tileCount = (int32_t)planeMap->rowTileCount;
  if((tile.x < 0 || tile.x >= tileCount) || (tile.y < 0 || tile.y >= tileCount)) {
    return INVALID_TILE_ID;
  }
  TileConnexStruct initialConnexTile = planeMap->map[tile.x][tile.y];
  if(initialConnexTile.planeID == INVALID_TILE_ID || !map_TileConnex_IsMarked(initialConnexTile)) {
    return INVALID_TILE_ID;
  }
  return map_TileConnex_PlaneID(initialConnexTile);
}

void map_TileMap_FillPlaneIDs() {
  PMapData md = game_GetMapData();
  TilePlaneMap *planeMap = md->planeMap;

  const size_t count = planeMap->rowTileCount;
  uint16_t planeID = 0;
  for(size_t i = 0; i < count; i++) {
    for(size_t j = 0; j < count; j++) {
      TileConnexStruct connexTile = planeMap->map[i][j];
      if(connexTile.planeID == INVALID_TILE_ID) {
        continue;
      }
      if(map_TileMap_Fill(i, j, planeID)) {
        planeID++;
      }
    }
  }
}

uint8_t map_IsSpaceMap() {
  PVOID mapPointer = map_GetMapPointer();
  size_t isSpaceMapPointer = (size_t)lib_LowLevelEngine() + isSpaceMap;
  uint8_t __thiscall (*method)(PVOID) = (uint8_t __thiscall (*)(PVOID))isSpaceMapPointer;
  return method((PVOID)((size_t)mapPointer + mapPointerTileOffset));
}

uint8_t map_Tile_IsSpace(TilePoint self) {
  PVOID mapPointer = map_GetMapPointer();
  size_t isSpaceTileMethodOffset = (size_t)lib_LowLevelEngine() + isSpaceMethodOffset;
  uint8_t __thiscall (*method)(PVOID, PVOID, PVOID) = (uint8_t __thiscall (*)(PVOID, PVOID, PVOID))isSpaceTileMethodOffset;
  return method((PVOID)((size_t)mapPointer + mapPointerTileOffset), (PVOID)self.x, (PVOID)self.y);
}

uint8_t map_Tile_IsSpecialTerrain(TilePoint self) {
  PMapData mapData = game_GetMapData();
  if(mapData->isSpaceMap) {
    return map_Tile_IsSpace(self);
  }
  return map_Tile_IsWater(self);
}

void map_ComputeConnexIslands() {
  map_TileMap_Init();
  map_TileMap_FillWithReffs();
  map_TileMap_FillPlaneIDs();
}

void map_Init() {
  PMapData mapData = game_GetMapData();
  mapData->isSpaceMap = map_IsSpaceMap();
  map_FillTiles();
  map_ComputeConnexIslands();
}

