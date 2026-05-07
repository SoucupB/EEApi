#pragma once
#include "EETwa.h"
#include <stdint.h>
#include "Resource.h"

typedef struct Unit_t Unit;

int8_t unit_IsDead(Unit unit);
vector<Unit> unit_GetBuildings(int8_t player);
vector<Unit> unit_GetUnits(int8_t player);
int32_t unit_CurrentlyBuilding(Unit building);
int8_t unit_IsBuilding(Unit unit);
TilePoint unit_Tile_Position(Unit unit);
uint16_t unit_GetPlaneID(Unit unit);
void unit_CastAbility(Unit unit, Point target, Ability ability);
UnitType unit_Type(Unit unit);
int32_t unit_CurrentHp(Unit unit);
void unit_Repair(Unit unit, Unit target);
int32_t unit_TotalHP(Unit unit);
PVOID unit_Reference(Unit unit);
uint8_t unit_GetPlayerIndex(Unit unit);
void unit_Farm(Unit unit, Resource resource);
int8_t unit_IsIdle(Unit unit);
void unit_Action(Unit unit, Point point, UnitAction action);
Point unit_Point_Position(Unit unit);
void unit_Convert(Unit src, Unit dst);
int8_t unit_IsBuildingComplete(Unit unit);
int8_t unit_Building_IsIdle(Unit building);
Unit unit_Null();
uint8_t unit_CurrentEnergy(Unit unit);
float unit_Range(Unit unit);
float unit_Distance(Unit first, Unit dst);
vector<Unit> unit_IdleBuildings(int8_t player);
vector<Unit> unit_Filter(uint8_t (*method)(Unit));
uint8_t unit_CanCast(Unit unit, Ability ability);
uint8_t unit_IsTransport(Unit unit);
void unit_Load(Unit transport, vector<Unit> &units);