#include "Game.h"
#include <unordered_map>
#include <stdint.h>
#include <windows.h>
using namespace std;

typedef struct UnitHealth_t {
  int8_t isBeingAttacked;
  int32_t health;
} UnitHealth;

typedef struct PlayerState_t {
  unordered_map<PVOID, UnitHealth> unitsHealth;
  PVOID method;
} PlayerState;

typedef struct Game_t {
  PlayerState plyState;

} Game;

typedef Game *PGame;

static PGame game;

void game_Init() {
  game = (PGame)malloc(sizeof(Game));
}