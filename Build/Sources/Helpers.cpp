#include "Helpers.h"
#include <iostream>
#include "EETwa.h"
#include <unordered_map>
#include <stdint.h>
#include <Windows.h>
#include "MethodsDefinitions.h"
#include "Helpers.h"
#include "LibManager.h"
#include "EETypes.h"
#include "Geometry.h"

#define ACTION_BUFFER_SIZE 0xB8

using namespace std;

typedef struct TileStruct_t {
  PVOID ref;
  TilePoint tile;
} TileStruct;

typedef struct MoveAction_t {
  PVOID methodsBundle; // 0x0
  PVOID _const_1; // 0x4
  PVOID _known_Const; // 0x8
  PVOID _const_3; // 0xC
  PVOID _const_4; // 0x10
  PVOID _const_5; // 0x14
  PVOID _const_6; // 0x18
  PVOID _const_7; // 0x1C
  Point pos; // 0x20
  float angle; // 0x28
  PVOID actionType; // 0x2C
  PVOID _const_8; // 0x30
  PVOID _const_9; // 0x34
} MoveAction;

typedef struct MoveActionUnit_t {
  PVOID methodsBundle;
  PVOID _const_1;
  PVOID _known_Const;
  PVOID _const_3;
  PVOID _const_4;
  PVOID _const_5;
  PVOID _const_6;
  PVOID _const_7;
  Point pos1;
  float angle1;
  Point pos2;
  float angle2;
  PVOID target;
} MoveActionUnit;

typedef struct ActionBuffer_t {
  uint8_t *buffer;
  uint8_t validAddress;
} ActionBuffer;

PVOID __fastcall help_SearchUnits(PVOID self);
MoveAction *help_GetAction(PVOID parent, Point pos, UnitAction action);
void __cdecl help_Delete(PVOID pointer);
PVOID __thiscall help_Checker_4C2A3C(PVOID self, PVOID _1, PVOID _2, PVOID _3);
void helper_Convert_Fill(PVOID mem, PVOID unitAction, PVOID unit);
void helper_Convert_Secondary(PVOID unitAction, PVOID src, PVOID dst);
PVOID __fastcall helper_ConvertUnit(PVOID movingStructure);
void helper_CastPoint(PVOID unit, Point target, Ability ability);

static unordered_map<PVOID, size_t> memoryMap; // No need this into the general pointer

void help_FillData_NonAir(PVOID buffer);
void help_FillData_Air(PVOID buffer);
MoveActionUnit *help_GetActionUnit(PVOID parent, PVOID unit);

PVOID __cdecl help_New(size_t size) {
  PVOID __cdecl (*method)(size_t) = (PVOID __cdecl (*)(size_t)) ((uint8_t *)lib_BaseAddress() + 0x29D178);
  PVOID response = method(size);
  if(!response) {
    return NULL;
  }
  memset(response, 0, size);
  return response;
}

void helper_Convert_FillConstants(PVOID mem, PVOID currentUnit) {
  PVOID unitBuffer = help_New(0x8);
  builder_FillValue(unitBuffer, 0x0, (size_t)currentUnit);
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

void helper_Convert(PVOID src, PVOID dst) {
  PVOID actionMove = help_New(0xB8);
  PVOID secondary = help_New(0x44);
  memset(actionMove, 0x0, 0xB8);
  memset(secondary, 0x0, 0x44);
  helper_Convert_Fill(actionMove, secondary, src);
  builder_FillValue(actionMove, 0x68, (size_t)secondary);
  helper_Convert_Secondary(secondary, src, dst);
  builder_FillValue(actionMove, 0x6C, (size_t)0x7D1);
  helper_ConvertUnit(actionMove);
}

void helper_Convert_Secondary(PVOID unitAction, PVOID src, PVOID dst) {
  PVOID classNameRef = (PVOID)((size_t)lib_BaseAddress() + (size_t)0x447380);
  builder_FillValue(unitAction, 0x0, (size_t)classNameRef);
  builder_FillValue(unitAction, 0x4, 0x201);
  builder_FillValue(unitAction, 0x8, 0x1388);
  builder_FillValue(unitAction, 0x10, 0x5FE);
  builder_FillValue(unitAction, 0x14, 0x3);
  builder_FillValue(unitAction, 0x1C, 0x1);
  builder_FillValue(unitAction, 0x20, 0x424A0000);
  builder_FillValue(unitAction, 0x24, 0x424A0000);
  builder_FillValue(unitAction, 0x2C, (size_t)src);
  builder_FillValue(unitAction, 0x30, (size_t)dst);
  builder_FillValue(unitAction, 0x34, 0x32);
  builder_FillValue(unitAction, 0x38, 0x31);
}

PVOID helper_FindSuperClass_BB884() {
  PVOID hModule = lib_BaseAddress();
  PVOID param = (PVOID)((size_t)hModule + (size_t)0x530D40);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)hModule + 0x139670);
  return method(param, (PVOID)0xFE);
}

