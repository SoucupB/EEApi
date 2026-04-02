#include <math.h>
#include "AttackDecision.h"
#include "PlayerState.h"
#include "ResourceManager.h"
#include "MethodsDefinitions.h"

void test_PrintUnits();
void test_ConvertEnemy();

void execDataPengus() {
  if(GetAsyncKeyState('J') & 0x8000) {
    test_PrintUnits();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('P') & 0x8000) {
    test_ConvertEnemy();
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

Unit getEnemy() {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_Player(units[i]) != eeTa_SelfPlayer()) {
      return units[i];
    }
  }
  return eeTa_EmptyUnit();
}

void test_ConvertEnemy() {
  Unit priest = getPriest();
  Unit enemy = getEnemy();
  eeTa_FilePrintf("Unit pointer: %p, is converting %p\n", eeTa_Reference(priest), eeTa_Reference(enemy));
  eeTa_MoveTo(priest, enemy);
}

PVOID __fastcall moveTest(PVOID movingStructure) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  builder_PrintMemoryLayout(movingStructure, 0xB8);
  PVOID __thiscall (*method)(PVOID) = (PVOID __thiscall (*)(PVOID)) ((uint8_t *)hModule + 0x1EDCC0);
  return method(movingStructure);
}

void bt_OnInit() {
  eeTa_FilePrintf("Changing at %p\n", (size_t)GetModuleHandleA("EE-AOC.exe") + (size_t)0xBB9D8);
  builder_Definition((PVOID)0xBB9DB, (PVOID)moveTest);
}

void bt_OnFrame() {
  execDataPengus();
}

void bt_OnUnitDestroy(Unit unit) {

}