#pragma once
#include <vector>
#include <map>
#include <windows.h>
#include "EETypesStruct.h"

using namespace std;

/*
 * Checks if a unit type is from a specific class.
 */
uint8_t eeTypes_IsFromClass(UnitClassType unitClass, UnitType unitType);
/*
 * Checks if a unit type is a unit that can go through water (attacking/farming/transport boats)
 */
uint8_t eeTypes_IsWaterUnit(UnitType unitType);
/*
 * Checks if a unit type is a citizen (female/male)
 */
uint8_t eeTypes_IsCitizen(UnitType unitType);
/*
 * Checks if a unit type is a water transport boat (any epoch)
 */
uint8_t eeTypes_IsWaterTransport(UnitType unitType);
/*
 * Checks if a unit type is a transport, both water and flying unit.
 */
uint8_t eeTypes_IsTransport(UnitType unitType);
/*
 * Checks if a unit type is a ground unit but buildings are not included.
 */
uint8_t eeTypes_IsGroundUnit(UnitType unitType);
/*
 * Checks if a unit type is a fish boat
 */
uint8_t eeTypes_IsFishBoat(UnitType unitType);
/*
 * Checks if a unit type is a priest
 */
uint8_t eeTypes_IsPriest(UnitType type);
/*
 * Checks if a unit type is a plane, helicopter or any other type which flies.
 */
uint8_t eeTypes_IsAirUnit(UnitType unitType);
/*
 * Checks if a unit type is either a capitol or town center.
 */
uint8_t eeTypes_CanProduceWorkers(UnitType type);
PVOID eeTypes_GetTemplate(UnitType typeIndex);
char *eeTypes_Name(UnitType type);
vector<AbilityTypes> eeTypes_Abilities(UnitType type);
size_t eeTypes_BuildingSize(UnitType type);

UnitClassType eeTypes_UnitClass(UnitType unitType);
uint8_t eeTypes_IsBuilding(UnitType unitType);

NeutralClassType eeTypes_Neutral_Type(NeutralUnitType unitType);
uint8_t eeTypes_Neutral_IsResource(NeutralUnitType unitType);