#pragma once
#include <windows.h>
#include <stdint.h>
#include "Unit.h"

uint8_t ply_GetPlayerIndex(Unit unit);
Player ply_GetPlayer(Unit unit);
PVOID ply_Reference(Player self);