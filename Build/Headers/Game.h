#pragma once
#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <map>
#include "MapData.h"
#include "EETypes.h"

using namespace std;

typedef struct UnitHealth_t {
  int8_t isBeingAttacked;
  int32_t health;
} UnitHealth;

typedef struct PlayerState_t {
  unordered_map<PVOID, UnitHealth> *unitsHealth;
  PVOID method;
} PlayerState;

typedef PlayerState *PPlayerState;

typedef struct MapData_t {
  vector<TileStruct> *tiles;
  TilePlaneMap *planeMap;
} MapData;

typedef MapData *PMapData;

typedef struct EETypes_t {
  map<UnitClassType, map<UnitType, uint8_t> > classTreeStructure;
} EETypes;

typedef struct Game_t {
  PPlayerState plyState;
  PMapData mapData;
  EETypes types;
} Game;

typedef Game *PGame;

void game_Init();
PPlayerState game_GetPlayerState();
PMapData game_GetMapData();