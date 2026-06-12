#include <math.h>
#include "Unit.h"
#include "Player.h"
#include <vector>
#include "Action.h"
#include "Ability.h"
#include "Driver.h"

uint8_t isIdleCitizen(Unit unit) {
  const UnitType type = unit_Type(unit);
  return eeTypes_IsCitizen(type) && unit_IsIdle(unit) && ply_Reference(ply_GetPlayer(unit)) == ply_Reference(ply_Self());
}

void bt_OnGamePrepare() {
  
}

PVOID getTechNode(Unit unit) {
  TechTree tree = ply_TechTree(ply_GetPlayer(unit));
  PVOID cTechNode = driver_TechNode(tree, (AbilityTypes)unit_Type(unit));
  if(!cTechNode) {
    return 0;
  }
  return cTechNode;
}

void printPlayersResources() {
  vector<Player> plys = ply_All();
  ResourceIndex resources[] = {
    COST_FOOD,
    COST_WOOD,
    COST_ORE,
    COST_GOLD,
    COST_IRON
  };
  for(size_t i = 0; i < plys.size(); i++) {
    uint8_t plyIndex = ply_PlayerIndex(plys[i]);
    eeTa_FilePrintf("Player index is %d ", plyIndex);
    for(size_t j = 0; j < sizeof(resources) / sizeof(ResourceIndex); j++) {
      eeTa_FilePrintf("%d ", ply_GetResources(plys[i], resources[j]));
    }
    eeTa_FilePrintf("\n");
  }
}

void test_PrintUnitResCosts(Unit unit) {
  ResourceCost costs[6];
  uint8_t costsCount = 0;
  unit_Costs(unit, costs, &costsCount);
  eeTa_FilePrintf("Unit %p with type %p\n", unit_Reference(unit), unit_Type(unit));
  for(size_t i = 0; i < costsCount; i++) {
    eeTa_FilePrintf("Cost %d with value %d\n", costs[i].resIndex, costs[i].cost);
  }
  eeTa_FilePrintf("\n");
}

void test_PrintUnits() {
  vector<Unit> units = unit_Player_GetUnits(ply_Null());
  for(int32_t i = 0; i < units.size(); i++) {
    Point currentPoint = unit_Point_Position(units[i]);
    TilePoint tile = unit_Tile_Position(units[i]);
    eeTa_FilePrintf("Unit pointer: %p, unit type: %p unit team %d, tile ID: %d, position: (%f, %f) class %p with hp %d and range %f, current fuel %d, max fuel %d Civ Node %p\n", 
                    units[i]._payload, 
                    unit_Type(units[i]), eeTa_Player(units[i]), unit_GetPlaneID(units[i]), currentPoint.x, currentPoint.y, 
                    eeTypes_UnitClass(unit_Type(units[i])), unit_TotalHP(units[i]), unit_Range(units[i]), unit_GetCurrentFuel(units[i]), unit_GetMaxFuel(units[i]),
                    getTechNode(units[i]));
    eeTa_FilePrintf("Action is %d\n", act_Get(units[i]).type);
  }
  vector<Unit> buildings = unit_Player_GetBuildings(ply_Null());
  for(int32_t i = 0; i < buildings.size(); i++) {
    Point currentPoint = unit_Point_Position(buildings[i]);
    eeTa_FilePrintf("Building pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p with hp %d Civ Node %p\n", 
                    buildings[i]._payload, unit_Type(buildings[i]), 
                    eeTa_Player(buildings[i]), currentPoint.x, currentPoint.y, eeTypes_UnitClass(unit_Type(buildings[i])), unit_TotalHP(buildings[i]),
                    getTechNode(buildings[i]));
  }
  vector<Resource> resources = res_All();
  for(int32_t i = 0; i < resources.size(); i++) {
    eeTa_FilePrintf("Resource pointer: %p, unit type: %p pos (%f %f)\n", resources[i]._payload, res_Type(resources[i]), res_Point_Position(resources[i]).x, res_Point_Position(resources[i]).y);
  }
  printPlayersResources();
  for(int32_t i = 0; i < units.size(); i++) {
    test_PrintUnitResCosts(units[i]);
  }
  for(int32_t i = 0; i < buildings.size(); i++) {
    test_PrintUnitResCosts(buildings[i]);
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

uint8_t isSelfUnit(Unit unit) {
  return unit_GetPlayerIndex(unit) == eeTa_SelfPlayer();
}

uint8_t isSelfFlyingCombustionUnit(Unit unit) {
  const UnitType type = unit_Type(unit);
  return isSelfUnit(unit) && eeTypes_IsFromClass(CLASS_AIR_COMBUSTION_FLYEIR, type);
}

void replaceMoveCommandForFliers(PVOID _) {
  vector<Unit> airplanes = unit_Filter(isSelfFlyingCombustionUnit);
  for(size_t i = 0, c = airplanes.size(); i < c; i++) {
    Action currentAction = act_Get(airplanes[i]);
    if(currentAction.type == ACTION_MOVE) {
      unit_Action(airplanes[i], currentAction.targetPoint, UNIT_ATTACK);
    }
  }
}

void bt_InitAirplaneActions() {
  TimeAtom atom;
  atom.method = (PVOID)replaceMoveCommandForFliers;
  atom.arguments = NULL;
  atom.time = 3214;
  eeTa_AddFrameMethod(atom);
}

void bt_OnFrame() {
  execDataPengus();
}

void bt_OnInit() {
  eeTa_AddCheat(CHEAT_COST_REDUCTION);
  // bt_InitAirplaneActions();
}

void bt_OnUnitDestroy(Unit unit) {

}

void bt_OnUnitCreate(Unit unit) {
  
}