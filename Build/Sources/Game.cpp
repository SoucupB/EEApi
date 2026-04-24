#include "Game.h"
#include <unordered_map>
#include <stdint.h>
#include <windows.h>
#include "MapData.h"
#include "EETypes.h"
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

PPlayerState game_GetPlayerState() {
  return game->plyState;
}

PMapData game_GetMapData() {
  return game->mapData;
}

void game_PlayerState_Delete(PGame game) {
  if(!game->plyState) {
    return ;
  }
  delete game->plyState->unitsHealth;
  free(game->plyState);
  game->plyState = NULL;
}

void game_MapData_Delete(PGame game) {
  if(game->mapData) {
    return ;
  }
  delete game->mapData->tiles;
  free(game->mapData->planeMap);
  game->mapData = NULL;
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
  free(*self);
  *self = NULL;
}

void game_Init() {
  game_Delete(&game);
  game = (PGame)malloc(sizeof(Game));
  game_PlayerState_Init(game);
  game_MapData_Init(game);
}