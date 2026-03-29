#pragma once
#include <windows.h>
#include "EETwa.h"

void pls_OnUnitIteration(Unit unit);
void pls_OnUnitDestory(Unit unit);
int8_t pls_IsUnitAttacked(Unit unit);
void pls_OnInit(PVOID remoteMethod);