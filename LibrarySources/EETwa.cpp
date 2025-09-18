#include "EETwa.h"
#include <unordered_map>
#include <vector>
#include "BotLogic.h"

static PTimerHelper timers;

using namespace std;
static unordered_map<PVOID, uint8_t> unitPresence[24];
static int64_t frames;
static int8_t all_players = 20;
static int8_t playerIndex = 1;
static int8_t neutralPlayer = 0;
static int8_t shouldCostBeReduced = 0;
static int8_t playerPresence[30];
__declspec(dllexport) char outputBuffer[2048];

void eeTa_Clean() {
  for(uint8_t i = 0; i < 24; i++) {
    unitPresence[i].clear();
  }
}

uint8_t eeTa_NeutralPlayer() {
  return neutralPlayer;
}

uint8_t eeTa_IsNeutral(Unit unit) {
  return eeTa_Player(unit) == neutralPlayer;
}

// Money pointer is at ["EE-AOC.exe"+530DB8 + 0xAFC]

void eeTa_Printf(const char *format, ...) {
  memset(outputBuffer, 0, sizeof(outputBuffer));
  va_list args;
  va_start(args, format);
  int32_t neededSize = vsnprintf(NULL, 0, format, args) + 1;
  va_end(args);
  va_start(args, format);
  vsnprintf(outputBuffer, neededSize, format, args);
  va_end(args);
}

void __cdecl eeTa_OnUnitFrame(Unit unit) {
  int8_t playerTeam = eeTa_Player(unit);
  if(playerTeam < 0 || playerTeam >= 24) {
    return ;
  }
  playerPresence[playerTeam] = 1;
  if(eeTa_IsUnitDead(unit)) {
    unitPresence[playerTeam].erase(unit._payload);
    unitPresence[all_players].erase(unit._payload);
    bt_OnUnitDestroy(unit);
    return ;
  }
  unitPresence[playerTeam][unit._payload] = 1;
  unitPresence[all_players][unit._payload] = 1;
  bt_OnUnitIteration(unit);
}

int8_t eeTa_AllPlayers() {
  return all_players;
}

vector<Unit> eeTa_Units(int8_t player) {
  vector<Unit> units;
  for(auto &it : unitPresence[player]) {
    if(!eeTa_IsUnitDead((Unit) {
      ._payload = it.first
    }) && eeTa_IsUnit((Unit) {
      ._payload = (PVOID)it.first
    })) {
      units.push_back((Unit) {
        ._payload = (PVOID)it.first
      });
    }
  }
  return units;
}

void __cdecl eeTa_OnUnitDeath(Unit unit) {
  for(int8_t i = 0; i < 24; i++) {
    unitPresence[i].erase(unit._payload);
  }
  bt_OnUnitDestroy(unit);
}

void eeTa_BuildUnit(Unit building, PVOID unitType) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return ;
  }
  if(eeTa_CurrentPopulation() > eeTa_TotalPop()) {
    return ;
  }
  int32_t __thiscall (*method)(PVOID, PVOID, PVOID) = (int32_t __thiscall (*)(PVOID, PVOID, PVOID)) ((uint8_t *)hModule + 0x1F5F97);
  shouldCostBeReduced = 1;
  method(building._payload, unitType, 0);
  shouldCostBeReduced = 0;
}

int32_t eeTa_UnitPopulation(Unit unit) {
  PVOID unitTypeStruct = util_Pointer(unit._payload, 0x2C, POINTER_TYPE);
  PVOID callerMethods = util_Pointer(unitTypeStruct, 0x0, POINTER_TYPE);
  PVOID callee = util_Pointer(callerMethods, 0x6C, POINTER_TYPE);
  int32_t __fastcall (*method)(PVOID) = (int32_t __fastcall (*)(PVOID)) ((uint8_t *)callee);

  return method(unitTypeStruct);
}

PVOID eeTa_GetPlayer() {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return NULL;
  }
  return util_Pointer(hModule, 0x530DB8, POINTER_TYPE);
}

int32_t eeTa_CurrentPopulation() {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return -1;
  }
  PVOID unitTypeStruct = util_Pointer(hModule, 0x530DB8, POINTER_TYPE);
  int32_t *unitPop = (int32_t *)util_Pointer(unitTypeStruct, 0xB14, INT32_T_TYPE);
  
  return *unitPop;
}

int32_t eeTa_OnUnitBuy(long double resources, int32_t (*method)(long double)) {
  if(shouldCostBeReduced) {
    return (int32_t)(method(resources) * 0.05f);
  }
  return method(resources);
}

int8_t eeTa_AreAllies(uint8_t plySrc, uint8_t plyDst) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  PVOID _1 = util_Pointer(hModule, 0x530DB4 + 0x4 * plySrc, POINTER_TYPE);
  if(!_1) {
    return 0;
  }
  uint8_t *_2 = (uint8_t *)util_Pointer(_1, 0x4 * plyDst + 0x9DC, INT8_T_TYPE);

  return !*_2;
}

