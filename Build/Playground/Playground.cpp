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
void convertUnit();
void addProphetSpells();
void printTransport();
void loadTransport();
void unloadTransport();
void buildSomeShit();

__declspec(dllexport) void castEarthquake();
__declspec(dllexport) void castMalaria();
__declspec(dllexport) void castHurricane();
__declspec(dllexport) void printAllUnitTypes();

PVOID createCalamityStruct(Point pos, Ability ability);

// I think I find the water tile.
void onLosingHealth(Unit unit) {
  eeTa_FilePrintf("Unit %p taking damage\n", unit_Reference(unit));
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
      Point currentPoint = unit_Point_Position(units[i]);
      eeTa_FilePrintf("Unit pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p with hp %d and range %f\n", units[i]._payload, 
                      unit_Type(units[i]), eeTa_Player(units[i]), currentPoint.x, currentPoint.y, 
                      eeTypes_UnitClass(unit_Type(units[i])), unit_TotalHP(units[i]), unit_Range(units[i]));
    }
  }
  vector<Unit> buildings = unit_GetBuildings(eeTa_AllPlayers());
  if(buildings.size()) {
    for(int32_t i = 0; i < buildings.size(); i++) {
      Point currentPoint = unit_Point_Position(buildings[i]);
      eeTa_FilePrintf("Building pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p with hp %d\n", 
                      buildings[i]._payload, unit_Type(buildings[i]), 
                      eeTa_Player(buildings[i]), currentPoint.x, currentPoint.y, eeTypes_UnitClass(unit_Type(buildings[i])), unit_TotalHP(buildings[i]));
    }
  }
}

Unit getBuilding(Unit prophet) {
  vector<Unit> units = unit_GetBuildings(eeTa_AllPlayers());
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(eeTa_Player(units[i]) != eeTa_SelfPlayer()) {
      eeTa_FilePrintf("Some Dist %f %f\n", unit_Distance(prophet, units[i]), unit_Range(prophet));
    }
    if(eeTa_Player(units[i]) != eeTa_SelfPlayer() && unit_Distance(prophet, units[i]) <= unit_Range(prophet)) {
      return units[i];
    }
  }
  return unit_Null();
}

void castEarthQuake(Unit prophet) {
  Unit building = getBuilding(prophet);
  eeTa_FilePrintf("Building found at %p\n", unit_Reference(building));
  if(!unit_Reference(building)) {
    return ;
  }
  unit_CastAbility(prophet, unit_Point_Position(building), PROPHET_EARTHQUAKE);
}

void prophetCommandSpells(PVOID _) {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(unit_Type(units[i]) == PROPHET) {
      castEarthQuake(units[i]);
    }
  }
}

void addProphetSpells() {
  TimeAtom atom;
  atom.method = (PVOID)prophetCommandSpells;
  atom.arguments = NULL;
  atom.time = 4144;
  eeTa_AddFrameMethod(atom);
}

Unit getProphet() {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(unit_Type(units[i]) == PROPHET) {
      return units[i];
    }
  }
  return unit_Null();
}

Unit getEnemyBuilding() {
  vector<Unit> buildings = unit_GetBuildings(eeTa_AllPlayers());
  for(size_t i = 0; i < buildings.size(); i++) {
    if(eeTa_Player(buildings[i]) != eeTa_SelfPlayer() && unit_Type(buildings[i]) != B_TEMPLE) {
      return buildings[i];
    }
  }
  return unit_Null();
}

Unit getPriest() {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(unit_Type(units[i]) == PRIEST) {
      return units[i];
    }
  }
  return unit_Null();
}

void castEarthquake() {
  Unit currentProphet = getProphet();
  if(!unit_Reference(currentProphet)) {
    return ;
  }
  Unit currentBuilding = getEnemyBuilding();
  if(!unit_Reference(currentBuilding)) {
    return ;
  }
  unit_CastAbility(currentProphet, unit_Point_Position(currentBuilding), PROPHET_MALARIA);
  eeTa_FilePrintf("Some ability is casted lolol\n");
}

