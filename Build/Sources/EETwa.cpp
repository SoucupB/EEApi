#include "EETwa.h"
#include <unordered_map>
#include <vector>
#include <stdarg.h>
#include "PlayerState.h"
#include "MapData.h"
#include "LibManager.h"
#include "Game.h"
#include "Unit.h"

static PTimerHelper timers;

void bt_OnUnitDestroy(Unit unit);
void bt_OnInit();
void bt_OnFrame();

void eeta_FileClean();
void eeTa_RebuildDTs();

using namespace std;

void eeTa_Clear() {
  tmrs_Delete(timers);
}

void eeTa_RebuildExtraDataStructure() {
  eeTa_Clear();
  eeTa_RebuildDTs();
}

void eeTa_MoveTo(Unit src, Unit dst) {
  help_MoveToTarget(eeTa_Unit_Reference(src), eeTa_Unit_Reference(dst));
}

Unit eeTa_Unit_Null() {
  return (Unit) {
    ._payload = NULL
  };
}

PVOID eeTa_Unit_Reference(Unit unit) {
  return unit._payload;
}

uint8_t eeTa_NeutralPlayer() {
  PEETwa eeTwa = game_EETwa();
  return eeTwa->neutralPlayer;
}

uint8_t eeTa_IsNeutral(Unit unit) {
  PEETwa eeTwa = game_EETwa();
  return eeTa_Player(unit) == eeTwa->neutralPlayer;
}

// Money pointer is at ["EE-AOC.exe"+530DB8 + 0xAFC]

void __cdecl eeTa_OnUnitFrame(Unit unit) {
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;
  int8_t playerTeam = eeTa_Player(unit);
  if(playerTeam < 0 || playerTeam >= 24) {
    return ;
  }
  eeTwa->playerPresence[playerTeam] = 1;
  if(eeTa_IsUnitDead(unit)) {
    unitPresence[playerTeam]->erase(unit._payload);
    unitPresence[eeTwa->all_players]->erase(unit._payload);
    bt_OnUnitDestroy(unit);
    return ;
  }
  (*unitPresence[playerTeam])[unit._payload] = 1;
  (*unitPresence[eeTwa->all_players])[unit._payload] = 1;
  pls_ProcessHealth(unit._payload);
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

void eeTa_Unit_CastPoint(Unit src, Point target, Ability ability) {
  helper_CastPoint(eeTa_Unit_Reference(src), target, ability);
}

uint8_t eeTa_CurrentEnergy(Unit unit) {
  uint8_t *energyPointer = (uint8_t *)util_Pointer(eeTa_Unit_Reference(unit), 0x2D4, INT8_T_TYPE);
  return *energyPointer;
}

void __cdecl eeTa_OnUnitDeath(Unit unit) {
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;
  for(int8_t i = 0; i < 24; i++) {
    unitPresence[i]->erase(unit._payload);
  }
  bt_OnUnitDestroy(unit);
}

void eeTa_BuildUnit(Unit building, PVOID unitType) {
  if(eeTa_CurrentPopulation() > eeTa_TotalPop()) {
    return ;
  }
  int32_t __thiscall (*method)(PVOID, PVOID, PVOID) = (int32_t __thiscall (*)(PVOID, PVOID, PVOID)) ((uint8_t *)lib_BaseAddress() + 0x1F5F97);
  PEETwa eeTwa = game_EETwa();
  eeTwa->shouldCostBeReduced = 1;
  method(building._payload, unitType, 0);
  eeTwa->shouldCostBeReduced = 0;
}

int32_t eeTa_UnitPopulation(Unit unit) {
  PVOID unitTypeStruct = util_Pointer(unit._payload, 0x2C, POINTER_TYPE);
  PVOID callerMethods = util_Pointer(unitTypeStruct, 0x0, POINTER_TYPE);
  PVOID callee = util_Pointer(callerMethods, 0x6C, POINTER_TYPE);
  int32_t __fastcall (*method)(PVOID) = (int32_t __fastcall (*)(PVOID)) ((uint8_t *)callee);

  return method(unitTypeStruct);
}

PVOID eeTa_GetPlayer() {
  return util_Pointer(lib_BaseAddress(), 0x530DB8, POINTER_TYPE);
}

int32_t eeTa_CurrentPopulation() {
  PVOID unitTypeStruct = util_Pointer(lib_BaseAddress(), 0x530DB8, POINTER_TYPE);
  int32_t *unitPop = (int32_t *)util_Pointer(unitTypeStruct, 0xB14, INT32_T_TYPE);
  return *unitPop;
}

int32_t eeTa_OnUnitBuy(long double resources, int32_t (*method)(long double)) {
  PEETwa eeTwa = game_EETwa();
  if(eeTwa->shouldCostBeReduced) {
    return (int32_t)(method(resources) * 0.05f);
  }
  return method(resources);
}

int8_t eeTa_AreAllies(uint8_t plySrc, uint8_t plyDst) {
  PVOID _1 = util_Pointer(lib_BaseAddress(), 0x530DB4 + 0x4 * plySrc, POINTER_TYPE);
  if(!_1) {
    return 0;
  }
  uint8_t *_2 = (uint8_t *)util_Pointer(_1, 0x4 * plyDst + 0x9DC, INT8_T_TYPE);

  return !*_2;
}

void eeTa_SetCvCAggression(uint8_t botIndex, float aggression) {
  PVOID _1 = util_Pointer(lib_BaseAddress(), 0x530DB4 + 0x4 * botIndex, POINTER_TYPE);
  if(!_1) {
    return ;
  }
  float *_2 = (float *)util_Pointer(_1, 0xBF0, FLOAT_TYPE);
  *_2 = aggression;
} 

vector<Unit> eeTa_Filter(vector<Unit> &units, uint8_t (*method)(Unit)) {
  vector<Unit> filteredUnits;
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(method(units[i])) {
      filteredUnits.push_back(units[i]);
    }
  }
  return filteredUnits;
}

