#include <math.h>
#include "AttackDecision.h"
#include "PlayerState.h"
#include "ResourceManager.h"
#include "MethodsDefinitions.h"

void test_PrintUnits();
void test_ConvertEnemy();
void test_Convert(Unit src, Unit dst);
void test_Move();

void execDataPengus() {
  if(GetAsyncKeyState('J') & 0x8000) {
    test_PrintUnits();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('P') & 0x8000) {
    test_ConvertEnemy();
    Beep (300, 250);
  }
  // if(GetAsyncKeyState('Z') & 0x8000) {
  //   test_Move();
  //   Beep (300, 250);
  // }
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
  // test_Convert(priest, enemy);
  eeTa_ConvertUnit(priest, enemy);
  eeTa_FilePrintf("Unit pointer: %p, is converting %p\n", eeTa_Reference(priest), eeTa_Reference(enemy));
  // eeTa_MoveTo(priest, enemy);
}

PVOID __fastcall test_MoveToUnit(PVOID movingStructure) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
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
  builder_Store(pnt, 0xB8);
  pnt = method(movingStructure, _a, _b);
  builder_CheckChanges(pnt);
  eeTa_FilePrintf("Spoc Method %p - %p - %p\n", movingStructure, _a, _b);
  return pnt;
}

PVOID __thiscall test_Method_BB94D(PVOID movingStructure, PVOID _a, PVOID _b, PVOID _c) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)hModule + 0x1EECB2);
  PVOID response = method(movingStructure, _a, _b, _c);
  return response;
}


PVOID test_FindSuperClass_BB884() {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  PVOID param = (PVOID)((size_t)hModule + (size_t)0x530D40);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)hModule + 0x139670);
  return method(param, (PVOID)0xFE);
}

PVOID test_Fill_BB8FD(PVOID moveAction, PVOID baseClass) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)hModule + 0x1EBC86);
  return method(moveAction, baseClass);
}

PVOID __thiscall test_Fill_BB912(PVOID self, PVOID _a, PVOID _b, PVOID _c) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)hModule + 0x1ED9D4);
  builder_Store(pnt, 0xB8);
  PVOID resp = method(self, _a, _b, _c);
  builder_CheckChanges(pnt);
  return resp;
}

PVOID test_FindAddress_BB8FD(PVOID mem, PVOID baseAddres) {
  HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
  if(!hModule) {
    return 0;
  }
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)hModule + 0x1EBC86);
  return method(mem, baseAddres);
}

void test_FillMemCheck(PVOID mem, PVOID baseAddres) {
  // PVOID checkerAddress = test_FindAddress_BB8FD(mem, baseAddres);
  PVOID pointer = util_Pointer(baseAddres, 0x9AC, POINTER_TYPE);
  test_Fill_BB912(mem, pointer, (PVOID)0x1, (PVOID)0x1);
}

void test_Convert_FillConstants(PVOID mem, Unit currentUnit) {
  PVOID unitBuffer = help_New(0x8);
  builder_FillValue(unitBuffer, 0x0, (size_t)eeTa_Reference(currentUnit));
  builder_FillValue(mem, 0x4, 0x2);
  builder_FillValue(mem, 0x1C, 0x1);
  builder_FillValue(mem, 0x24, (size_t)unitBuffer);
  builder_FillValue(mem, 0x28, (size_t)unitBuffer + 0x4);
  builder_FillValue(mem, 0x2C, (size_t)unitBuffer + 0x4);
  builder_FillValue(mem, 0x38, 0x23C04);
  builder_FillValue(mem, 0x70, 0x3F000000);
  builder_FillValue(mem, 0x74, 0x3F000000);
  builder_FillValue(mem, 0x78, 0x3F000000);
  builder_FillValue(mem, 0x7C, 0x400E38E4);
  builder_FillValue(mem, 0x80, 0x400E38E4);
  builder_FillValue(mem, 0x84, 0x400E38E4);
  builder_FillValue(mem, 0x88, 0x41783980);
  builder_FillValue(mem, 0x8C, 0x41783980);
  builder_FillValue(mem, 0x90, 0x41783980);
  builder_FillValue(mem, 0x94, 0x41593251);
  builder_FillValue(mem, 0x98, 0x41593251);
  builder_FillValue(mem, 0x9C, 0x41593251);
  builder_FillValue(mem, 0xA0, 0x40000000);
  builder_FillValue(mem, 0xA4, 0x3EE66666);
  builder_FillValue(mem, 0xA8, 0x3D84026D);
  builder_FillValue(mem, 0xAC, 0x3D96DE33);
}

void test_Convert_Secondary(PVOID unitAction, Unit src, Unit dst) {
  PVOID classNameRef = (PVOID)((size_t)GetModuleHandleA("EE-AOC.exe") + (size_t)0x447380);
  builder_FillValue(unitAction, 0x0, (size_t)classNameRef);
  builder_FillValue(unitAction, 0x4, 0x201);
  builder_FillValue(unitAction, 0x8, 0x1388);
  builder_FillValue(unitAction, 0x10, 0x5FE);
  builder_FillValue(unitAction, 0x14, 0x3);
  builder_FillValue(unitAction, 0x1C, 0x1);
  builder_FillValue(unitAction, 0x20, 0x424A0000);
  builder_FillValue(unitAction, 0x24, 0x424A0000);
  builder_FillValue(unitAction, 0x2C, (size_t)eeTa_Reference(src));
  builder_FillValue(unitAction, 0x30, (size_t)eeTa_Reference(dst));
  builder_FillValue(unitAction, 0x34, 0x32);
  builder_FillValue(unitAction, 0x38, 0x31);
}

void test_Convert_Fill(PVOID mem, PVOID unitAction, Unit unit) {
  PVOID baseAddres = test_FindSuperClass_BB884();
  test_Fill_BB8FD(mem, baseAddres);
  test_Convert_FillConstants(mem, unit);
  // test_FillMemCheck(actionStruct, baseAddres);
}

void replace_AnchorMethods() {
  builder_Definition((PVOID)0xBB9DB, (PVOID)test_MoveToUnit);
  // builder_Definition((PVOID)0xBB912, (PVOID)test_Fill_BB912);
  // builder_Definition((PVOID)0xBB8FD, (PVOID)test_Method_BB8FD);
  // builder_Definition((PVOID)0xBB8F0, (PVOID)test_Method_BB8F0);
  // builder_Definition((PVOID)0xBB94D, (PVOID)test_Method_BB94D);
  // builder_Definition((PVOID)0xBB9CD, (PVOID)test_Method_BB9CD);

  // eeTa_FilePrintf("Base class %p\n", test_FindSuperClass_BB884());
}

// Fixed nigga
void test_Convert(Unit src, Unit dst) {
  PVOID actionMove = help_New(0xB8);
  PVOID secondary = help_New(0x44);
  memset(actionMove, 0x0, 0xB8);
  memset(secondary, 0x0, 0x44);
  // builder_Store(actionMove, 0xB8);
  // builder_Store(secondary, 0x44);
  test_Convert_Fill(actionMove, secondary, src);
  builder_FillValue(actionMove, 0x68, (size_t)secondary);
  test_Convert_Secondary(secondary, src, dst);
  builder_FillValue(actionMove, 0x6C, (size_t)0x7D1);
  test_MoveToUnit(actionMove);
  // builder_CheckChanges(actionMove);
  // builder_CheckChanges(secondary);
}

void test_Move() {
  test_Convert(eeTa_EmptyUnit(), eeTa_EmptyUnit());
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