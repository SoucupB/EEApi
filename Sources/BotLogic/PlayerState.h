#pragma once
#include <windows.h>
#include "EETwa.h"

void pls_OnUnitIteration(PVOID unit);
void pls_OnUnitDestory(PVOID unit);
int8_t pls_IsUnitAttacked(PVOID unit);
void pls_OnInit(PVOID remoteMethod);