int32_t eeTa_TotalPop() {
  PVOID unitTypeStruct = util_Pointer(lib_BaseAddress(), 0x530DB8, POINTER_TYPE);
  PVOID callerMethods = util_Pointer(unitTypeStruct, 0x0, POINTER_TYPE);
  PVOID popMethod = util_Pointer(callerMethods, 0x7C, POINTER_TYPE);
  int32_t __fastcall (*method)(PVOID, PVOID, PVOID) = (int32_t __fastcall (*)(PVOID, PVOID, PVOID)) ((uint8_t *)popMethod);

  return method(unitTypeStruct, NULL, NULL);
}

int8_t eeTa_IsIdle(Unit building) {
  return eeTa_CurrentlyBuilding(building) == IDLE;
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

vector<PVOID> eeTa_UnitTypes(Unit building) {
  return vector<PVOID>();
}

PVOID _eeTa_EpochStruct(PVOID building, PVOID unitType) {
  size_t *buildingMetaData = (size_t *)util_Pointer((PVOID)building, 0x18, POINTER_TYPE);
  size_t *epochStruct = (size_t *)util_Pointer((PVOID)buildingMetaData, 0x9CC, POINTER_TYPE);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)lib_BaseAddress() + 0x18A4);

  return method((PVOID)epochStruct, unitType);
}

int32_t eeTa_Buildables(Unit unit) {
  size_t *typeMetaPointer = (size_t *)util_Pointer((PVOID)unit._payload, 0x2C, POINTER_TYPE);
  int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0x196DFF);
  return method(typeMetaPointer);
}

vector<int32_t> eeTa_AllBuildableTypes(Unit unit) {
  size_t *typeMetaPointer = (size_t *)util_Pointer((PVOID)unit._payload, 0x2C, POINTER_TYPE);
  size_t *buildableTypes = (size_t *)util_Pointer((PVOID)typeMetaPointer, 0x30, POINTER_TYPE);
  vector<int32_t> types;
  if(!buildableTypes) {
    return types;
  }

  int32_t totalBuildables = eeTa_Buildables(unit);
  for(int32_t i = 0; i < totalBuildables; i++) {
    if(eeTa_CanBuild(unit, (PVOID)buildableTypes[i])) {
      types.push_back(buildableTypes[i]);
    }
  }

  return types;
}

int8_t eeTa_IsBuildingComplete(Unit unit) {
  int8_t *isBuildingRef = (int8_t *)util_Pointer((PVOID)unit._payload, 0x34C, INT8_T_TYPE);
  
  return *isBuildingRef;
}

__declspec(dllexport) uint8_t eeTa_CanBuild(Unit building, PVOID type) {
  size_t *epochStruct = (size_t *)_eeTa_EpochStruct(building._payload, type);
  if(!epochStruct) {
    return 0;
  }

  size_t *checkMethod = (size_t *)util_Pointer((PVOID)(epochStruct[0]), 0x4, POINTER_TYPE);
  int8_t __thiscall (*method)(PVOID) = (int8_t __thiscall (*)(PVOID)) ((uint8_t *)checkMethod);

  return method((PVOID)epochStruct);
}

int8_t *eeTa_PlayerIDs() {
  PEETwa eeTwa = game_EETwa();
  return eeTwa->playerPresence;
}

int8_t eeTa_PlayerCount() {
  int8_t total = 0;
  PEETwa eeTwa = game_EETwa();
  for(size_t i = 0; i < 20; i++) {
    if(eeTwa->playerPresence[i]) {
      total++;
    }
  }
  return total;
}

void eeTa_RebuildDTs() {
  game_Init();
  eeta_FileClean();
  eeTypes_InitUnits();
  timers = tmr_Init();
}