PVOID helper_Fill_BB8FD(PVOID moveAction, PVOID baseClass) {
  PVOID hModule = lib_BaseAddress();
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)hModule + 0x1EBC86);
  return method(moveAction, baseClass);
}

void helper_Convert_Fill(PVOID mem, PVOID unitAction, PVOID unit) {
  PVOID baseAddres = helper_FindSuperClass_BB884();
  helper_Fill_BB8FD(mem, baseAddres);
  helper_Convert_FillConstants(mem, unit);
}

PVOID __fastcall helper_ConvertUnit(PVOID movingStructure) {
  PVOID hModule = lib_BaseAddress();
  PVOID __fastcall (*method)(PVOID) = (PVOID __fastcall (*)(PVOID)) ((uint8_t *)hModule + 0x1EDCC0);
  return method(movingStructure);
}

void __cdecl help_Delete(PVOID pointer) {
  PVOID __cdecl (*method)(PVOID) = (PVOID __cdecl (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0x29D150);
  method(pointer);
}

void help_Convert(PVOID src, PVOID dst) {

}

string help_DisplayDiff(PVOID src, PVOID dst, size_t sz) {
  string response = "";
  int32_t *diffSrc = (int32_t *)src;
  int32_t *diffDst = (int32_t *)dst;
  for(int32_t i = 0, t = 0; i < sz; t++, i += 4) {
    if(diffSrc[t] != diffDst[t]) {
      char pengus[256] = {0};
      sprintf(pengus, "{0x%p, 0x%p, 0x%p},\n", i, diffDst[t], diffSrc[t]);
      response += string(pengus);
    }
  }
  return response;
}

PVOID _snapshot(PVOID self, size_t sz) {
  PVOID buffer = malloc(sz);
  memcpy(buffer, self, sz);
  return buffer;
}

string searchDiffs(PVOID self) {
  string response = "";
  int32_t *diffSrc = (int32_t *)self;
  for(int32_t i = 0, t = 0, c = memoryMap[self]; i < c; t++, i += 4) {
    if(diffSrc[t] != 0) {
      char pengus[256] = {0};
      sprintf(pengus, "%p != %p at %p\n", diffSrc[t], 0, i);
      response += string(pengus);
    }
  }
  return response;
}

void addPositionDiff(PVOID pos, string &current, size_t size) {
  int32_t *diffSrc = (int32_t *)pos;
  current += "\n------------\n";
  for(int32_t i = 0, t = 0, c = size; i < c; t++, i += 4) {
    if(diffSrc[t] != 0) {
      char pengus[256] = {0};
      sprintf(pengus, "%p != %p at %p\n", diffSrc[t], 0, i);
      current += string(pengus);
    }
  }
}

__declspec(dllexport) void help_AddSelectedUnitToBuffer(PVOID self, PVOID unit) {
  PVOID derefStruct = util_Pointer(self, 0x0, POINTER_TYPE);
  PVOID methodStruct = util_Pointer(derefStruct, 0x4, POINTER_TYPE);
  void __thiscall (*method)(PVOID, PVOID) = (void __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);

  method(self, unit);
}

void help_SetActionPointer(PVOID self, Point pos, UnitAction action) {
  MoveAction *actionPointer = help_GetAction(self, pos, action);
  memcpy((PVOID)((size_t)self + 0x68), &actionPointer, sizeof(PVOID));
}

void help_SetActionPointerUnit(PVOID self, PVOID unit) {
  MoveActionUnit *actionPointer = help_GetActionUnit(self, unit);
  memcpy((PVOID)((size_t)self + 0x68), &actionPointer, sizeof(PVOID));
}

string searchDiffsDracu(PVOID self) {
  string response = "";
  int32_t *diffSrc = (int32_t *)self;
  for(int32_t i = 0, t = 0, c = 56; i < c; t++, i += 4) {
    if(diffSrc[t] != 0) {
      char pengus[256] = {0};
      sprintf(pengus, "%p != %p at %p\n", diffSrc[t], 0, i);
      response += string(pengus);
    }
  }
  return response;
}

void help_SetActionPointerTset(PVOID self, Point pos, UnitAction action) {
  MoveAction *actionPointer = help_GetAction(self, pos, action);
  memcpy((PVOID)((size_t)self + 0x68), &actionPointer, sizeof(PVOID));
}

void helper_ReplaceOrder(PVOID unit, Point target, Ability ability) {
  PVOID unitBuffer = help_New(0x34);
  int32_t valX = (int32_t)target.x;
  int32_t valY = (int32_t)target.y;
  builder_FillValue(unitBuffer, 0x0, 0x44748C + (size_t)lib_BaseAddress());
  builder_FillValue(unitBuffer, 0x4, 2817);
  builder_FillValue(unitBuffer, 0x8, 5000);
  builder_FillValue(unitBuffer, 0xc, 0x10235);
  builder_FillValue(unitBuffer, 0x10, 0x39ED);
  builder_FillValue(unitBuffer, 0x14, 4);
  builder_FillValue(unitBuffer, 0x18, 2);
  builder_FillValue(unitBuffer, 0x1C, 257);
  builder_FillValue(unitBuffer, 0x20, (size_t)unit);
  builder_FillValue(unitBuffer, 0x24, 0);
  builder_FillValue(unitBuffer, 0x28, valX);
  builder_FillValue(unitBuffer, 0x2C, valY);
  builder_FillValue(unitBuffer, 0x30, ability);
  memcpy((PVOID)((size_t)unit + 0x1EC), &unitBuffer, sizeof(size_t));
  memcpy((PVOID)((size_t)unit + 0x1F0), &unitBuffer, sizeof(size_t));
}

void helper_CastPoint(PVOID unit, Point target, Ability ability) {
  help_UnitMove(unit, target, UNIT_ATTACK);
  helper_ReplaceOrder(unit, target, ability);
}



void helper_ReplaceECOrder(PVOID unit, Point target, Ability ability) {
  PVOID unitBuffer = help_New(0x34);
  builder_FillValue(unitBuffer, 0x0, 0x44748C + (size_t)lib_BaseAddress());
  builder_FillValue(unitBuffer, 0x4, 2817);
  builder_FillValue(unitBuffer, 0x8, 8000);
  builder_FillValue(unitBuffer, 0xc, 0x0);
  builder_FillValue(unitBuffer, 0x10, 0xA5CB);
  builder_FillValue(unitBuffer, 0x14, 4);
  builder_FillValue(unitBuffer, 0x18, 2);
  builder_FillValue(unitBuffer, 0x1C, 0x101);
  builder_FillValue(unitBuffer, 0x20, (size_t)unit);
  builder_FillValue(unitBuffer, 0x24, 0);
  builder_FillValue(unitBuffer, 0x28, 32);
  builder_FillValue(unitBuffer, 0x2C, 38);
  builder_FillValue(unitBuffer, 0x30, ability);
  memcpy((PVOID)((size_t)unit + 0x1EC), &unitBuffer, sizeof(size_t));
  // memcpy((PVOID)((size_t)unit + 0x1F0), &unitBuffer, sizeof(size_t));
}

void helper_ReplaceF0Order(PVOID unit, Point target, Ability ability) {
  PVOID unitBuffer = help_New(0x38);
  float valX = (float)(int32_t)target.x + 0.5f;
  float valY = (float)(int32_t)target.y + 0.5f;
  builder_FillValue(unitBuffer, 0x0, 0x4478A0 + (size_t)lib_BaseAddress());
  builder_FillValue(unitBuffer, 0x4, 0x101);
  builder_FillValue(unitBuffer, 0x8, 5000);
  builder_FillValue(unitBuffer, 0xc, 0x1A0B1);
  builder_FillValue(unitBuffer, 0x10, 0x19980);
  builder_FillValue(unitBuffer, 0x14, 0x13);
  builder_FillValue(unitBuffer, 0x18, 1);
  builder_FillValue(unitBuffer, 0x1C, 0x1);
  builder_FillValue(unitBuffer, 0x20, valX);
  builder_FillValue(unitBuffer, 0x24, valY);
  builder_FillValue(unitBuffer, 0x28, 0);
  builder_FillValue(unitBuffer, 0x2C, 0);
  builder_FillValue(unitBuffer, 0x30, 0x00010001);
  builder_FillValue(unitBuffer, 0x34, 0);
  memcpy((PVOID)((size_t)unit + 0x1F0), &unitBuffer, sizeof(size_t));
  // memcpy((PVOID)((size_t)unit + 0x1F0), &unitBuffer, sizeof(size_t));
}

__declspec(dllexport) void helper_CastPointInTesting(PVOID unit, Point target, Ability ability) {
  help_UnitMove(unit, target, UNIT_ATTACK);
  helper_ReplaceECOrder(unit, target, ability);
  helper_ReplaceF0Order(unit, target, ability);
}



__declspec(dllexport)  PVOID __fastcall help_SearchUnits(PVOID self) {
  PVOID __thiscall (*method)(PVOID) = (PVOID __thiscall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0x1EDCC0);
  return method(self);
}

PVOID help_Const_x14_Value() {
  int32_t *value = (int32_t *)util_Pointer(lib_BaseAddress(), 0x564480, INT32_T_TYPE);
  (*value) += 1;
  return (PVOID)*value;
}

PVOID help_Const_x10_Value() {
  PVOID addressAt = util_Pointer(lib_BaseAddress(), 0x5318F0, POINTER_TYPE);

  return util_Pointer(addressAt, 0xD0, POINTER_TYPE);
}

MoveAction *help_GetAction(PVOID parent, Point pos, UnitAction action) {
  MoveAction *self = (MoveAction *)help_New(sizeof(MoveAction));
  self->methodsBundle = (PVOID)((size_t)lib_BaseAddress() + 0x4478A0);
  self->_const_1 = (PVOID)0x101;
  self->_known_Const = (PVOID)0x1388;
  self->_const_4 = help_Const_x10_Value();
  self->_const_5 = help_Const_x14_Value();
  self->_const_7 = (PVOID)0x1;
  self->pos.x = pos.x;
  self->pos.y = pos.y;
  int32_t angle = 0xBFB969E4;
  memcpy(&self->angle, &angle, sizeof(float));
  self->actionType = (PVOID)action;
  return self;
}

void __fastcall help_FillMetaParameters(PVOID tempStruct) {
  PVOID basePointer = lib_BaseAddress();
  PVOID generalStruct = util_Pointer(basePointer, 0x530DB8, POINTER_TYPE);
  int8_t __thiscall (*method)(PVOID, PVOID, PVOID) = (int8_t __thiscall (*)(PVOID, PVOID, PVOID)) ((uint8_t *)basePointer + 0x1EBC86);
  method(tempStruct, generalStruct, generalStruct);
}

void help_AddMagicFlag(ActionBuffer self) {
  size_t specialNumber = 0x1F40;
  memcpy(&self.buffer[0x6C], &specialNumber, sizeof(size_t));
}

void help_AddConvertMagicFlag(ActionBuffer self) {
  size_t specialNumber = 0x7D1;
  memcpy(&self.buffer[0x6C], &specialNumber, sizeof(size_t));
}

void help_RunMethod_4C2AAD(PVOID self) {
  int8_t __fastcall (*method)(PVOID) = (int8_t __fastcall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0x1EDAC5);
  method(self);
}

void help_RunMethod_4C2A67(PVOID self) {
  PVOID __fastcall (*method)(PVOID) = (PVOID __fastcall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0x1EF56D);
  method(self);
}

void help_RunMethod_4BB02A(PVOID self) {
  PVOID __fastcall (*method)(PVOID) = (PVOID __fastcall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0xBB02A);
  method(self);
}

void help_RunMethod_4C2A60(PVOID self) {
  PVOID basePointer = lib_BaseAddress();
  PVOID generalStruct = util_Pointer(basePointer, 0x530DB8, POINTER_TYPE);
  PVOID __fastcall (*method)(PVOID) = (PVOID __fastcall (*)(PVOID)) ((uint8_t *)basePointer + 0xC8F99);
  method(&self);
}

PVOID __thiscall help_Checker_4C2A3C(PVOID self, PVOID _1, PVOID _2, PVOID _3) {
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)lib_BaseAddress() + 0x1ED9D4);
  return method(self, _1, _2, _3);
}

