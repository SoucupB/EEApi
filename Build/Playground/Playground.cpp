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
void printAllUnitTypes();
Unit getBuilding();
Unit getCitizen();
void repairBuildings();
void queueCommand(PVOID unit, Point target, Ability ability);
void farmUnit();
Unit getFishboat();
Resource getFish();
void farmFish();

__declspec(dllexport) void castEarthquake();
__declspec(dllexport) void castMalaria();
__declspec(dllexport) void castHurricane();
__declspec(dllexport) void printAllUnitTypes();

PVOID createCalamityStruct(Point pos, Ability ability);

// I think I find the water tile.
void onLosingHealth(Unit unit) {
  eeTa_FilePrintf("Unit %p taking damage\n", eeTa_Unit_Reference(unit));
}

void printResources() {
  vector<Resource> resources = res_All();
  eeTa_FilePrintf("Total fixed resources is %d\n", resources.size());
  for(size_t i = 0; i < resources.size(); i++) {
    eeTa_FilePrintf("Resource pointer is %p with name %s and type %p\n", res_Reference(resources[i]), res_Name(resources[i]), res_Type(resources[i]));
  }
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
    Beep (300, 250);
  }
  if(GetAsyncKeyState('T') & 0x8000) {
    printResources();
    Beep (300, 250);
  }
}

void bt_OnInit() {
  eeTa_FilePrintf("Changing at %p\n", (size_t)GetModuleHandleA("EE-AOC.exe") + (size_t)0xBB9D8);
}

Point airNextPosition(Unit unit) {
  Point currentPosition = unit_Point_Position(unit);
  Point copyPosition = currentPosition;
  currentPosition.x = copyPosition.x + sinf(rand()) * 20.0f;
  currentPosition.y = copyPosition.y + sinf(rand()) * 20.0f;
  int32_t index = 5;
  while(index && map_Tile_GetPlaneID(geom_Tile_FromPoint(currentPosition)) == INVALID_TILE_ID) {
    currentPosition.x = copyPosition.x + sinf(rand()) * 20.0f;
    currentPosition.y = copyPosition.y + sinf(rand()) * 20.0f;
    index--;
  }
  if(!index) {
    return geom_Point_Invalid();
  }
  return currentPosition;
}

Point getNextPosition(Unit unit) {
  if(eeTypes_IsAirUnit(unit_Type(unit))) {
    return airNextPosition(unit);
  }
  Point currentPosition = unit_Point_Position(unit);
  Point copyPosition = currentPosition;
  currentPosition.x += sinf(rand()) * 20.0f;
  currentPosition.y += sinf(rand()) * 20.0f;
  int32_t index = 5;
  while(index && map_Tile_GetPlaneID(geom_Tile_FromPoint(copyPosition)) != map_Tile_GetPlaneID(geom_Tile_FromPoint(currentPosition))) {
    currentPosition.x = copyPosition.x + sinf(rand()) * 20.0f;
    currentPosition.y = copyPosition.y + sinf(rand()) * 20.0f;
    index--;
  }
  if(!index) {
    return geom_Point_Invalid();
  }
  return currentPosition;
}

void moveUnit(Unit unit) {
  if(!unit_IsIdle(unit)) {
    return ;
  }
  Point nextPosition = getNextPosition(unit);
  if(geom_Point_IsInvalid(nextPosition)) {
    return ;
  }
  unit_Action(unit, nextPosition, UNIT_ATTACK);
}

Unit getIdleCitizen(vector<Unit> &units) {
  for(size_t i = 0; i < units.size(); i++) {
    if(unit_IsIdle(units[i]) && eeTypes_IsCitizen(unit_Type(units[i]))) {
      return units[i];
    }
  }
  return eeTa_Unit_Null();
}

void repairBuildings(vector<Unit> &units) {
  Unit currentCitizen = getIdleCitizen(units);
  if(!unit_Reference(currentCitizen)) {
    return ;
  }
  vector<Unit> buildings = unit_GetBuildings(eeTa_AllPlayers());
  for(size_t i = 0; i < buildings.size(); i++) {
    if(unit_CurrentHp(buildings[i]) < unit_TotalHP(buildings[i])) {
      eeTa_FilePrintf("Started repairing!\n");
      unit_Repair(currentCitizen, buildings[i]);
      break;
    }
  }
}

void bt_MoveUnitsRandomly() {
  vector<Unit> units = unit_GetUnits(eeTa_AllPlayers());
  repairBuildings(units);
  for(size_t i = 0; i < units.size(); i++) {
    moveUnit(units[i]);
  }
}

void bt_OnFrame() {
  execDataPengus();
  bt_MoveUnitsRandomly();
  // pls_OnInit((PVOID)onLosingHealth);
}

void bt_OnUnitDestroy(Unit unit) {

}