Unit getEnemy() {
  vector<Unit> units = unit_GetUnits(eeTa_AllPlayers());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_Player(units[i]) != eeTa_SelfPlayer()) {
      return units[i];
    }
  }
  return unit_Null();
}

void convertUnit() {
  Unit currentPriest = getPriest();
  if(!unit_Reference(currentPriest)) {
    return ;
  }
  Unit currentUnit = getEnemy();
  if(!unit_Reference(currentUnit)) {
    return ;
  }
  // unit_CastAbility(currentProphet, unit_Point_Position(currentBuilding), PROPHET_MALARIA);
  unit_Convert(currentPriest, currentUnit);
  eeTa_FilePrintf("Some convertion is made\n");
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
    buildSomeShit();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('T') & 0x8000) {
    // printTransport();
    unloadTransport();
    Beep (300, 250);
  }
}

void bt_OnInit() {
  addProphetSpells();
  eeTa_FilePrintf("Added prophet spells\n");
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
  return unit_Null();
}

void repairBuildings(vector<Unit> &units) {
  Unit currentCitizen = getIdleCitizen(units);
  if(!unit_Reference(currentCitizen)) {
    return ;
  }
  vector<Unit> buildings = unit_GetBuildings(eeTa_SelfPlayer());
  for(size_t i = 0; i < buildings.size(); i++) {
    if(unit_CurrentHp(buildings[i]) < unit_TotalHP(buildings[i])) {
      unit_Repair(currentCitizen, buildings[i]);
      break;
    }
  }
}

void bt_MoveUnitsRandomly() {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  repairBuildings(units);
  for(size_t i = 0; i < units.size(); i++) {
    moveUnit(units[i]);
  }
}

uint8_t transportFilter(Unit unit) {
  return unit_GetPlayerIndex(unit) == eeTa_SelfPlayer() && unit_IsTransport(unit);
}

uint8_t nonTransportFilter(Unit unit) {
  return unit_GetPlayerIndex(unit) == eeTa_SelfPlayer() && !unit_IsTransport(unit);
}

void printTransport() {
  vector<Unit> transport = unit_Filter(transportFilter);
  for(size_t i = 0, c = transport.size(); i < c; i++) {
    eeTa_FilePrintf("Transport %p with population %d\n", unit_Reference(transport[i]), unit_Transport_Population(transport[i]));
    vector<Unit> unitsInside = unit_Transport_UnitsInside(transport[i]);
    for(size_t j = 0; j < unitsInside.size(); j++) {
      eeTa_FilePrintf(" Unit %p\n", unit_Reference(unitsInside[j]));
    }
  }
}

void unloadTransport() {
  vector<Unit> transport = unit_Filter(transportFilter);
  if(!transport.size()) {
    return ;
  }
  unit_Transport_Unload(transport[0], (TilePoint) {
    .x = 53,
    .y = 48
  });
}

void loadTransport() {
  vector<Unit> transport = unit_Filter(transportFilter);
  if(!transport.size()) {
    return ;
  }
  vector<Unit> units = unit_Filter(nonTransportFilter);
  if(!units.size()) {
    return ;
  }
  unit_Transport_Load(transport[0], units);
}

Unit getCitizen() {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTypes_IsCitizen(unit_Type(units[i]))) {
      return units[i];
    }
  }
  return unit_Null();
}

void buildSomeShit() {
  Unit citizen = getCitizen();
  if(!unit_Reference(citizen)) {
    return ;
  }
  unit_Building_Build(citizen, (TilePoint) {
    .x = 50,
    .y = 46
  }, B_BARRACKS);
  // PVOID building = helper_Building_Create(0x22,
  // (TilePoint) {
  //   .x = 50,
  //   .y = 46
  // });

  eeTa_FilePrintf("Building shit\n");
}

void bt_OnFrame() {
  execDataPengus();
  // bt_MoveUnitsRandomly();
}

void bt_OnUnitDestroy(Unit unit) {

}