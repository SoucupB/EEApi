#include "Helpers.h"
#include <iostream>
#include "EETwa.h"
#include <unordered_map>

static unordered_map<PVOID, size_t> memoryMap;

void help_FillData_NonAir(PVOID buffer);
void help_FillData_Air(PVOID buffer);
MoveActionUnit *help_GetActionUnit(PVOID parent, PVOID unit);

PVOID __cdecl help_New(size_t size) {
  PVOID __cdecl (*method)(size_t) = (PVOID __cdecl (*)(size_t)) ((uint8_t *)GetModuleHandleA("EE-AOC.exe") + 0x29D178);
  PVOID response = method(size);
  if(!response) {
    return NULL;
  }
  memset(response, 0, size);
  // memoryMap[response] = size;
  return response;
}

void __cdecl help_Delete(PVOID pointer) {
  PVOID __cdecl (*method)(PVOID) = (PVOID __cdecl (*)(PVOID)) ((uint8_t *)GetModuleHandleA("EE-AOC.exe") + 0x29D150);
  method(pointer);
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

void cleanSlate(PVOID self) {
  memset((PVOID)((size_t)self + 0x70), 0, 0x3C);
}

PVOID __thiscall help_FillSelectedUnits(PVOID self, size_t _a, size_t _b, size_t _c) {
  PVOID __thiscall (*method)(PVOID, size_t, size_t, size_t) = (PVOID __thiscall (*)(PVOID, size_t, size_t, size_t)) ((uint8_t *)GetModuleHandleA("EE-AOC.exe") + 0x1ED9D4);
  size_t bufferSize = 0xB8;
  PVOID deepCopy = _snapshot(self, bufferSize);
  PVOID rsp = method(self, _a, _b, _c);
  if(_c == 1) {
    // cleanSlate(self);
    string response = help_DisplayDiff(self, deepCopy, bufferSize);
    eeTa_Printf("Pengus call %p %p %p %p -> \n%s", self, _a, _b, _c, &response[0]);
  }
  free(deepCopy);
  return rsp;
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
  // eeTa_Printf("Pengus call %p %p %p %p\n -> \n%s", actionPointer, &actionPointer->pos.x, &actionPointer->pos.y, &actionPointer->pos.y - (size_t)actionPointer);
  memcpy((PVOID)((size_t)self + 0x68), &actionPointer, sizeof(PVOID));
}

void help_SetActionPointerUnit(PVOID self, PVOID unit) {
  MoveActionUnit *actionPointer = help_GetActionUnit(self, unit);
  // eeTa_Printf("Pengus call %p %p %p %p\n -> \n%s", actionPointer, &actionPointer->pos.x, &actionPointer->pos.y, &actionPointer->pos.y - (size_t)actionPointer);
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
  // memcpy(actionPointer, (PVOID)(*(uint32_t *)((size_t)self + 0x68)), 0x38);
  // actionPointer->angle = (float)0xC02730BD;
  memcpy((PVOID)((size_t)self + 0x68), &actionPointer, sizeof(PVOID));
}


__declspec(dllexport)  PVOID __fastcall help_SearchUnits(PVOID self) {
  PVOID __thiscall (*method)(PVOID) = (PVOID __thiscall (*)(PVOID)) ((uint8_t *)GetModuleHandleA("EE-AOC.exe") + 0x1EDCC0);
  string response;
  addPositionDiff((PVOID)(*(uint32_t *)((size_t)self + 0x68)), response, 0x44);
  // string s = searchDiffsDracu((PVOID)(*(uint32_t *)((size_t)self + 0x68)));
  // addPositionDiff(self, response, 0xB8);
  eeTa_Printf("Perseus call\n%s\n", &response[0]);
  // // help_Delete((PVOID)((size_t)self + 0x68));
  // // changeSelectedUnits(self, eeTa_Unit_Sample(eeTa_SelfPlayer()));
  // help_SetActionPointerTset(self, (Point) {
  //   .x = 50.5f,
  //   .y = 1500.5f
  // }, UNIT_MOVE);
  PVOID caller = method(self);
  return caller;
}

PVOID help_Const_x14_Value() {
  int32_t *value = (int32_t *)util_Pointer(GetModuleHandleA("EE-AOC.exe"), 0x564480, INT32_T_TYPE);
  (*value) += 1;
  return (PVOID)*value;
}

PVOID help_Const_x10_Value() {
  PVOID addressAt = util_Pointer(GetModuleHandleA("EE-AOC.exe"), 0x5318F0, POINTER_TYPE);

  return util_Pointer(addressAt, 0xD0, POINTER_TYPE);
}

MoveAction *help_GetAction(PVOID parent, Point pos, UnitAction action) {
  MoveAction *self = (MoveAction *)help_New(sizeof(MoveAction));
  self->methodsBundle = (PVOID)((size_t)GetModuleHandleA("EE-AOC.exe") + 0x4478A0); // 8478A0
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
  PVOID basePointer = GetModuleHandleA("EE-AOC.exe");
  PVOID generalStruct = util_Pointer(basePointer, 0x530DB8, POINTER_TYPE);
  int8_t __thiscall (*method)(PVOID, PVOID, PVOID) = (int8_t __thiscall (*)(PVOID, PVOID, PVOID)) ((uint8_t *)basePointer + 0x1EBC86);
  method(tempStruct, generalStruct, generalStruct);
}

void help_AddMagicFlag(ActionBuffer self) {
  size_t specialNumber = 0x1F40;
  memcpy(&self.buffer[0x6C], &specialNumber, sizeof(size_t));
}

void help_RunMethod_4C2AAD(PVOID self) {
  PVOID basePointer = GetModuleHandleA("EE-AOC.exe");
  int8_t __fastcall (*method)(PVOID) = (int8_t __fastcall (*)(PVOID)) ((uint8_t *)basePointer + 0x1EDAC5);
  method(self);
}

void help_RunMethod_4C2A67(PVOID self) {
  PVOID basePointer = GetModuleHandleA("EE-AOC.exe");
  PVOID __fastcall (*method)(PVOID) = (PVOID __fastcall (*)(PVOID)) ((uint8_t *)basePointer + 0x1EF56D);
  method(self);
}

void help_RunMethod_4BB02A(PVOID self) {
  PVOID basePointer = GetModuleHandleA("EE-AOC.exe");
  PVOID __fastcall (*method)(PVOID) = (PVOID __fastcall (*)(PVOID)) ((uint8_t *)basePointer + 0xBB02A);
  method(self);
}

void help_RunMethod_4C2A60(PVOID self) {
  PVOID basePointer = GetModuleHandleA("EE-AOC.exe");
  PVOID generalStruct = util_Pointer(basePointer, 0x530DB8, POINTER_TYPE);
  PVOID __fastcall (*method)(PVOID) = (PVOID __fastcall (*)(PVOID)) ((uint8_t *)basePointer + 0xC8F99);
  method(&self);
}

// void help_RunMethod_4C2A60(PVOID self) {
//   PVOID basePointer = GetModuleHandleA("EE-AOC.exe");
//   PVOID generalStruct = util_Pointer(basePointer, 0x530DB8, POINTER_TYPE);
//   PVOID __fastcall (*method)(PVOID) = (PVOID __fastcall (*)(PVOID)) ((uint8_t *)basePointer + 0xC8F99);
//   method(&self);
// }

// Test method
PVOID __thiscall help_Checker_4C2A3C(PVOID self, PVOID _1, PVOID _2, PVOID _3) {
  PVOID basePointer = GetModuleHandleA("EE-AOC.exe");
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)basePointer + 0x1ED9D4);
  return method(self, _1, _2, _3);
}