void eeTa_OnInit() {
  eeTa_Map_Init();
  bt_OnInit();
}

UnitType eeTa_UnitType(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer((PVOID)unit._payload, 0x2C, POINTER_TYPE);
  return (UnitType)*(int32_t *)util_Pointer((PVOID)unitMetaData, 0x1E4, INT32_T_TYPE);
}

UnitType eeTa_EETypes_UnitType(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer((PVOID)unit._payload, 0x2C, POINTER_TYPE);
  return (UnitType)*(int32_t *)util_Pointer((PVOID)unitMetaData, 0x1E4, INT32_T_TYPE);
}

void eeTa_OnFrame() {
  PEETwa eeTwa = game_EETwa();
  bt_OnFrame();
  tmr_OnFrame(timers);
  eeTwa->frames++;
}

void eeTa_AddFrameMethod(TimeAtom atom) {
  tmrs_AddMethod(timers, atom);
}

int8_t eeTa_IsBuilding(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer((PVOID)unit._payload, 0x2C, POINTER_TYPE);
  size_t *callerStruct = (size_t *)util_Pointer((PVOID)unitMetaData[0], 0xB8, POINTER_TYPE);

  return (size_t)callerStruct == (size_t)lib_BaseAddress() + 0x20FD9D;
}

PVOID eeTa_CreateDestionationPointer() {
  size_t totalSize = 0x38;
  PVOID response = help_New(totalSize);
  memset(response, 0, totalSize);
  return response;
}

PVOID eeTa_SetPlayers(PVOID unit) {
  PVOID selectedUnits = help_New(sizeof(PVOID));
  memcpy(selectedUnits, &unit, sizeof(PVOID));
  return selectedUnits;
}

int8_t eeTa_IsUnit(Unit unit) {
  return !eeTa_IsBuilding(unit);
}

Point eeTa_GetDestinationCommand(Unit unit) {
  PVOID commandPointer = util_Pointer(unit._payload, 0x1C8, POINTER_TYPE);
  if(!commandPointer) {
    return (Point) {
      .x = -1.0f,
      .y = -1.0f
    };
  }
  PVOID metaPointer = util_Pointer(commandPointer, 0x34, POINTER_TYPE);
  float *positionPointer = (float *)util_Pointer(metaPointer, 0x48, FLOAT_TYPE);

  return (Point) {
    .x = positionPointer[0],
    .y = positionPointer[1]
  };
}

TilePoint eeTa_Unit_TilePosition(Unit unit) {
  int32_t x = *(int32_t *)util_Pointer(eeTa_Unit_Reference(unit), 0x1C, INT32_T_TYPE);
  int32_t y = *(int32_t *)util_Pointer(eeTa_Unit_Reference(unit), 0x20, INT32_T_TYPE);
  return (TilePoint) {
    .x = x,
    .y = y
  };
}

int32_t eeTa_CurrentlyBuilding(Unit building) {
  return *(int32_t *)util_Pointer((PVOID)building._payload, 0x260, INT32_T_TYPE);
}

int32_t eeTa_CurrentHp(Unit unit) {
  return *(int32_t *)util_Pointer((PVOID)unit._payload, 0x3C, INT32_T_TYPE);
}

int8_t eeTa_IsUnitDead(Unit unit) {
  return !eeTa_CurrentHp(unit);
}

int64_t eeTa_CurrentFrame() {
  PEETwa eeTwa = game_EETwa();
  return eeTwa->frames;
}

Point eeTa_CurrentPosition(Unit unit) {
  float *x = (float *)util_Pointer(unit._payload, 0x13C, FLOAT_TYPE);
  float *y = (float *)util_Pointer(unit._payload, 0x14C, FLOAT_TYPE);
  return (Point) {
    .x = *x,
    .y = *y
  };
}

int8_t eeTa_IsUnitIdle(Unit unit) {
  return !util_Pointer(unit._payload, 0x1F0, POINTER_TYPE);
}

PVOID eeTa_Unit_Sample(int8_t player) {
  vector<Unit> units = unit_GetUnits(player);
  if(!units.size()) {
    return NULL;
  }

  return units[rand() % units.size()]._payload;
}

int8_t eeTa_PlayerIndex() {
  return *(int8_t *)util_Pointer(lib_BaseAddress(), 0x5318C4, POINTER_TYPE);
}

uint8_t eeTa_ShouldOnInitExecute() {
  return eeTa_PlayerCount() != 0;
}

int8_t eeTa_Player(Unit unit) {
  PVOID nextStruct = util_Pointer((PVOID)unit._payload, 0x18, POINTER_TYPE);
  return *(uint8_t *)util_Pointer((PVOID)nextStruct, 0x45C, INT8_T_TYPE);
}

uint8_t eeTa_Tile_IsWater(TilePoint self) {
  return map_Tile_IsWater(self);
}