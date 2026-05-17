#pragma once
#include "EETypesStructPrivate.h"

void __cdecl eeTa_OnUnitFrame(Unit unit);
void eeTa_OnFrame();
void eeTa_OnUnitDeath(Unit unit);
void eeTa_OnInit();
int32_t eeTa_OnUnitBuy(long double resources, int32_t (*method)(long double));
uint8_t eeTa_ShouldOnInitExecute();
void eeTa_Map_PrintBitMap();