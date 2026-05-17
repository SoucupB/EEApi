#pragma once
#include <Windows.h>
#include <stdint.h>
#include <vector>
#include "Geometry.h"

PVOID __cdecl help_New(size_t size);
void help_UnitMove(PVOID unit, Point point, UnitAction currentAction);
void __cdecl help_MoveToTarget(PVOID unit, PVOID target);
void __cdecl help_ConvertTarget(PVOID unit, PVOID target);
void driver_Convert(PVOID src, PVOID dst);
void __cdecl help_Delete(PVOID pointer);
void driver_CastPoint(PVOID unit, Point target, AbilityTypes ability);
void help_MoveSecondMethod(PVOID unit, Point target);
void driver_CastAbility(PVOID unit, Point target, AbilityTypes ability);
void driver_RepairBuilding(PVOID unit, PVOID building);
void driver_Citizen_Gather(PVOID unit, PVOID resource);
void driver_Unit_Command(PVOID unit, Point position, UnitAction action);
PVOID driver_Player_FromUnit(PVOID unit);
void driver_CastAbility_Remade(PVOID unit, Point target, AbilityTypes ability);
void driver_Convert_Remade(PVOID unit, PVOID target);
void driver_Transport_Load(vector<PVOID> &units, PVOID transport);
size_t driver_Transport_UnitsCount(PVOID unit);
PVOID driver_Transport_Ref(PVOID unit);
vector<Unit> unit_Transport_UnitsInside(Unit transport);
void driver_Building_Create(PVOID citizen, TilePoint position, PVOID type);
void driver_Transport_Unload(PVOID transport, TilePoint tile);
PVOID driver_Building_GetTemplate(size_t index);
PVOID driver_TechNode(TechTree tree, AbilityTypes ability);
PVOID driver_AbilityPointer(PVOID manager, size_t abilityIndex);
int32_t driver_AbilityEnergy(PVOID reference);
void driver_CastAbility_Target(PVOID unit, PVOID target, Point targetPoint, AbilityTypes ability);