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
    Unit priest = getPriest();
    if(!priest._payload) {
      return ;
    }
    help_MoveSecondMethod(priest._payload, (Point) {
      .x = 50.0f,
      .y = 30.0f
    });
    Beep (300, 250);
  }
}

void test_PrintUnits() {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  if(units.size()) {
    for(int32_t i = 0; i < units.size(); i++) {
      eeTa_FilePrintf("Unit pointer: %p, unit type: %p unit team %d\n", units[i]._payload, eeTa_UnitType(units[i]), eeTa_Player(units[i]));
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
  vector<PVOID> buildings = eeTa_Buildings(eeTa_AllPlayers());
  for(size_t i = 0; i < buildings.size(); i++) {
    if(eeTa_Player((Unit) {
      ._payload = buildings[i]
    }) != eeTa_SelfPlayer()) {
      return (Unit) {
        ._payload = buildings[i]
      };
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