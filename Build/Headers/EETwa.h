#pragma once
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "InjectUtilities.h"
#include "EETwaTypes.h"
#include "Helpers.h"
#include "TimerHelper.h"

typedef struct Unit_t {
  PVOID _payload;
} Unit;

vector<PVOID> eeTa_Buildings(int8_t player);
vector<PVOID> eeTa_UnitTypes(Unit building);
void __cdecl eeTa_OnUnitFrame(Unit unit);
vector<Unit> eeTa_Units(int8_t player);
void eeTa_Clean();
void eeTa_OnFrame();
void eeTa_BuildUnit(Unit building, PVOID unitType);
int32_t eeTa_CurrentlyBuilding(Unit building);
vector<Unit> eeTa_IdleBuildings(int8_t player);
int64_t eeTa_CurrentFrame();
int8_t eeTa_IsBuilding(Unit unit);
void eeTa_OnUnitDeath(Unit unit);
int8_t eeTa_Player(Unit unit);
UnitTypeDef eeTa_UnitType(Unit unit);
int32_t eeTa_CurrentHp(Unit unit);
int8_t eeTa_IsUnitDead(Unit unit);
int8_t eeTa_IsUnit(Unit unit);
void eeTa_Printf(const char *format, ...);
int8_t eeTa_SelfPlayer();
int8_t eeTa_AllPlayers();
PVOID _eeTa_EpochStruct(PVOID building, PVOID unitType);
uint8_t eeTa_CanBuild(Unit building, PVOID type);
int32_t eeTa_Buildables(Unit unit);
vector<int32_t> eeTa_AllBuildableTypes(Unit unit);
int8_t eeTa_IsBuildingComplete(Unit unit);
Point eeTa_GetDestinationCommand(Unit unit);
Point eeTa_CurrentPosition(Unit unit);
PVOID eeTa_Unit_Sample(int8_t player);
int8_t eeTa_IsUnitIdle(Unit unit);
int32_t eeTa_UnitPopulation(Unit unit);
int32_t eeTa_CurrentPopulation();
int32_t eeTa_TotalPop();
int8_t eeTa_AreAllies(uint8_t plySrc, uint8_t plyDst);
void eeTa_SetCvCAggression(uint8_t botIndex, float aggression);
void eeTa_OnInit();
void eeTa_AddFrameMethod(TimeAtom atom);
vector<Unit> eeTa_Filter(vector<Unit> &units, uint8_t (*method)(Unit));
uint8_t eeTa_IsNeutral(Unit unit);
int8_t eeTa_IsIdle(Unit building);
uint8_t eeTa_NeutralPlayer();
PVOID eeTa_GetPlayer();
int32_t eeTa_OnUnitBuy(long double resources, int32_t (*method)(long double));
int8_t eeTa_PlayerCount();
int8_t *eeTa_PlayerIDs(); // 30 elements max
int8_t eeTa_PlayerIndex(); // needs to be fixed.

uint8_t eeTa_ShouldOnInitExecute();