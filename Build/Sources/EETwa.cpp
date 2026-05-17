#include "EETwa.h"
#include <unordered_map>
#include <vector>
#include <stdarg.h>
#include "PlayerState.h"
#include "MapData.h"
#include "LibManager.h"
#include "Game.h"
#include "Unit.h"
#include "PlayerStatePrivate.h"
#include "PlayerPrivate.h"
#include "EETypesStructPrivate.h"
#include "EETwaPrivate.h"
#include "InjectUtilities.h"
#include "Helpers.h"
#include "Offset.h"

void bt_OnUnitDestroy(Unit unit);
void bt_OnInit();
void bt_OnFrame();

void eeta_FileClean();
void eeTa_RebuildDTs();

using namespace std;

void eeTa_RebuildExtraDataStructure() {
  eeTa_RebuildDTs();
}

uint8_t eeTa_NeutralPlayer() {
  PEETwa eeTwa = game_EETwa();
  return eeTwa->neutralPlayer;
}

// Money pointer is at ["EE-AOC.exe"+530DB8 + 0xAFC]

void eeTa_OnUnitDestroy(Unit unit) {
  pls_OnUnitDestory(unit);
  bt_OnUnitDestroy(unit);
}

void __cdecl eeTa_OnUnitFrame(Unit unit) {
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;
  int8_t playerTeam = eeTa_Player(unit);
  if(playerTeam < 0 || playerTeam >= 24) {
    return ;
  }
  eeTwa->playerPresence[playerTeam] = 1;
  if(unit_IsDead(unit)) {
    unitPresence[playerTeam]->erase(unit._payload);
    unitPresence[eeTwa->all_players]->erase(unit._payload);
    eeTa_OnUnitDestroy(unit);
    return ;
  }
  (*unitPresence[playerTeam])[unit._payload] = 1;
  (*unitPresence[eeTwa->all_players])[unit._payload] = 1;
  pls_ProcessHealth(unit._payload);
  ply_Inits(unit_Reference(unit));
}

int8_t eeTa_AllPlayers() {
  PEETwa eeTwa = game_EETwa();
  return eeTwa->all_players;
}

void eeTa_Map_Init() {
  map_Init();
}

void eeTa_Map_PrintBitMap() {
  size_t mapSizeInTiles;
  char **map = map_GetBitMap(&mapSizeInTiles);
  for(size_t i = 0; i < mapSizeInTiles; i++) {
    for(size_t j = 0; j < mapSizeInTiles; j++) {
      eeTa_FilePrintf("%d", map[i][j]);
    }
    eeTa_FilePrintf("\n");
  }
  map_BitMapDelete(map, mapSizeInTiles);
}

void __cdecl eeTa_OnUnitDeath(Unit unit) {
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;
  for(int8_t i = 0; i < 24; i++) {
    unitPresence[i]->erase(unit._payload);
  }
  bt_OnUnitDestroy(unit);
}

int32_t eeTa_OnUnitBuy(long double resources, int32_t (*method)(long double)) {
  PEETwa eeTwa = game_EETwa();
  if(eeTwa->shouldCostBeReduced) {
    return (int32_t)(method(resources) * 0.05f);
  }
  return method(resources);
}

void eeTa_SetCvCAggression(uint8_t botIndex, float aggression) {
  PVOID _1 = util_Pointer(lib_BaseAddress(), 0x530DB4 + 0x4 * botIndex, POINTER_TYPE);
  if(!_1) {
    return ;
  }
  float *_2 = (float *)util_Pointer(_1, 0xBF0, FLOAT_TYPE);
  *_2 = aggression;
}

void eeta_FileClean() {
  FILE* f = fopen("EETWa.log", "w");
  fclose(f);
}

void eeTa_FilePrintf(const char *format, ...) {
  FILE* f = fopen("EETWa.log", "a");
  if (!f) return;

  va_list args;
  va_start(args, format);
  vfprintf(f, format, args);
  va_end(args);

  fclose(f);
}

int8_t eeTa_SelfPlayer() {
  PEETwa eeTwa = game_EETwa();
  return eeTwa->playerIndex;
}

int8_t eeTa_PlayerCount() {
  return ply_PlayerCount();
}

void eeTa_RebuildDTs() {
  game_Init();
  eeta_FileClean();
  eeTypes_InitUnits();
}

void eeTa_OnInit() {
  eeTa_Map_Init();
  bt_OnInit();
}

void eeTa_OnFrame() {
  PEETwa eeTwa = game_EETwa();
  bt_OnFrame();
  tmr_OnFrame(eeTwa->timers);
  eeTwa->frames++;
}

void eeTa_AddFrameMethod(TimeAtom atom) {
  tmrs_AddMethod(game_EETwa()->timers, atom);
}

PVOID eeTa_SetPlayers(PVOID unit) {
  PVOID selectedUnits = help_New(sizeof(PVOID));
  memcpy(selectedUnits, &unit, sizeof(PVOID));
  return selectedUnits;
}

// Needs refactoring.
// Point eeTa_GetDestinationCommand(Unit unit) {
//   PVOID commandPointer = util_Pointer(unit._payload, 0x1C8, POINTER_TYPE);
//   if(!commandPointer) {
//     return (Point) {
//       .x = -1.0f,
//       .y = -1.0f
//     };
//   }
//   PVOID metaPointer = util_Pointer(commandPointer, 0x34, POINTER_TYPE);
//   float *positionPointer = (float *)util_Pointer(metaPointer, 0x48, FLOAT_TYPE);

//   return (Point) {
//     .x = positionPointer[0],
//     .y = positionPointer[1]
//   };
// }

int64_t eeTa_CurrentFrame() {
  PEETwa eeTwa = game_EETwa();
  return eeTwa->frames;
}

uint8_t eeTa_ShouldOnInitExecute() {
  return eeTa_PlayerCount() != 0;
}

int8_t eeTa_Player(Unit unit) {
  PVOID nextStruct = util_Pointer((PVOID)unit._payload, UNIT_PLAYER_OFFSET, POINTER_TYPE);
  return *(uint8_t *)util_Pointer((PVOID)nextStruct, UNIT_PLAYER_OFFSET_INDEX, INT8_T_TYPE);
}