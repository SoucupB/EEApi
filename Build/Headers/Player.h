#pragma once
#include <windows.h>
#include <stdint.h>
#include "Unit.h"

uint8_t ply_GetPlayerIndex(Unit unit);
Player ply_GetPlayer(Unit unit);
PVOID ply_Reference(Player self);
void ply_Inits(PVOID self);
Player ply_Null();
uint8_t ply_AreAllies(Player a, Player b);
void ply_Print();
Player ply_Self();
Player ply_Neutral();
int8_t ply_PlayerCount();