void help_SetPlayerActionPointer(PVOID unit, PVOID self) {
  memcpy((PVOID)((size_t)unit + 0x1C8), &self, sizeof(PVOID));
}

int32_t help_DerefChecker(PVOID self) {
  PVOID derefStruct = util_Pointer(lib_BaseAddress(), 0x0, POINTER_TYPE);
  PVOID callerStruct = util_Pointer(derefStruct, 0x18, POINTER_TYPE);
  int32_t __fastcall (*method)(PVOID) = (int32_t __fastcall (*)(PVOID)) ((uint8_t *)callerStruct);

  return method(self);
}

int32_t help_DerefCounter(PVOID ecx) {
  PVOID methodsBundle = util_Pointer(ecx, 0x0, POINTER_TYPE);
  PVOID derefMethod = util_Pointer(methodsBundle, 0x18, POINTER_TYPE);
  int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)derefMethod);

  return method(ecx);
}

int32_t derefPointer(PVOID ecx) {
  PVOID methodsAddress = (PVOID)((size_t)lib_BaseAddress() + 0x4371A8);
  PVOID methodsBundle = util_Pointer(methodsAddress, 0x0, POINTER_TYPE);
  int32_t (*method)(PVOID) = (int32_t (*)(PVOID)) ((uint8_t *)methodsBundle);

  return method(ecx);
}

