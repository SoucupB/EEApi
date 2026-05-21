#pragma once
#include <Windows.h>
#include <stdint.h>
#include <vector>
#include "Geometry.h"

PVOID __cdecl driver_New(size_t size);
void __cdecl help_ConvertTarget(PVOID unit, PVOID target);
void __cdecl driver_Delete(PVOID pointer);
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
size_t driver_CanBuiltAt(PVOID player, TilePoint tile, size_t buildingTypeID);
size_t driver_CanBuiltAt_Complete(PVOID player, PVOID citizen, TilePoint tile, size_t buildingTypeID);