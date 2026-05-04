#include <math.h>
#include "PlayerState.h"
#include "MethodsDefinitions.h"
#include "Unit.h"
#include "LibManager.h"
#include "EETypes.h"
#include <map>
#include <algorithm>
#include "Resource.h"

void test_PrintUnits();
void test_ConvertEnemy();
void printPositions();
void testCastSomeSpell(Unit unit, Point pos);
void randomChecker();
void replaceCoc(Unit unit, Point pos);
Unit getProphet();
Unit getPriest();
void printAllUnitTypes();
Unit getBuilding();
Unit getCitizen();
void repairBuildings();
void queueCommand(PVOID unit, Point target, Ability ability);
void farmUnit();
Unit getFishboat();
Resource getFish();
void farmFish();
void moveUnit();

__declspec(dllexport) void castEarthquake();
__declspec(dllexport) void castMalaria();
__declspec(dllexport) void castHurricane();
__declspec(dllexport) void printAllUnitTypes();

PVOID createCalamityStruct(Point pos, Ability ability);

// I think I find the water tile.
void onLosingHealth(Unit unit) {
  eeTa_FilePrintf("Unit %p taking damage\n", eeTa_Unit_Reference(unit));
}

__declspec(dllexport) void printAllTiles() {
  Unit currentPriest = getPriest();
  if(!eeTa_Unit_Reference(currentPriest)) {
    return ;
  }
  eeTa_FilePrintf("Current plane ID %d\n", unit_GetPlaneID(currentPriest));
}

void printResources() {
  vector<Resource> resources = res_All();
  eeTa_FilePrintf("Total fixed resources is %d\n", resources.size());
  for(size_t i = 0; i < resources.size(); i++) {
    eeTa_FilePrintf("Resource pointer is %p with name %s and type %p\n", res_Reference(resources[i]), res_Name(resources[i]), res_Type(resources[i]));
  }
}

void execDataPengus() {
  if(GetAsyncKeyState('J') & 0x8000) {
    test_PrintUnits();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('P') & 0x8000) {
    // test_ConvertEnemy();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('F') & 0x8000) {
    // repairBuildings();
    // farmUnit();
    // farmFish();
    moveUnit();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('T') & 0x8000) {
    printResources();
    Beep (300, 250);
  }
}

void repairBuildings() {
  Unit building = getBuilding();
  if(!unit_Reference(building)) {
    return ;
  }
  Unit citizen = getCitizen();
  if(!unit_Reference(citizen)) {
    return ;
  } 
  unit_Repair(citizen, building);
}

void test_PrintUnits() {
  vector<Unit> units = unit_GetUnits(eeTa_AllPlayers());
  if(units.size()) {
    for(int32_t i = 0; i < units.size(); i++) {
      Point currentPoint = eeTa_CurrentPosition(units[i]);
      eeTa_FilePrintf("Unit pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p with hp %d\n", units[i]._payload, 
                      eeTa_UnitType(units[i]), eeTa_Player(units[i]), currentPoint.x, currentPoint.y, eeTypes_UnitClass(eeTa_EETypes_UnitType(units[i])), unit_TotalHP(units[i]));
    }
  }
  vector<Unit> buildings = unit_GetBuildings(eeTa_AllPlayers());
  if(buildings.size()) {
    for(int32_t i = 0; i < buildings.size(); i++) {
      Point currentPoint = eeTa_CurrentPosition(buildings[i]);
      eeTa_FilePrintf("Building pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p with hp %d\n", 
                      buildings[i]._payload, eeTa_UnitType(buildings[i]), 
                      eeTa_Player(buildings[i]), currentPoint.x, currentPoint.y, eeTypes_UnitClass(eeTa_EETypes_UnitType(buildings[i])), unit_TotalHP(buildings[i]));
    }
  }
}

Unit getPriest() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_UnitType(units[i]) == PRIEST) {
      return units[i];
    }
  }
  return eeTa_Unit_Null();
}