void help_ActionBuffer(PVOID unit, Point point, UnitAction action) {
  ActionBuffer self = {0};
  self.validAddress = 1;
  for(int32_t i = 1; i <= 1; i++) {
    self.buffer = (uint8_t *)help_New(ACTION_BUFFER_SIZE);
    if(!self.buffer) {
      self.validAddress = 0;
      return ;
    }
    help_FillMetaParameters(self.buffer);
    help_AddSelectedUnitToBuffer(self.buffer, unit);
    if(help_DerefCounter(self.buffer) <= 0) {
      derefPointer(self.buffer);
      continue;
    }
    // help_RunMethod_4C2A60(self.buffer);
    help_RunMethod_4C2A67(self.buffer);
    help_RunMethod_4C2AAD(self.buffer);
    help_RunMethod_4BB02A(self.buffer);
    help_AddMagicFlag(self);
    help_SetActionPointer(self.buffer, point, action);
    help_SearchUnits(self.buffer);
  }
  // help_SetPlayerActionPointer(unit, self.buffer);
}

void help_UnitMove(PVOID unit, Point point, UnitAction currentAction) {
  if(!unit) {
    return ;
  }
  help_ActionBuffer(unit, point, currentAction);
}

void help_FillData_NonAir(PVOID buffer) {
  const size_t data[][2] = {
    {0x00000038, 0x00023002}, // Needs to be revised.
    {0x00000070, 0x3F4CCCCD},
    {0x00000074, 0x3F4CCCCD},
    {0x00000078, 0x3F4CCCCD},
    {0x0000007C, 0x3FC00000},
    {0x00000080, 0x3FC00000},
    {0x00000084, 0x3FC00000},
    {0x00000088, 0x40C90FDB},
    {0x0000008C, 0x40C90FDB},
    {0x00000090, 0x40C90FDB},
    {0x00000094, 0x40C90FDB},
    {0x00000098, 0x40C90FDB},
    {0x0000009C, 0x40C90FDB},
    {0x000000A0, 0x3FA00000},
    {0x000000A4, 0x3F2AAAAB},
    {0x000000A8, 0x3E22F983},
    {0x000000AC, 0x3E22F983}
  };

  for(size_t i = 0, c = sizeof(data) / (sizeof(size_t) * 2); i < c; i++) {
    memcpy((PVOID)((size_t)buffer + data[i][0]), &data[i][1], sizeof(size_t));
  }
}

