#pragma once
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "Geometry.h"
#include "InjectUtilities.h"
#include "Helpers.h"
#include "TimerHelper.h"
#include "EETypes.h"

vector<Unit> eeTa_Buildings(int8_t player);
vector<PVOID> eeTa_UnitTypes(Unit building);
void __cdecl eeTa_OnUnitFrame(Unit unit);
void eeTa_OnFrame();
void eeTa_BuildUnit(Unit building, PVOID unitType);
int32_t eeTa_CurrentlyBuilding(Unit building);
vector<Unit> eeTa_IdleBuildings(int8_t player);
int64_t eeTa_CurrentFrame();
void eeTa_OnUnitDeath(Unit unit);
int8_t eeTa_Player(Unit unit);
PVOID eeTa_Unit_Reference(Unit unit);
void eeTa_Printf(const char *format, ...);
int8_t eeTa_SelfPlayer();
int8_t eeTa_AllPlayers();
PVOID _eeTa_EpochStruct(PVOID building, PVOID unitType);
uint8_t eeTa_CanBuild(Unit building, PVOID type);
int32_t eeTa_Buildables(Unit unit);
vector<int32_t> eeTa_AllBuildableTypes(Unit unit);
int8_t eeTa_IsBuildingComplete(Unit unit);
Point eeTa_GetDestinationCommand(Unit unit);
PVOID eeTa_Unit_Sample(int8_t player);
int32_t eeTa_UnitPopulation(Unit unit);
int32_t eeTa_CurrentPopulation();
int32_t eeTa_TotalPop();
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
void eeTa_FilePrintf(const char *format, ...);

uint8_t eeTa_ShouldOnInitExecute();

uint8_t eeTa_Tile_IsWater(TilePoint self);
void eeTa_Map_PrintBitMap();