Unit getProphet() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_UnitType(units[i]) == PROPHET) {
      return units[i];
    }
  }
  return eeTa_Unit_Null();
}

Unit getCitizen() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTypes_IsCitizen(unit_Type(units[i]))) {
      return units[i];
    }
  }
  return eeTa_Unit_Null();
}

Unit getCavalry() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(unit_Type(units[i]) == CAV04_BRONZE_SPEAR_CAVALRY) {
      return units[i];
    }
  }
  return eeTa_Unit_Null();
}

Unit getFishboat() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTypes_IsFishBoat(unit_Type(units[i]))) {
      return units[i];
    }
  }
  return eeTa_Unit_Null();
}

Unit getEnemy() {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_Player(units[i]) != eeTa_SelfPlayer()) {
      return units[i];
    }
  }
  return eeTa_Unit_Null();
}

Unit getEnemyBuilding() {
  vector<Unit> buildings = eeTa_Buildings(eeTa_AllPlayers());
  for(size_t i = 0; i < buildings.size(); i++) {
    if(eeTa_Player(buildings[i]) != eeTa_SelfPlayer() && eeTa_UnitType(buildings[i]) != B_TEMPLE) {
      return buildings[i];
    }
  }
  return eeTa_Unit_Null();
}

Unit getBuilding() {
  vector<Unit> buildings = eeTa_Buildings(eeTa_AllPlayers());
  for(size_t i = 0; i < buildings.size(); i++) {
    if(eeTa_Player(buildings[i]) == eeTa_SelfPlayer()) {
      return buildings[i];
    }
  }
  return eeTa_Unit_Null();
}

Resource getFarmableRes() {
  vector<Resource> units = res_All();
  for(size_t i = 0; i < units.size(); i++) {
    if(res_Type(units[i]) != RES_FISH) {
      return units[i];
    }
  }
  return res_Null();
}

Resource getFish() {
  vector<Resource> units = res_All();
  for(size_t i = 0; i < units.size(); i++) {
    if(res_Type(units[i]) == RES_FISH) {
      return units[i];
    }
  }
  return res_Null();
}

uint8_t navalAttackFilter(Unit unit) {
  UnitType def = eeTa_EETypes_UnitType(unit);
  return eeTypes_IsWaterUnit(def);
}

// CAV04_BRONZE_SPEAR_CAVALRY
void farmUnit() {
  Unit citizen = getCitizen();
  if(!unit_Reference(citizen)) {
    return ;
  }
  Resource res = getFarmableRes();
  if(!res_Reference(res)) {
    return ;
  }
  eeTa_FilePrintf("Farm init %s!\n", res_Name(res));
  unit_Farm(citizen, res);
}

void moveUnit() {
  Unit citizen = getCavalry();
  if(!unit_Reference(citizen)) {
    return ;
  }
  unit_Action(citizen, (Point) {
    .x = 84.399f,
    .y = 47.001f
  }, UNIT_MOVE);
}

void farmFish() {
  Resource res = getFish();
  if(!res_Reference(res)) {
    return ;
  }
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTypes_IsFishBoat(unit_Type(units[i]))) {
      unit_Farm(units[i], res);
    }
  }
  eeTa_FilePrintf("Farm fish init %s!\n", res_Name(res));
}


Unit getEnemyShip() {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  vector<Unit> filteredUnits = eeTa_Filter(units, navalAttackFilter);
  for(size_t i = 0; i < filteredUnits.size(); i++) {
    if(eeTa_Player(filteredUnits[i]) != eeTa_SelfPlayer()) {
      return filteredUnits[i];
    }
  }
  return eeTa_Unit_Null();
}

void bt_OnInit() {
  eeTa_FilePrintf("Changing at %p\n", (size_t)GetModuleHandleA("EE-AOC.exe") + (size_t)0xBB9D8);
}

void bt_OnFrame() {
  execDataPengus();
  // pls_OnInit((PVOID)onLosingHealth);
}

void bt_OnUnitDestroy(Unit unit) {

}