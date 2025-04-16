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
vector<PVOID> eeTa_UnitTypes(PVOID building);
void __cdecl eeTa_OnUnitFrame(PVOID unit);
vector<PVOID> eeTa_Units(int8_t player);
void eeTa_Clean();
void eeTa_OnFrame();
void eeTa_BuildUnit(PVOID building, PVOID unitType);
int32_t eeTa_CurrentlyBuilding(PVOID building);
vector<PVOID> eeTa_IdleBuildings(int8_t player);
int64_t eeTa_CurrentFrame();
int8_t eeTa_IsBuilding(PVOID unit);
void eeTa_OnUnitDeath(PVOID unit);
int8_t eeTa_Player(PVOID unit);
UnitTypeDef eeTa_UnitType(PVOID unit);
int32_t eeTa_CurrentHp(PVOID unit);
int8_t eeTa_IsUnitDead(PVOID unit);
int8_t eeTa_IsUnit(PVOID unit);
void eeTa_Printf(const char *format, ...);
int8_t eeTa_SelfPlayer();
int8_t eeTa_AllPlayers();
PVOID _eeTa_EpochStruct(PVOID building, PVOID unitType);
uint8_t eeTa_CanBuild(PVOID building, PVOID type);
int32_t eeTa_Buildables(PVOID unit);
vector<int32_t> eeTa_AllBuildableTypes(PVOID unit);
int8_t eeTa_IsBuildingComplete(PVOID unit);
Point eeTa_GetDestinationCommand(PVOID unit);
Point eeTa_CurrentPosition(PVOID unit);
PVOID eeTa_Unit_Sample(int8_t player);
int8_t eeTa_IsUnitIdle(PVOID unit);
int32_t eeTa_UnitPopulation(PVOID unit);
int32_t eeTa_CurrentPopulation();
int32_t eeTa_TotalPop();
int8_t eeTa_AreAllies(uint8_t plySrc, uint8_t plyDst);
void eeTa_OnInit();
void eeTa_AddFrameMethod(TimeAtom atom);
vector<PVOID> eeTa_Filter(vector<PVOID> &units, uint8_t (*method)(PVOID));
uint8_t eeTa_IsNeutral(PVOID unit);
int8_t eeTa_IsIdle(PVOID building);
uint8_t eeTa_NeutralPlayer();
PVOID eeTa_GetPlayer();
int32_t eeTa_OnUnitBuy(long double resources, int32_t (*method)(long double));