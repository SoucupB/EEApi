#include "Game.h"
#include <unordered_map>
#include <stdint.h>
#include <windows.h>
#include "MapData.h"
#include "EETypes.h"
using namespace std;

typedef struct UnitHealth_t {
  int8_t isBeingAttacked;
  int32_t health;
} UnitHealth;

typedef struct PlayerState_t {
  unordered_map<PVOID, UnitHealth> unitsHealth;
  PVOID method;
} PlayerState;

typedef struct MapData_t {
  vector<TileStruct> tiles;
  TilePlaneMap planeMap;
} MapData;

typedef struct EETypes_t {
  map<UnitClassType, map<UnitType, uint8_t> > classTreeStructure;
} EETypes;

typedef struct Game_t {
  PlayerState plyState;
  MapData mapData;
  EETypes types;
} Game;

typedef Game *PGame;

static PGame game;

void game_PlayerState_Init(PGame game) {
  new (&game->plyState.unitsHealth) unordered_map<PVOID, UnitHealth>();
  game->plyState.method = NULL;
}

void game_MapData_Init(PGame game) {
  new (&game->mapData.tiles) vector<TileStruct>;
  memset(&game->mapData.planeMap, 0, sizeof(TilePlaneMap));
}

void game_PlayerState_Delete(PGame game) {
  game->plyState.unitsHealth.~unordered_map();
  game->plyState.method = NULL;
}

void game_MapData_Delete(PGame game) {
  game->mapData.tiles.~vector();
}

PGame game_Reference() {
  return game;
}

void game_Delete(PGame self) {
  if(!self) {
    return ;
  }
  game_PlayerState_Delete(self);
  game_MapData_Delete(self);
  free(self);
}

void game_Init() {
  game_Delete(game);
  game = (PGame)malloc(sizeof(Game));
  game_PlayerState_Init(game);
  game_MapData_Init(game);
}