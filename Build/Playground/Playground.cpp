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
#include "Ability.h"
#include "PlayerPrivate.h"
#include "EETypesStructPrivate.h"
#include "Action.h"


void act_Print(Unit unit);
void buildDock();
void attackTarget();
// I think I find the water tile.
void onLosingHealth(Unit unit) {
  eeTa_FilePrintf("Unit %p taking damage\n", unit_Reference(unit));
}

unordered_map<PVOID, Action> actionUnits;

Unit getCitizen() {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTypes_IsCitizen(unit_Type(units[i]))) {
      return units[i];
    }
  }
  return unit_Null();
}

Unit getAttackingUnit() {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(!eeTypes_IsCitizen(unit_Type(units[i])) && !eeTypes_IsWaterUnit(unit_Type(units[i]))) {
      return units[i];
    }
  }
  return unit_Null();
}

void printBuildingsOffset() {
  vector<Unit> buildings = unit_Player_GetBuildings(ply_Null());
  for(int32_t i = 0; i < buildings.size(); i++) {
    UnitType type = unit_Type(buildings[i]);
    eeTa_FilePrintf("File type %p for type %p and template %p with size %d\n", eeTypes_UnitTypeIndex(type), type, eeTypes_GetTemplate(type), eeTypes_BuildingSize(type));
  }
}

void buildStuff() {
  Unit citizen = getCitizen();
  if(!unit_Reference(citizen)) {
    return ;
  }
  unit_Building_Build(citizen, unit_Tile_Position(citizen), B_BARRACKS);
}

void printCanBuild(UnitType type) {
  Unit citizen = getCitizen();
  if(!unit_Reference(citizen)) {
    return ;
  }
  vector<Unit> units = unit_Player_GetUnits(ply_Null());
  for(int32_t i = 0; i < units.size(); i++) {
    TilePoint tile = unit_Tile_Position(units[i]);
    uint8_t canBuild = unit_Building_CanBuildAt(citizen, type, tile);
    Point currentPoint = unit_Point_Position(units[i]);
    eeTa_FilePrintf("Citizen %p can build %p at (%d %d)? %d\n", unit_Reference(citizen), type, tile.x, tile.y, canBuild);
  }
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
  // printBuildingsOffset();
  printCanBuild(B_DOCK);
  eeTa_FilePrintf("Is space %p\n", map_IsSpaceMap());
}

void printTileMethod() {
  vector<Unit> units = unit_Player_GetUnits(ply_Null());
  for(int32_t i = 0; i < units.size(); i++) {
    eeTa_FilePrintf("Is tile Z is %f\n", map_Tile_GetZ(unit_Tile_Position(units[i])));
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
    // printTileMethod();
    attackTarget();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('T') & 0x8000) {
    buildDock();
    Beep (300, 250);
  }
}

void bt_OnInit() {
  // printSpells();
}

Point getNextPosition(Unit unit) {
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

uint8_t searchAndBuild(Unit citizen, UnitType buildingType) {
  if(!unit_Reference(citizen) || !unit_IsIdle(citizen)) {
    return 0;
  }
  TilePoint pnt = unit_Building_FindBuildablePosition(citizen, buildingType, unit_Tile_Position(citizen));
  if(geom_Tile_IsInvalid(pnt)) {
    return 0;
  }
  unit_Building_Build(citizen, pnt, buildingType);
  return 1;
}

void citizenOperate() {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  int32_t index = 2;
  for(size_t i = 0; i < units.size(); i++) {
    if(index && eeTypes_IsCitizen(unit_Type(units[i])) && searchAndBuild(units[i], B_BARRACKS)) {
      index--;
    }
  }
}

uint32_t tileHash(TilePoint tile) {
  return tile.x * 300 + tile.y;
}

TilePoint findDockPosition(Unit citizen, TilePoint tile) {
  int32_t xPos[] = {1, 0, -1, 0};
  int32_t yPos[] = {0, -1, 0, 1};
  int32_t index = 255, head = 0;
  vector<TilePoint> vct;
  map<uint32_t, uint8_t> valid;
  vct.push_back(tile);
  valid[tileHash(tile)] = 1;
  while(index-- && head < vct.size()) {
    TilePoint currentTile = vct[head];
    if(unit_Building_CanBuildAt(citizen, B_DOCK, currentTile)) {
      return currentTile;
    }
    for(size_t i = 0; i < sizeof(xPos) / sizeof(int32_t); i++) {
      TilePoint nextPoint = (TilePoint) {
        .x = xPos[i] + currentTile.x,
        .y = yPos[i] + currentTile.y,
      };
      if(valid.find(tileHash(nextPoint)) != valid.end()) {
        continue;
      }
      vct.push_back(nextPoint);
      valid[tileHash(nextPoint)] = 1;
    }
    head++;
  }
  return geom_Tile_Invalid();
}

Unit findWaterUnit() {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTypes_IsWaterUnit(unit_Type(units[i]))) {
      return units[i];
    }
  }
  return unit_Null();
}

void buildDock() {
  Unit unit = findWaterUnit();
  if(!unit_Reference(unit)) {
    return ;
  }
  Unit citizen = getCitizen();
  if(!unit_Reference(citizen)) {
    return ;
  }
  TilePoint currentTile = unit_Building_FindBuildablePosition(citizen, B_DOCK, unit_Tile_Position(unit));
  if(geom_Tile_IsInvalid(currentTile)) {
    return ;
  }
  unit_Building_Build(citizen, currentTile, B_DOCK);
  eeTa_FilePrintf("Some crc (%d %d)\n", currentTile.x, currentTile.y);
}

void bt_OnUnitCreate(Unit unit) {
  eeTa_FilePrintf("Created unit %p and type %p\n", unit_Reference(unit), unit_Type(unit));
}

Unit getEnemyBuilding() {
  vector<Unit> buildings = unit_Player_GetBuildings(ply_Null());
  for(size_t i = 0; i < buildings.size(); i++) {
    if(eeTa_Player(buildings[i]) != eeTa_SelfPlayer() && unit_Type(buildings[i]) != B_TEMPLE) {
      return buildings[i];
    }
  }
  return unit_Null();
}

void attackTarget() {
  Unit attacker = getAttackingUnit();
  if(!unit_Reference(attacker)) {
    return ;
  }
  Unit enemyBuildings = getEnemyBuilding();
  if(!unit_Reference(enemyBuildings)) {
    return ;
  }
  unit_AttackTarget(attacker, enemyBuildings);
}

void bt_PrintActions() {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  for(size_t i = 0, c = units.size(); i < c; i++) {
    PVOID unitRef = unit_Reference(units[i]);
    Action lastAction = actionUnits[unitRef];
    Action currentAction = act_Get(units[i]);
    if(lastAction.type != currentAction.type) {
      act_Print(units[i]);
      actionUnits[unitRef] = currentAction;
    }
  }
}

void bt_OnFrame() {
  execDataPengus();
  bt_PrintActions();
  // citizenOperate();
}

void bt_OnUnitDestroy(Unit unit) {

}