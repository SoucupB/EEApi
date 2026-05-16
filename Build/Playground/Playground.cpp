#include <math.h>
#include "PlayerState.h"
#include "MethodsDefinitions.h"
#include "Unit.h"
#include "LibManager.h"
#include "EETypes.h"
#include <map>
#include <algorithm>
#include "Resource.h"
#include "Player.h"

void test_PrintUnits();
void printSpells();

// I think I find the water tile.
void onLosingHealth(Unit unit) {
  eeTa_FilePrintf("Unit %p taking damage\n", unit_Reference(unit));
}

void test_PrintUnits() {
  vector<Unit> units = unit_Player_GetUnits(ply_Null());
  if(units.size()) {
    for(int32_t i = 0; i < units.size(); i++) {
      Point currentPoint = unit_Point_Position(units[i]);
      eeTa_FilePrintf("Unit pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p with hp %d and range %f\n", units[i]._payload, 
                      unit_Type(units[i]), eeTa_Player(units[i]), currentPoint.x, currentPoint.y, 
                      eeTypes_UnitClass(unit_Type(units[i])), unit_TotalHP(units[i]), unit_Range(units[i]));
    }
  }
  vector<Unit> buildings = unit_Player_GetBuildings(ply_Null());
  if(buildings.size()) {
    for(int32_t i = 0; i < buildings.size(); i++) {
      Point currentPoint = unit_Point_Position(buildings[i]);
      eeTa_FilePrintf("Building pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p with hp %d\n", 
                      buildings[i]._payload, unit_Type(buildings[i]), 
                      eeTa_Player(buildings[i]), currentPoint.x, currentPoint.y, eeTypes_UnitClass(unit_Type(buildings[i])), unit_TotalHP(buildings[i]));
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
    ply_Print();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('F') & 0x8000) {
    // repairBuildings();
    // farmUnit();
    // farmFish();
    // castEarthquake();
    // convertUnit();
    // loadTransport();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('T') & 0x8000) {
    // printTransport();
    printSpells();
    Beep (300, 250);
  }
}

void printSpells() {
  PEETypes types = game_GetEETypes();
  map<UnitType, vector<Ability> > *abilityPointers = types->abilityPointers;
  for(auto &it : *abilityPointers) {
    if(!it.second.size() || eeTypes_IsBuilding(it.first)) {
      continue;
    }
    eeTa_FilePrintf("Unit %p with %s ", it.first, eeTypes_Name(it.first));
    for(size_t i = 0; i < it.second.size(); i++) {
      eeTa_FilePrintf("%p ", it.second[i]);
    }
    eeTa_FilePrintf("\n");
  }
}

void bt_OnInit() {
  printSpells();
  eeTa_FilePrintf("Spookly\n");
}

void bt_OnFrame() {
  execDataPengus();
}

void bt_OnUnitDestroy(Unit unit) {

}