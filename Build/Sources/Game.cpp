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

void game_Init() {
  game = (PGame)malloc(sizeof(Game));
}