void help_FillData_Air(PVOID buffer) {
  const size_t data[][2] = {
    {0x00000038, 0x00000002},
    {0x00000070, 0x3F99999A},
    {0x00000074, 0x3F99999A},
    {0x00000078, 0x3F99999A},
    {0x0000007C, 0x3F800000},
    {0x00000080, 0x3F800000},
    {0x00000084, 0x3F800000},
    {0x00000088, 0x3F32B8C3},
    {0x0000008C, 0x3F32B8C3},
    {0x00000090, 0x3F32B8C3},
    {0x00000094, 0x3F490FDB},
    {0x00000098, 0x3F490FDB},
    {0x0000009C, 0x3F490FDB},
    {0x000000A0, 0x3F555555},
    {0x000000A4, 0x3F800000},
    {0x000000A8, 0x3FB758B3},
    {0x000000AC, 0x3FA2F983},
  };

  for(size_t i = 0, c = sizeof(data) / (sizeof(size_t) * 2); i < c; i++) {
    memcpy((PVOID)((size_t)buffer + data[i][0]), &data[i][1], sizeof(size_t));
  }
}

MoveActionUnit *help_GetActionUnit(PVOID parent, PVOID unit) {
  Point pos = eeTa_CurrentPosition((Unit) {._payload = unit});
  MoveActionUnit *self = (MoveActionUnit *)help_New(0x44);
  self->methodsBundle = (PVOID)((size_t)lib_BaseAddress() + 0x4475A8); // 8478A0
  self->_const_1 = (PVOID)0x601;
  self->_known_Const = (PVOID)0x1388;
  self->_const_4 = help_Const_x10_Value();
  self->_const_5 = help_Const_x14_Value();
  self->_const_7 = (PVOID)0x1;
  self->pos1.x = pos.x;
  self->pos1.y = pos.y;
  self->pos2 = self->pos1;
  self->target = unit;
  int32_t angle = 0xBFB969E4;
  memcpy(&self->angle1, &angle, sizeof(float));
  memcpy(&self->angle2, &angle, sizeof(float));
  return self;
}

