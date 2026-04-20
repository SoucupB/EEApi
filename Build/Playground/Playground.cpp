#include <math.h>
#include "AttackDecision.h"
#include "PlayerState.h"
#include "ResourceManager.h"
#include "MethodsDefinitions.h"

void test_PrintUnits();
void test_ConvertEnemy();
void printPositions();
void testCastSomeSpell(Unit unit, Point pos);
void randomChecker();
void replaceCoc(Unit unit, Point pos);
Unit getProphet();
Unit getPriest();

// I think I find the water tile.
void pulas() {
  Unit currentPriest = getPriest();
  if(!eeTa_Unit_Reference(currentPriest)) {
    return ;
  }
  // TilePoint pnt = eeTa_Unit_TilePosition(currentPriest);
  // size_t isWaterTileMethod = (size_t)dll + 0x12681;
  // uint8_t __thiscall (*method)(PVOID, PVOID, PVOID) = (uint8_t __thiscall (*)(PVOID, PVOID, PVOID))isWaterTileMethod;
  // uint8_t resp = method((PVOID)((size_t)mapPointer + 0x1C), (PVOID)pnt.x, (PVOID)pnt.y);
  eeTa_FilePrintf("Is water for %p tile %d\n", currentPriest, eeTa_Tile_IsWater(eeTa_Unit_TilePosition(currentPriest)));
}

__declspec(dllexport) void printAllTiles() {
  // PVOID mapPointer = help_GetMapPointer();
  // size_t count = help_Map_TileCount(mapPointer);
  // PVOID tileRef = help_Map_TilePointer(mapPointer);
  // size_t c = 0;
  // for(size_t i = 0; i < count; i++) {
  //   for(size_t j = 0; j < count; j++) {
  //     size_t currentIndex = *(size_t *)((size_t)tileRef + (count * i + j) * 4);
  //     eeTa_FilePrintf("Tile index %d - %p\n", c++, currentIndex);
  //   }
  // }

  // PVOID tileRef = help_Map_TilePointer(mapPointer);
  // eeTa_FilePrintf("SSSSSS is %p\n", tileRef);
  eeTa_Map_PrintTiles();
  // pulas();
}

void execDataPengus() {
  if(GetAsyncKeyState('J') & 0x8000) {
    test_PrintUnits();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('P') & 0x8000) {
    test_ConvertEnemy();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('F') & 0x8000) {
    randomChecker();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('T') & 0x8000) {
    printAllTiles();
    Beep (300, 250);
  }
}

void test_PrintUnits() {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  if(units.size()) {
    for(int32_t i = 0; i < units.size(); i++) {
      Point currentPoint = eeTa_CurrentPosition(units[i]);
      eeTa_FilePrintf("Unit pointer: %p, unit type: %p unit team %d, position: (%f, %f)\n", units[i]._payload, eeTa_UnitType(units[i]), eeTa_Player(units[i]), currentPoint.x, currentPoint.y);
    }
  }
  vector<Unit> buildings = eeTa_Buildings(eeTa_AllPlayers());
  if(buildings.size()) {
    for(int32_t i = 0; i < buildings.size(); i++) {
      Point currentPoint = eeTa_CurrentPosition(buildings[i]);
      eeTa_FilePrintf("Building pointer: %p, unit type: %p unit team %d, position: (%f, %f)\n", buildings[i]._payload, eeTa_UnitType(buildings[i]), eeTa_Player(buildings[i]), currentPoint.x, currentPoint.y);
    }
  }
}

Unit getPriest() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_UnitType(units[i]) == UNIT_PREHISTORIC_PRIEST) {
      return units[i];
    }
  }
  return eeTa_Unit_Null();
}

Unit getProphet() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_UnitType(units[i]) == UNIT_PREHISTORIC_PROPHET) {
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
    if(eeTa_Player(buildings[i]) != eeTa_SelfPlayer()) {
      return buildings[i];
    }
  }
  return eeTa_Unit_Null();
}

void printPositions() {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  for(size_t i = 0; i < units.size(); i++) {
    Point pnt = eeTa_GetDestinationCommand(units[i]);
    if(pnt.x != -1) {
      eeTa_FilePrintf("Position for %p is X: %f, Y: %f\n", eeTa_Unit_Reference(units[i]), pnt.x, pnt.y);
    }
  }
}

void test_ConvertEnemy() {
  Unit priest = getPriest();
  Unit enemy = getEnemy();
  if(!eeTa_Unit_Reference(priest) || !eeTa_Unit_Reference(enemy)) {
    eeTa_FilePrintf("Not found\n");
    return ;
  }
  eeTa_Unit_Convert(priest, enemy);
  eeTa_FilePrintf("Rombububico\n");
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

void bt_OnInit() {
  eeTa_FilePrintf("Changing at %p\n", (size_t)GetModuleHandleA("EE-AOC.exe") + (size_t)0xBB9D8);
}

void bt_OnFrame() {
  execDataPengus();
}

void bt_OnUnitDestroy(Unit unit) {

}