void help_SetPlayerActionPointer(PVOID unit, PVOID self) {
  memcpy((PVOID)((size_t)unit + 0x1C8), &self, sizeof(PVOID));
}

int32_t help_DerefChecker(PVOID self) {
  PVOID basePointer = GetModuleHandleA("EE-AOC.exe");
  PVOID derefStruct = util_Pointer(basePointer, 0x0, POINTER_TYPE);
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
  PVOID methodsAddress = GetModuleHandleA("EE-AOC.exe") + 0x4371A8;
  PVOID methodsBundle = util_Pointer(methodsAddress, 0x0, POINTER_TYPE);
  int32_t (*method)(PVOID) = (int32_t (*)(PVOID)) ((uint8_t *)methodsBundle);

  return method(ecx);
}

// __declspec(dllexport) ActionBuffer help_ActionBuffer(PVOID unit, Point point, UnitAction action) {
//   ActionBuffer self = {0};
//   self.validAddress = 1;
//   for(int32_t i = 1; i <= 6; i++) {
//     self.buffer = (uint8_t *)help_New(ACTION_BUFFER_SIZE);
//     if(!self.buffer) {
//       self.validAddress = 0;
//       return self;
//     }
//     help_FillMetaParameters(self.buffer);
//     if(!changeSelectedUnits(self.buffer, unit)) {
//       help_Delete(self.buffer);
//       self.validAddress = 0;
//       return self;
//     }
//     if(help_DerefCounter(self.buffer) <= 0) {
//       derefPointer(self.buffer);
//       continue;
//     }
//     // help_RunMethod_4C2A60(self.buffer);
//     help_RunMethod_4C2A67(self.buffer);
//     help_RunMethod_4C2AAD(self.buffer);
//     help_RunMethod_4BB02A(self.buffer);
//     help_AddMagicFlag(self);
//     help_SetActionPointer(self.buffer, point, action);
//     return self;
//   }
//   // help_SetPlayerActionPointer(unit, self.buffer);
//   return self;
// }

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

void printString(PVOID buffer) {
  string s = searchDiffs(buffer);
  eeTa_Printf("Pengus mengus \n%s\n", &s[0]);
}

void help_UnitMove(PVOID unit, Point point, UnitAction currentAction) {
  if(!unit) {
    return ;
  }
  help_ActionBuffer(unit, point, currentAction);
  // ActionBuffer action = help_ActionBuffer(unit, point, currentAction);
  // if(!action.validAddress) {
  //   return ;
  // }
  // help_SearchUnits(action.buffer);
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
  self->methodsBundle = (PVOID)((size_t)GetModuleHandleA("EE-AOC.exe") + 0x4475A8); // 8478A0
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