void help_AddMagicFlagToTarget(ActionBuffer self) {
  size_t specialNumber = 0xFA4;
  memcpy(&self.buffer[0x6C], &specialNumber, sizeof(size_t));
}

void __cdecl help_MoveToTarget(PVOID unit, PVOID target) {
  ActionBuffer self = {0};
  self.validAddress = 1;
  for(int32_t i = 1; i <= 1; i++) {
    self.buffer = (uint8_t *)help_New(ACTION_BUFFER_SIZE);
    if(!self.buffer) {
      self.validAddress = 0;
      return ;
    }
    help_FillMetaParameters(self.buffer);
    help_AddSelectedUnitToBuffer(self.buffer, unit);
    if(help_DerefCounter(self.buffer) <= 0) {
      derefPointer(self.buffer);
      continue;
    }
    help_RunMethod_4C2A67(self.buffer);
    help_RunMethod_4C2AAD(self.buffer);
    help_RunMethod_4BB02A(self.buffer);
    help_AddMagicFlagToTarget(self);
    help_SetActionPointerUnit(self.buffer, target);
    help_SearchUnits(self.buffer);
  }
}

void __cdecl help_ConvertTarget(PVOID unit, PVOID target) {
  ActionBuffer self = {0};
  self.validAddress = 1;
  for(int32_t i = 1; i <= 1; i++) {
    self.buffer = (uint8_t *)help_New(ACTION_BUFFER_SIZE);
    if(!self.buffer) {
      self.validAddress = 0;
      return ;
    }
    help_FillMetaParameters(self.buffer);
    help_AddSelectedUnitToBuffer(self.buffer, unit);
    if(help_DerefCounter(self.buffer) <= 0) {
      derefPointer(self.buffer);
      continue;
    }
    help_RunMethod_4C2A67(self.buffer);
    help_RunMethod_4C2AAD(self.buffer);
    help_RunMethod_4BB02A(self.buffer);
    help_AddConvertMagicFlag(self);
    help_SetActionPointerUnit(self.buffer, target);
    help_SearchUnits(self.buffer);
  }
}

