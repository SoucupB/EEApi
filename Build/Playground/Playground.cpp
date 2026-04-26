#include <math.h>
#include "PlayerState.h"
#include "MethodsDefinitions.h"
#include "Unit.h"
#include "LibManager.h"
#include "EETypes.h"
#include <map>
#include <algorithm>

void test_PrintUnits();
void test_ConvertEnemy();
void printPositions();
void testCastSomeSpell(Unit unit, Point pos);
void randomChecker();
void replaceCoc(Unit unit, Point pos);
Unit getProphet();
Unit getPriest();
void printAllUnitTypes();
void queueCommand(PVOID unit, Point target, Ability ability);
__declspec(dllexport) void castEarthquake();
__declspec(dllexport) void castMalaria();
__declspec(dllexport) void castHurricane();

PVOID createCalamityStruct(Point pos, Ability ability);

// I think I find the water tile.
void pulas() {
  Unit currentPriest = getPriest();
  if(!eeTa_Unit_Reference(currentPriest)) {
    return ;
  }
  eeTa_FilePrintf("Is water for %p tile %d\n", currentPriest, eeTa_Tile_IsWater(eeTa_Unit_TilePosition(currentPriest)));
}

void onLosingHealth(Unit unit) {
  eeTa_FilePrintf("Unit %p taking damage\n", eeTa_Unit_Reference(unit));
}

__declspec(dllexport) void printAllTiles() {
  Unit currentPriest = getPriest();
  if(!eeTa_Unit_Reference(currentPriest)) {
    return ;
  }
  eeTa_FilePrintf("Current plane ID %d\n", unit_GetPlaneID(currentPriest));
  // pulas();
  // eeTa_Map_PrintBitMap();
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
    // randomChecker();
    castEarthquake();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('T') & 0x8000) {
    // printAllTiles();
    // printAllUnitTypes();
    // castMalaria();
    castHurricane();
    Beep (300, 250);
  }
}

void test_PrintUnits() {
  vector<Unit> units = unit_GetUnits(eeTa_AllPlayers());
  if(units.size()) {
    for(int32_t i = 0; i < units.size(); i++) {
      Point currentPoint = eeTa_CurrentPosition(units[i]);
      eeTa_FilePrintf("Unit pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p\n", units[i]._payload, 
                      eeTa_UnitType(units[i]), eeTa_Player(units[i]), currentPoint.x, currentPoint.y, eeTypes_UnitClass(eeTa_EETypes_UnitType(units[i])));
    }
  }
  vector<Unit> buildings = unit_GetBuildings(eeTa_AllPlayers());
  if(buildings.size()) {
    for(int32_t i = 0; i < buildings.size(); i++) {
      Point currentPoint = eeTa_CurrentPosition(buildings[i]);
      eeTa_FilePrintf("Building pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p\n", 
                      buildings[i]._payload, eeTa_UnitType(buildings[i]), eeTa_Player(buildings[i]), currentPoint.x, currentPoint.y, eeTypes_UnitClass(eeTa_EETypes_UnitType(buildings[i])));
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

uint8_t navalAttackFilter(Unit unit) {
  UnitType def = eeTa_EETypes_UnitType(unit);
  return eeTypes_IsWaterUnit(def);
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

__declspec(dllexport) void randomChecker() {
  Unit priest = getProphet();
  if(!priest._payload) {
    return ;
  }
  Unit building = getEnemyBuilding();
  if(!building._payload) {
    return ;
  }
  eeTa_Unit_CastPoint(priest, eeTa_CurrentPosition(building), PROPHET_EARTHQUAKE);
  eeTa_FilePrintf("Moved\n");
}

PVOID createCalamityStruct(Point pos, Ability ability) {
  PVOID calamityBuffer = help_New(0x24);
  int32_t xPos = (int32_t)pos.x;
  int32_t yPos = (int32_t)pos.y;

  builder_FillValue(calamityBuffer, 0x0, (size_t)lib_BaseAddress() + 0x438B98);
  builder_FillValue(calamityBuffer, 0x4, 2);
  builder_FillValue(calamityBuffer, 0x8, 0xFEFF75);
  builder_FillValue(calamityBuffer, 0xC, 2);
  builder_FillValue(calamityBuffer, 0x10, 0x1000004);
  builder_FillValue(calamityBuffer, 0x14, 0xFFFFFFFF);
  builder_FillValue(calamityBuffer, 0x18, xPos);
  builder_FillValue(calamityBuffer, 0x1C, yPos);
  builder_FillValue(calamityBuffer, 0x20, ability);

  return calamityBuffer;
}

void fillCalamityStruct(PVOID unit, PVOID calamityStruct, PVOID originalPointer) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x2209C9);
  void __thiscall (*method)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID) = (void __thiscall (*)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(originalPointer, 
         unit, 
         0x0, 
         (PVOID)*(size_t *)((size_t)calamityStruct + 0x18), 
         (PVOID)*(size_t *)((size_t)calamityStruct + 0x1C),
         (PVOID)*(size_t *)((size_t)calamityStruct + 0x20), 
         (PVOID)0x1);
}

void addCommandToUnit(PVOID unit, PVOID calamityStruct) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FE863);
  void __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (void __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(unit, calamityStruct, NULL, NULL);
}

void unknownMethod4BC7AF(PVOID unit) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FDFA5);
  void __thiscall (*method)(PVOID, PVOID) = (void __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(unit, 
         (PVOID)0x1F40);
}

