#pragma once
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "Geometry.h"
#include "TimerHelper.h"
#include "EETypes.h"

int64_t eeTa_CurrentFrame();
int8_t eeTa_Player(Unit unit);
void eeTa_Printf(const char *format, ...);
int8_t eeTa_SelfPlayer();
int8_t eeTa_AllPlayers();
Point eeTa_GetDestinationCommand(Unit unit);
void eeTa_SetCvCAggression(uint8_t botIndex, float aggression);
void eeTa_AddFrameMethod(TimeAtom atom);
uint8_t eeTa_NeutralPlayer();
PVOID eeTa_GetPlayer();
int8_t eeTa_PlayerCount();
void eeTa_FilePrintf(const char *format, ...);