void help_MoveSecondMethod(PVOID unit, Point target) {
  // just for testing, not good enough.
  PVOID unitBuffer = help_New(0x38);
  float orientation = -1.93f;
  size_t xPos = *((size_t *)&target.x);
  size_t yPos = *((size_t *)&target.y);
  size_t orientantionInteger = *((size_t *)&orientation);

  builder_FillValue(unitBuffer, 0x0, 0x008478A0);
  builder_FillValue(unitBuffer, 0x4, 257);
  builder_FillValue(unitBuffer, 0x8, 5000);
  builder_FillValue(unitBuffer, 0xc, 1101130);
  builder_FillValue(unitBuffer, 0x10, 1101130);
  builder_FillValue(unitBuffer, 0x14, 64);
  builder_FillValue(unitBuffer, 0x18, 1);
  builder_FillValue(unitBuffer, 0x1C, 257);
  builder_FillValue(unitBuffer, 0x20, xPos);
  builder_FillValue(unitBuffer, 0x24, yPos);
  builder_FillValue(unitBuffer, 0x28, 0);
  builder_FillValue(unitBuffer, 0x2C, orientantionInteger);
  builder_FillValue(unitBuffer, 0x30, 257);
  builder_FillValue(unitBuffer, 0x34, 3);
  memcpy((PVOID)((size_t)unit + 0x1EC), &unitBuffer, sizeof(size_t));
  memcpy((PVOID)((size_t)unit + 0x1F0), &unitBuffer, sizeof(size_t));
}

PVOID helper_CreateCalamityStruct(Point pos, Ability ability) {
  PVOID calamityBuffer = help_New(0x24);
  int32_t xPos = (int32_t)pos.x;
  int32_t yPos = (int32_t)pos.y;

  builder_FillValue(calamityBuffer, 0x0, (size_t)lib_BaseAddress() + 0x438B98);
  builder_FillValue(calamityBuffer, 0x4, 2);
  builder_FillValue(calamityBuffer, 0x8, 0xFEFF75);
  builder_FillValue(calamityBuffer, 0xC, 2);
  builder_FillValue(calamityBuffer, 0x10, 0x1000004);
  builder_FillValue(calamityBuffer, 0x14, 0xFFFFFFFF);
  builder_FillValue(calamityBuffer, 0x18, xPos);
  builder_FillValue(calamityBuffer, 0x1C, yPos);
  builder_FillValue(calamityBuffer, 0x20, ability);

  return calamityBuffer;
}

