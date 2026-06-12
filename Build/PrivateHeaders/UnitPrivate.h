#pragma once
#include "Unit.h"

uint8_t unit_IsComplexUnit(Unit unit);
uint8_t unit_Exists(Unit unit);
Unit unit_FromPayload(PVOID unit);
PVOID unit_GetTechNode(Unit unit);