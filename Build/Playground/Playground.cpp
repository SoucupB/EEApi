#include <math.h>
#include "Unit.h"
#include "Player.h"
#include <vector>
#include "Action.h"
#include "Ability.h"

uint8_t isIdleCitizen(Unit unit) {
  const UnitType type = unit_Type(unit);
  return eeTypes_IsCitizen(type) && unit_IsIdle(unit) && ply_Reference(ply_GetPlayer(unit)) == ply_Reference(ply_Self());
}

void test_PrintUnits() {
  vector<Unit> units = unit_Player_GetUnits(ply_Null());
  for(int32_t i = 0; i < units.size(); i++) {
    Point currentPoint = unit_Point_Position(units[i]);
    TilePoint tile = unit_Tile_Position(units[i]);
    eeTa_FilePrintf("Unit pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p with hp %d and range %f\n", units[i]._payload, 
                    unit_Type(units[i]), eeTa_Player(units[i]), currentPoint.x, currentPoint.y, 
                    eeTypes_UnitClass(unit_Type(units[i])), unit_TotalHP(units[i]), unit_Range(units[i]));
    eeTa_FilePrintf("Action is %d\n", act_Get(units[i]).type);
  }
  vector<Unit> buildings = unit_Player_GetBuildings(ply_Null());
  for(int32_t i = 0; i < buildings.size(); i++) {
    Point currentPoint = unit_Point_Position(buildings[i]);
    eeTa_FilePrintf("Building pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p with hp %d\n", 
                    buildings[i]._payload, unit_Type(buildings[i]), 
                    eeTa_Player(buildings[i]), currentPoint.x, currentPoint.y, eeTypes_UnitClass(unit_Type(buildings[i])), unit_TotalHP(buildings[i]));
  }
  vector<Resource> resources = res_All();
  for(int32_t i = 0; i < resources.size(); i++) {
    eeTa_FilePrintf("Resource pointer: %p, unit type: %p pos (%f %f)\n", resources[i]._payload, res_Type(resources[i]), res_Point_Position(resources[i]).x, res_Point_Position(resources[i]).y);
  }
}

void execDataPengus() {
  if(GetAsyncKeyState('J') & 0x8000) {
    test_PrintUnits();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('P') & 0x8000) {
    Beep (300, 250);
  }
  if(GetAsyncKeyState('F') & 0x8000) {
    Beep (300, 250);
  }
  if(GetAsyncKeyState('T') & 0x8000) {
    Beep (300, 250);
  }
}

void collectResourcesWithWorkers() {
  vector<Unit> citizens = unit_Filter(isIdleCitizen);
  vector<Resource> resources = res_All();
  for(size_t i = 0; i < citizens.size(); i++) {
    float maxDistance = 1000000.0f;
    Resource currentResource = res_Null();
    for(size_t j = 0; j < resources.size(); j++) {
      NeutralUnitType resType = res_Type(resources[j]);
      if(resType != RES_GOLD_MINE) {
        continue;
      }
      const float currentDistF = distanceEuclidf(unit_Point_Position(citizens[i]), res_Point_Position(resources[j]));
      if(maxDistance > currentDistF) {
        maxDistance = currentDistF;
        currentResource = resources[j];
      }
    }
    if(res_Reference(currentResource)) {
      unit_Farm(citizens[i], currentResource);
    }
  }
}

void bt_OnFrame() {
  execDataPengus();
  collectResourcesWithWorkers();
}

void bt_OnInit() {
}

void bt_OnUnitDestroy(Unit unit) {

}

void bt_OnUnitCreate(Unit unit) {
  
}