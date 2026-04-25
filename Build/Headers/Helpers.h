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
void helper_CastPoint(PVOID unit, Point target, Ability ability);
void help_MoveSecondMethod(PVOID unit, Point target);
PVOID help_GetMapPointer();
size_t help_Map_TileCount(PVOID mapPointer);
PVOID help_Map_TilePointer(PVOID mapPointer);
void helper_CastAbility(PVOID unit, Point target, Ability ability);