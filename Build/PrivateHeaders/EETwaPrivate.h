#pragma once
#include "EETypesStructPrivate.h"

void eeTa_OnUnitFrame(Unit unit);
void eeTa_OnFrame();
void bt_OnUnitCreate(Unit unit);
void eeTa_OnUnitDeath(Unit unit);
void eeTa_OnInit();
int32_t eeTa_OnUnitBuy(long double resources, int32_t (*method)(long double));
uint8_t eeTa_ShouldOnInitExecute();
void eeTa_Map_PrintBitMap();
void eeTa_OnUnitCreate(PVOID unitBuffer);
void eeTa_OnMapInit();