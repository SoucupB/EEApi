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
    // Unit priest = getProphet();
    // if(!priest._payload) {
    //   return ;
    // }
    // replaceCoc(priest, (Point) {
    //   .x = 50.0f,
    //   .y = 30.0f
    // });
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
  return eeTa_EmptyUnit();
}

Unit getProphet() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_UnitType(units[i]) == UNIT_PREHISTORIC_PROPHET) {
      return units[i];
    }
  }
  return eeTa_EmptyUnit();
}

Unit getEnemy() {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_Player(units[i]) != eeTa_SelfPlayer()) {
      return units[i];
    }
  }
  return eeTa_EmptyUnit();
}

void printPositions() {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  for(size_t i = 0; i < units.size(); i++) {
    Point pnt = eeTa_GetDestinationCommand(units[i]);
    if(pnt.x != -1) {
      eeTa_FilePrintf("Position for %p is X: %f, Y: %f\n", eeTa_Reference(units[i]), pnt.x, pnt.y);
    }
  }
}

void test_ConvertEnemy() {
  Unit priest = getPriest();
  Unit enemy = getEnemy();
  if(!eeTa_Reference(priest) || !eeTa_Reference(enemy)) {
    eeTa_FilePrintf("Not found\n");
    return ;
  }
  eeTa_ConvertUnit(priest, enemy);
  eeTa_FilePrintf("Rombububico\n");
}

PVOID __thiscall convergenceRandom(PVOID ecx) {
  PVOID __thiscall (*method)(PVOID) = (PVOID __thiscall (*)(PVOID))((size_t)0x4B8E86);
  eeTa_FilePrintf("PDSADSADSA %p\n", ecx);
  builder_PrintMemoryTree(ecx);
  PVOID szz = method(ecx);
  return szz;
}

void replaceRandomMethod() {
  // builder_Definition((PVOID)0x1537BA, (PVOID)convergenceRandom);
}

__declspec(dllexport) void randomChecker() {
  Unit priest = getProphet();
  if(!priest._payload) {
    return ;
  }
  testCastSomeSpell(priest, (Point) {
    .x = 50.0f,
    .y = 30.0f
  });
  eeTa_FilePrintf("Moved\n");
}

void replaceCoc(Unit unit, Point pos) {
  // PVOID view = (PVOID)((size_t)unit._payload + 0x1EC);
  // PVOID action = (PVOID)((size_t)unit._payload + 0x1F0);
  // if(*(size_t *)view) {
  //   help_Delete((PVOID)*(size_t *)view);
  // }
  // if(*(size_t *)action) {
  //   help_Delete((PVOID)*(size_t *)action);
  // }

  PVOID unitBuffer = help_New(0x34);
  int32_t valX = (int32_t)pos.x;
  int32_t valY = (int32_t)pos.y;
  builder_FillValue(unitBuffer, 0x0, 0x0084748C);
  builder_FillValue(unitBuffer, 0x4, 2817);
  builder_FillValue(unitBuffer, 0x8, 5000);
  builder_FillValue(unitBuffer, 0xc, 0x10235);
  builder_FillValue(unitBuffer, 0x10, 0x39ED);
  builder_FillValue(unitBuffer, 0x14, 4);
  builder_FillValue(unitBuffer, 0x18, 2);
  builder_FillValue(unitBuffer, 0x1C, 257);
  builder_FillValue(unitBuffer, 0x20, (size_t)unit._payload);
  builder_FillValue(unitBuffer, 0x24, 0);
  builder_FillValue(unitBuffer, 0x28, valX);
  builder_FillValue(unitBuffer, 0x2C, valY);
  builder_FillValue(unitBuffer, 0x30, 0x3FA);
  memcpy((PVOID)((size_t)unit._payload + 0x1EC), &unitBuffer, sizeof(size_t));
  memcpy((PVOID)((size_t)unit._payload + 0x1F0), &unitBuffer, sizeof(size_t));
}

void testCastSomeSpell(Unit unit, Point pos) {
  // help_Delete((PVOID)((size_t)unit._payload + 0x1EC));

  // This works for some reason ?!?!?.
  help_UnitMove(unit._payload, pos, UNIT_ATTACK);
  replaceCoc(unit, pos);
  // PVOID unitBuffer = help_New(0x34);
  // int32_t valX = (int32_t)pos.x;
  // int32_t valY = (int32_t)pos.y;
  // builder_FillValue(unitBuffer, 0x0, 0x0084748C);
  // builder_FillValue(unitBuffer, 0x4, 2817);
  // builder_FillValue(unitBuffer, 0x8, 5000);
  // builder_FillValue(unitBuffer, 0xc, 0x10235);
  // builder_FillValue(unitBuffer, 0x10, 0x39ED);
  // builder_FillValue(unitBuffer, 0x14, 4);
  // builder_FillValue(unitBuffer, 0x18, 2);
  // builder_FillValue(unitBuffer, 0x1C, 257);
  // builder_FillValue(unitBuffer, 0x20, (size_t)unit._payload);
  // builder_FillValue(unitBuffer, 0x24, 0);
  // builder_FillValue(unitBuffer, 0x28, valX);
  // builder_FillValue(unitBuffer, 0x2C, valY);
  // builder_FillValue(unitBuffer, 0x30, 0x3FA);

  // memcpy((PVOID)((size_t)unit._payload + 0x1EC), &unitBuffer, sizeof(size_t));
}

void bt_OnInit() {
  replaceRandomMethod();
  eeTa_FilePrintf("Changing at %p\n", (size_t)GetModuleHandleA("EE-AOC.exe") + (size_t)0xBB9D8);
}

void bt_OnFrame() {
  execDataPengus();
}

void bt_OnUnitDestroy(Unit unit) {

}