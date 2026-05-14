#pragma once
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "Geometry.h"
#include "InjectUtilities.h"
#include "Helpers.h"
#include "TimerHelper.h"
#include "EETypes.h"

vector<PVOID> eeTa_UnitTypes(Unit building);
void __cdecl eeTa_OnUnitFrame(Unit unit);
void eeTa_OnFrame();
int64_t eeTa_CurrentFrame();
void eeTa_OnUnitDeath(Unit unit);
int8_t eeTa_Player(Unit unit);
void eeTa_Printf(const char *format, ...);
int8_t eeTa_SelfPlayer();
int8_t eeTa_AllPlayers();
Point eeTa_GetDestinationCommand(Unit unit);
void eeTa_SetCvCAggression(uint8_t botIndex, float aggression);
void eeTa_OnInit();
void eeTa_AddFrameMethod(TimeAtom atom);
uint8_t eeTa_NeutralPlayer();
PVOID eeTa_GetPlayer();
int32_t eeTa_OnUnitBuy(long double resources, int32_t (*method)(long double));
int8_t eeTa_PlayerCount();
int8_t *eeTa_PlayerIDs(); // 30 elements max
int8_t eeTa_PlayerIndex(); // needs to be fixed.
void eeTa_FilePrintf(const char *format, ...);

uint8_t eeTa_ShouldOnInitExecute();

void eeTa_Map_PrintBitMap();