void helper_FillCalamityStruct(PVOID unit, PVOID calamityStruct, PVOID originalPointer) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x2209C9);
  void __thiscall (*method)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID) = (void __thiscall (*)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(originalPointer, 
         unit, 
         0x0, 
         (PVOID)*(size_t *)((size_t)calamityStruct + 0x18), 
         (PVOID)*(size_t *)((size_t)calamityStruct + 0x1C),
         (PVOID)*(size_t *)((size_t)calamityStruct + 0x20), 
         (PVOID)0x1);
}

void helper_AddCommandToUnit(PVOID unit, PVOID calamityStruct) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FE863);
  void __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (void __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(unit, calamityStruct, NULL, NULL);
}

void helper_UnknownMethod4BC7AF(PVOID unit) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FDFA5);
  void __thiscall (*method)(PVOID, PVOID) = (void __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(unit, 
         (PVOID)0x1F40);
}

void helper_CastAbility(PVOID unit, Point target, Ability ability) {
  PVOID pntTarget = help_New(0x34);
  PVOID calamityStruct = helper_CreateCalamityStruct(target, ability);
  helper_FillCalamityStruct(unit, calamityStruct, pntTarget);
  helper_AddCommandToUnit(unit, pntTarget);
  helper_UnknownMethod4BC7AF(unit); // This is the method which makes the unit move.
}

PVOID helper_Unit_GetPlayer(PVOID player) {
  size_t *buffer = (size_t *)((size_t)player + 0x18);
  return (PVOID)*buffer;
}

PVOID helper_Method61E164(PVOID buffer, PVOID building, PVOID player) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x21E164);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(buffer,
         player, 
         building,
        (PVOID)0x1);
}

PVOID helper_Repair_ClassInit(PVOID buffer, PVOID unit, PVOID building) {
  PVOID player = helper_Unit_GetPlayer(unit);
  return helper_Method61E164(buffer, building, player);
}

PVOID helper_Method5FE863(PVOID buffer, PVOID unit) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FE863);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(unit, 
                buffer,
                (PVOID)0x0,
                (PVOID)0x0);
}

PVOID helper_Method5FDFA5(PVOID buffer, PVOID unit) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FDFA5);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(unit, 
                (PVOID)0x1F51);
}

void helper_Repair_PushCommandToUnit(PVOID buffer, PVOID unit) {
  helper_Method5FE863(buffer, unit);
  helper_Method5FDFA5(buffer, unit);
}

void helper_RepairBuilding(PVOID unit, PVOID building) {
  PVOID pntTarget = help_New(0x5C);
  (void)helper_Repair_ClassInit(pntTarget, unit, building);
  (void)helper_Repair_PushCommandToUnit(pntTarget, unit);
}

__declspec(dllexport) void helper_Gather_Method621E95(PVOID self, PVOID targetResource) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x221E95);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
        targetResource,
        (PVOID)0x0,
        (PVOID)0x1);
}

__declspec(dllexport) void helper_Gather_Method5FE863(PVOID self, PVOID buffer) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FE863);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
         buffer,
         (PVOID)0x0,
         (PVOID)0x0);
}

__declspec(dllexport) void helper_Gather_Method5FDFA5(PVOID self) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FDFA5);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
        (PVOID)0xFB7);
}

PVOID helper_Gather_ClassInit(PVOID buffer, PVOID targetResource) {
  helper_Gather_Method621E95(buffer, targetResource);
}

__declspec(dllexport) void helper_Gather_Citizen_QueueCommand(PVOID unit, PVOID buffer) {
  helper_Gather_Method5FE863(unit, buffer);
  helper_Gather_Method5FDFA5(unit);
}

__declspec(dllexport) void helper_Citizen_Gather(PVOID unit, PVOID resource) {
  PVOID buffer = help_New(0x44);
  helper_Gather_ClassInit(buffer, resource);
  helper_Gather_Citizen_QueueCommand(unit, buffer);
}