void queueCommand(PVOID unit, Point target, Ability ability) {
  PVOID pntTarget = help_New(0x34);
  PVOID calamityStruct = createCalamityStruct(target, ability);
  fillCalamityStruct(unit, calamityStruct, pntTarget);
  addCommandToUnit(unit, pntTarget);
  unknownMethod4BC7AF(unit); // This is the method which makes the unit move.
}

// This method works YEEY, but it might contain memory leaks or access violations.
__declspec(dllexport) void castEarthquake() {
  Unit currentProphet = getProphet();
  if(!eeTa_Unit_Reference(currentProphet)) {
    return ;
  }
  Unit currentBuilding = getEnemyBuilding();
  if(!eeTa_Unit_Reference(currentBuilding)) {
    return ;
  }
  // queueCommand(eeTa_Unit_Reference(currentProphet), eeTa_CurrentPosition(currentBuilding), PROPHET_EARTHQUAKE);
  unit_CastAbility(currentProphet, eeTa_CurrentPosition(currentBuilding), PROPHET_EARTHQUAKE);
  eeTa_FilePrintf("Some ability\n");
}

__declspec(dllexport) void castMalaria() {
  Unit currentProphet = getProphet();
  if(!eeTa_Unit_Reference(currentProphet)) {
    return ;
  }
  Unit currentenemy = getEnemy();
  if(!eeTa_Unit_Reference(currentenemy)) {
    return ;
  }
  // queueCommand(eeTa_Unit_Reference(currentProphet), eeTa_CurrentPosition(currentenemy), PROPHET_MALARIA);
  unit_CastAbility(currentProphet, eeTa_CurrentPosition(currentenemy), PROPHET_MALARIA);
  eeTa_FilePrintf("Some ability\n");
}

__declspec(dllexport) void castHurricane() {
  Unit currentProphet = getProphet();
  if(!eeTa_Unit_Reference(currentProphet)) {
    return ;
  }
  // Unit currentenemy = getEnemyShip();
  Unit currentenemy = getEnemy();
  if(!eeTa_Unit_Reference(currentenemy)) {
    return ;
  }
  unit_CastAbility(currentProphet, eeTa_CurrentPosition(currentenemy), PROPHET_TORNADO);
  eeTa_FilePrintf("Some ability\n");
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