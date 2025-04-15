#pragma once
#include <Windows.h>
#include <stdint.h>
#include "PartialStructs.h"

PVOID __cdecl help_New(size_t size);
PVOID __thiscall help_FillSelectedUnits(PVOID self, size_t _a, size_t _b, size_t _c);
// PVOID __thiscall help_
PVOID __fastcall help_SearchUnits(PVOID self);
MoveAction *help_GetAction(PVOID parent, Point pos, UnitAction action);
void __cdecl help_Delete(PVOID pointer);
void help_UnitMove(PVOID unit, Point point, UnitAction currentAction);
PVOID __thiscall help_Checker_4C2A3C(PVOID self, PVOID _1, PVOID _2, PVOID _3);
void __cdecl help_MoveToTarget(PVOID unit, PVOID target);

