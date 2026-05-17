#pragma once
#include <Windows.h>
#include <stdint.h>
#include <vector>
#include "Geometry.h"

PVOID __cdecl help_New(size_t size);
void help_UnitMove(PVOID unit, Point point, UnitAction currentAction);
void __cdecl help_MoveToTarget(PVOID unit, PVOID target);
void __cdecl help_ConvertTarget(PVOID unit, PVOID target);
void helper_Convert(PVOID src, PVOID dst);
void __cdecl help_Delete(PVOID pointer);
void helper_CastPoint(PVOID unit, Point target, AbilityTypes ability);
void help_MoveSecondMethod(PVOID unit, Point target);
void helper_CastAbility(PVOID unit, Point target, AbilityTypes ability);
void helper_RepairBuilding(PVOID unit, PVOID building);
void helper_Citizen_Gather(PVOID unit, PVOID resource);
void helper_Unit_Command(PVOID unit, Point position, UnitAction action);
PVOID helper_Player_FromUnit(PVOID unit);
void helper_CastAbility_Remade(PVOID unit, Point target, AbilityTypes ability);
void helper_Convert_Remade(PVOID unit, PVOID target);
void helper_Transport_Load(vector<PVOID> &units, PVOID transport);
size_t helper_Transport_UnitsCount(PVOID unit);
PVOID helper_Transport_Ref(PVOID unit);
vector<Unit> unit_Transport_UnitsInside(Unit transport);
void helper_Building_Create(PVOID citizen, TilePoint position, PVOID type);
void helper_Transport_Unload(PVOID transport, TilePoint tile);
PVOID helper_Building_GetTemplate(size_t index);
PVOID helper_TechNode(TechTree tree, AbilityTypes ability);
PVOID helper_AbilityPointer(PVOID manager, size_t abilityIndex);
int32_t helper_AbilityEnergy(PVOID reference);
void helper_CastAbility_Target(PVOID unit, PVOID target, Point targetPoint, AbilityTypes ability);