#pragma once
#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <map>
#include "MapData.h"
#include "EETypes.h"
#include "TimerHelper.h"
#include "Player.h"

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
  map<NeutralClassType, map<NeutralUnitType, uint8_t> > *neutralClassTreeStructure;
  map<UnitType, UnitClassType> *parentsClass;
  map<NeutralUnitType, NeutralClassType> *neutralParentsClass;
  map<UnitType, PVOID> *unitTemplatePointers;
} EETypes;

typedef struct EETwa_t {
  unordered_map<PVOID, uint8_t> *unitPresence[24];
  int64_t frames;
  int8_t all_players;
  int8_t playerIndex;
  int8_t neutralPlayer;
  int8_t shouldCostBeReduced;
  int8_t playerPresence[30];
  int8_t playersCount;
  PTimerHelper timers;
} EETwa;

typedef struct Players_t {
  unordered_map<PVOID, uint8_t> *playerData;
  Player self;
  Player neutral;
} Players;

typedef struct ResourceManager_t {
  unordered_map<PVOID, uint8_t> *resourcesRefs;
} ResourceManager;

typedef struct EmpireEarthHook_t {
  uint8_t onInitFlag;
  uint8_t hasIterationBeenExecuted;
} EmpireEarthHook;

typedef EETwa *PEETwa;
typedef EETypes *PEETypes;
typedef MapData *PMapData;
typedef PlayerState *PPlayerState;
typedef ResourceManager *PResourceManager;
typedef Players *PPlayers;
typedef EmpireEarthHook *PEmpireEarthHook;

typedef struct Game_t {
  PPlayerState plyState;
  PMapData mapData;
  PEETypes types;
  PResourceManager resourceManager;
  PEETwa eeTwa;
  PPlayers players;
  PEmpireEarthHook empHook;
} Game;

typedef Game *PGame;

void game_Init();
PPlayerState game_GetPlayerState();
PMapData game_GetMapData();
PEETypes game_GetEETypes();
PEETwa game_EETwa();
PResourceManager game_GetResourcesManager();
PPlayers game_Players();
PEmpireEarthHook game_EmpHook();