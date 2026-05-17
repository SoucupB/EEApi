#include "Game.h"
#include <unordered_map>
#include <stdint.h>
#include <windows.h>
#include "MapData.h"
#include "EETypes.h"
#include "TimerHelperPrivate.h"
#include "EETypesStructPrivate.h"
#include "Offset.h"

using namespace std;

static PGame game;

void game_PlayerState_Init(PGame game) {
  game->plyState = (PlayerState *)malloc(sizeof(PlayerState));
  game->plyState->unitsHealth = new unordered_map<PVOID, UnitHealth>();
  game->plyState->method = NULL;
}

void game_MapData_Init(PGame game) {
  game->mapData = (PMapData)malloc(sizeof(MapData));
  game->mapData->tiles = new vector<TileStruct>();
  game->mapData->planeMap = (TilePlaneMap *)malloc(sizeof(TilePlaneMap));
  memset(game->mapData->planeMap, 0, sizeof(TilePlaneMap));
}

void game_Resources_Init(PGame game) {
  game->resourceManager = (PResourceManager)malloc(sizeof(ResourceManager));
  game->resourceManager->resourcesRefs = new unordered_map<PVOID, uint8_t>();
}

void game_Player_Init(PGame game) {
  game->players = (PPlayers)malloc(sizeof(Players));
  game->players->playerData = new unordered_map<PVOID, uint8_t>();
}


PEmpireEarthHook game_EmpHook_Init(PGame game) {
  game->empHook = (PEmpireEarthHook)malloc(sizeof(EmpireEarthHook));
  game->empHook->onInitFlag = 0;
  game->empHook->hasIterationBeenExecuted = 0;
}

void game_EmpHook_Delete(PGame game) {
  if(!game->empHook) {
    return ;
  }
  free(game->empHook);
  game->empHook = NULL;
}

void game_EETwa_Init(PGame game) {
  game->eeTwa = (PEETwa)malloc(sizeof(EETwa));
  memset(game->eeTwa, 0, sizeof(EETwa));
  for(size_t i = 0; i < sizeof(game->eeTwa->unitPresence) / sizeof(unordered_map<PVOID, uint8_t> *); i++) {
    game->eeTwa->unitPresence[i] = new unordered_map<PVOID, uint8_t>();
    game->eeTwa->simpleUnitPresence[i] = new unordered_map<PVOID, uint8_t>();
  }
  game->eeTwa->playersCount = (int8_t)(sizeof(game->eeTwa->unitPresence) / sizeof(unordered_map<PVOID, uint8_t> *));
  game->eeTwa->all_players = PLAYER_ALL;
  game->eeTwa->playerIndex = 1;
  game->eeTwa->neutralPlayer = 0;
  game->eeTwa->shouldCostBeReduced = 0;
  game->eeTwa->timers = tmr_Init();
}

PPlayerState game_GetPlayerState() {
  return game->plyState;
}

PMapData game_GetMapData() {
  return game->mapData;
}

PEETypes game_GetEETypes() {
  return game->types;
}

PEETwa game_EETwa() {
  return game->eeTwa;
}

PPlayers game_Players() {
  return game->players;
}

PEmpireEarthHook game_EmpHook() {
  return game->empHook;
}

PResourceManager game_GetResourcesManager() {
  return game->resourceManager;
}

void game_PlayerState_Delete(PGame game) {
  if(!game->plyState) {
    return ;
  }
  delete game->plyState->unitsHealth;
  free(game->plyState);
  game->plyState = NULL;
}

void game_Players_Delete(PGame game) {
  if(!game->players) {
    return ;
  }
  delete game->players->playerData;
  free(game->players);
  game->players = NULL;
}

void game_EETwa_Delete(PGame game) {
  if(!game->eeTwa) {
    return ;
  }
  for(size_t i = 0, c = game->eeTwa->playersCount; i < c; i++) {
    if(game->eeTwa->unitPresence[i]) {
      delete game->eeTwa->unitPresence[i];
    }
    if(game->eeTwa->simpleUnitPresence[i]) {
      delete game->eeTwa->simpleUnitPresence[i];
    }
  }
  tmrs_Delete(game->eeTwa->timers);
  free(game->eeTwa);
  game->eeTwa = NULL;
}

void game_MapData_Delete(PGame game) {
  if(!game->mapData) {
    return ;
  }
  delete game->mapData->tiles;
  free(game->mapData->planeMap);
  game->mapData = NULL;
}

void game_EETypes_Init(PGame game) {
  game->types = (PEETypes)malloc(sizeof(EETypes));
  game->types->classTreeStructure = new map<UnitClassType, map<UnitType, uint8_t> >();
  game->types->neutralClassTreeStructure = new map<NeutralClassType, map<NeutralUnitType, uint8_t> >();
  game->types->parentsClass = new map<UnitType, UnitClassType>();
  game->types->neutralParentsClass = new map<NeutralUnitType, NeutralClassType>();
  game->types->unitTemplatePointers = new map<UnitType, PVOID>();
  game->types->abilityPointers = new map<UnitType, vector<AbilityTypes> >();
}

void game_EETypes_Delete(PGame game) {
  if(!game->types) {
    return ;
  }
  delete game->types->classTreeStructure;
  delete game->types->parentsClass;
  delete game->types->neutralClassTreeStructure;
  delete game->types->neutralParentsClass;
  delete game->types->unitTemplatePointers;
  delete game->types->abilityPointers;
  free(game->types);
  game->types = NULL;
}

void game_Resources_Delete(PGame game) {
  if(!game->resourceManager) {
    return ;
  }
  delete game->resourceManager->resourcesRefs;
  free(game->resourceManager);
  game->resourceManager = NULL;
}

PGame game_Reference() {
  return game;
}

void game_Delete(PGame *self) {
  if(!*self) {
    return ;
  }
  game_PlayerState_Delete(*self);
  game_MapData_Delete(*self);
  game_EETypes_Delete(*self);
  game_Resources_Delete(*self);
  game_EETwa_Delete(*self);
  game_Players_Delete(*self);
  game_EmpHook_Delete(*self);
  free(*self);
  *self = NULL;
}

void game_Init() {
  game_Delete(&game);
  game = (PGame)malloc(sizeof(Game));
  game_PlayerState_Init(game);
  game_MapData_Init(game);
  game_EETypes_Init(game);
  game_Resources_Init(game);
  game_EETwa_Init(game);
  game_Player_Init(game);
  game_EmpHook_Init(game);
}