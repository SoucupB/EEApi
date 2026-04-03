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

PVOID __fastcall test_MoveToUnit(PVOID movingStructure) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  builder_PrintMemoryLayout(movingStructure, 0xB8);
  PVOID __fastcall (*method)(PVOID) = (PVOID __fastcall (*)(PVOID)) ((uint8_t *)hModule + 0x1EDCC0);
  return method(movingStructure);
}

PVOID pnt;

PVOID __thiscall test_Method_BB8FD(PVOID movingStructure) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  builder_PrintMemoryLayout(movingStructure, 0xB8);
  PVOID __thiscall (*method)(PVOID) = (PVOID __thiscall (*)(PVOID)) ((uint8_t *)hModule + 0x1EBC86);
  return method(movingStructure);;
}

PVOID __cdecl test_Method_BB8F0(PVOID movingStructure) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  PVOID __cdecl (*method)(PVOID) = (PVOID __cdecl (*)(PVOID)) ((uint8_t *)hModule + 0x29D178);
  pnt = method(movingStructure);
  eeTa_FilePrintf("Alocated memory at %p\n", pnt);
  return pnt;
}

PVOID __thiscall test_Method_BB9CD(PVOID movingStructure, PVOID _a, PVOID _b) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID)) ((uint8_t *)hModule + 0x1EDC93);
  builder_Store(movingStructure, 0xB8);
  pnt = method(movingStructure, _a, _b);
  builder_CheckChanges(movingStructure);
  eeTa_FilePrintf("Spoc Method %p - %p - %p\n", movingStructure, _a, _b);
  return pnt;
}

void replace_AnchorMethods() {
  builder_Definition((PVOID)0xBB9DB, (PVOID)test_MoveToUnit);
  // builder_Definition((PVOID)0xBB8FD, (PVOID)test_Method_BB8FD);
  builder_Definition((PVOID)0xBB8F0, (PVOID)test_Method_BB8F0);
  builder_Definition((PVOID)0xBB9CD, (PVOID)test_Method_BB9CD);
}

void bt_OnInit() {
  eeTa_FilePrintf("Changing at %p\n", (size_t)GetModuleHandleA("EE-AOC.exe") + (size_t)0xBB9D8);
  replace_AnchorMethods();
}

void bt_OnFrame() {
  execDataPengus();
}

void bt_OnUnitDestroy(Unit unit) {

}