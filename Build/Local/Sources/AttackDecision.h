#pragma once

#include <Windows.h>
#include <stdint.h>
#include <vector>
#include "EETwa.h"

using namespace std;

void att_AttackTransportWithNavals(vector<Unit> &units);
void att_PatrolRandomPositions(vector<Unit> &selfUnits);
void att_AddDamagedUnits(Unit unit);
uint8_t att_IsUnitCarrier(Unit unit);
void att_ConvertIfNecessary(vector<Unit> &units);