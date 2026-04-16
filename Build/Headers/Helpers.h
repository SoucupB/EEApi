#pragma once
#include <Windows.h>
#include <stdint.h>
#include "EETwaTypes.h"

PVOID __cdecl help_New(size_t size);
void help_UnitMove(PVOID unit, Point point, UnitAction currentAction);
void __cdecl help_MoveToTarget(PVOID unit, PVOID target);
void __cdecl help_ConvertTarget(PVOID unit, PVOID target);
void helper_Convert(PVOID src, PVOID dst);
void __cdecl help_Delete(PVOID pointer);