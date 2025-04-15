#pragma once

#include <Windows.h>
#include <stdint.h>
#include <vector>

using namespace std;

void att_AttackTransportWithNavals(vector<PVOID> &units);
void att_AttackEnemiesWithPlanes(vector<PVOID> &units);
void att_PatrolRandomPositions(vector<PVOID> &selfUnits);
void att_AddDamagedUnits(PVOID unit);
uint8_t att_IsUnitCarrier(PVOID unit);