void eeTa_SetCvCAggression(uint8_t botIndex, float aggression) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return ;
  }
  PVOID _1 = util_Pointer(hModule, 0x530DB4 + 0x4 * botIndex, POINTER_TYPE);
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
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return -1;
  }
  PVOID unitTypeStruct = util_Pointer(hModule, 0x530DB8, POINTER_TYPE);
  PVOID callerMethods = util_Pointer(unitTypeStruct, 0x0, POINTER_TYPE);
  PVOID popMethod = util_Pointer(callerMethods, 0x7C, POINTER_TYPE);
  int32_t __fastcall (*method)(PVOID) = (int32_t __fastcall (*)(PVOID)) ((uint8_t *)popMethod);

  return method(unitTypeStruct);
}

vector<PVOID> eeTa_Buildings(int8_t player) {
  vector<PVOID> buildingsPointer;
  for(auto &it : unitPresence[player]) {
    if(!eeTa_IsUnitDead((Unit) {
      ._payload = it.first
    }) && eeTa_IsBuilding((Unit) {
      ._payload = it.first
    })) {
      buildingsPointer.push_back(it.first);
    }
  }
  return buildingsPointer;
}

int8_t eeTa_IsIdle(Unit building) {
  return eeTa_CurrentlyBuilding(building) == IDLE;
}

vector<Unit> eeTa_IdleBuildings(int8_t player) {
  vector<Unit> buildingsPointer;
  for(auto &it : unitPresence[player]) {
    if(!eeTa_IsUnitDead((Unit) {
      ._payload = it.first
    }) && eeTa_IsBuildingComplete((Unit) {
      ._payload = it.first
    }) && eeTa_IsBuilding((Unit) {
      ._payload = it.first
    }) && eeTa_IsIdle((Unit) {
      ._payload = it.first
    })) {
      buildingsPointer.push_back((Unit) {
        ._payload = it.first
      });
    }
  }
  return buildingsPointer;
}

int8_t eeTa_SelfPlayer() {
  return playerIndex;
}

vector<PVOID> eeTa_UnitTypes(Unit building) {
  return vector<PVOID>();
}

PVOID _eeTa_EpochStruct(PVOID building, PVOID unitType) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return NULL;
  }
  size_t *buildingMetaData = (size_t *)util_Pointer((PVOID)building, 0x18, POINTER_TYPE);
  size_t *epochStruct = (size_t *)util_Pointer((PVOID)buildingMetaData, 0x9CC, POINTER_TYPE);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)hModule + 0x18A4);

  return method((PVOID)epochStruct, unitType);
}

int32_t eeTa_Buildables(Unit unit) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  size_t *typeMetaPointer = (size_t *)util_Pointer((PVOID)unit._payload, 0x2C, POINTER_TYPE);
  int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)hModule + 0x196DFF);
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
  return playerPresence;
}

int8_t eeTa_PlayerCount() {
  int8_t total = 0;
  for(size_t i = 0; i < 20; i++) {
    if(playerPresence[i]) {
      total++;
    }
  }
  return total;
}

void eeTa_OnInit() {
  eeTypes_OnInit();
  timers = tmr_Init();
  bt_OnInit();
}

UnitTypeDef eeTa_UnitType(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer((PVOID)unit._payload, 0x2C, POINTER_TYPE);
  return (UnitTypeDef)*(int32_t *)util_Pointer((PVOID)unitMetaData, 0x1E4, INT32_T_TYPE);
}

void eeTa_OnFrame() {
  tmr_OnFrame(timers);
  frames++;
}

void eeTa_AddFrameMethod(TimeAtom atom) {
  tmrs_AddMethod(timers, atom);
}

int8_t eeTa_IsBuilding(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer((PVOID)unit._payload, 0x2C, POINTER_TYPE);
  size_t *callerStruct = (size_t *)util_Pointer((PVOID)unitMetaData[0], 0xB8, POINTER_TYPE);

  return (size_t)callerStruct == (size_t)GetModuleHandleA("EE-AOC.exe") + 0x20FD9D;
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
  return frames;
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
  vector<Unit> units = eeTa_Units(player);
  if(!units.size()) {
    return NULL;
  }

  return units[rand() % units.size()]._payload;
}

int8_t eeTa_PlayerIndex() {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  return *(uint32_t *)util_Pointer(hModule, 0x5318C4, POINTER_TYPE);
}

uint8_t eeTa_ShouldOnInitExecute() {
  return eeTa_PlayerCount() != 0;
}

int8_t eeTa_Player(Unit unit) {
  PVOID nextStruct = util_Pointer((PVOID)unit._payload, 0x18, POINTER_TYPE);
  return *(uint8_t *)util_Pointer((PVOID)nextStruct, 0x45C, INT8_T_TYPE);
}