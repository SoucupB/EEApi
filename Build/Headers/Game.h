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

typedef struct MapData_t {
  vector<TileStruct> *tiles;
  TilePlaneMap *planeMap;
} MapData;

typedef struct EETypes_t {
  map<UnitClassType, map<UnitType, uint8_t> > *classTreeStructure;
  map<UnitType, UnitClassType> *parentsClass;
} EETypes;

typedef EETypes *PEETypes;
typedef MapData *PMapData;
typedef PlayerState *PPlayerState;

typedef struct Game_t {
  PPlayerState plyState;
  PMapData mapData;
  PEETypes types;
} Game;

typedef Game *PGame;

void game_Init();
PPlayerState game_GetPlayerState();
PMapData game_GetMapData();
PEETypes game_GetEETypes();