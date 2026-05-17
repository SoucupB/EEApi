#include "Driver.h"
#include <iostream>
#include "EETwa.h"
#include <unordered_map>
#include <stdint.h>
#include <Windows.h>
#include "MethodsDefinitions.h"
#include "Driver.h"
#include "LibManager.h"
#include "EETypes.h"
#include "Geometry.h"
#include "Unit.h"
#include "EETypesStructPrivate.h"
#include "PlayerPrivate.h"

using namespace std;

PVOID __cdecl driver_New(size_t size) {
  PVOID __cdecl (*method)(size_t) = (PVOID __cdecl (*)(size_t)) ((uint8_t *)lib_BaseAddress() + 0x29D178);
  PVOID response = method(size);
  if(!response) {
    return NULL;
  }
  memset(response, 0, size);
  return response;
}

void __cdecl driver_Delete(PVOID pointer) {
  PVOID __cdecl (*method)(PVOID) = (PVOID __cdecl (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0x29D150);
  method(pointer);
}

PVOID driver_Unit_GetPlayer(PVOID player) {
  size_t *buffer = (size_t *)((size_t)player + 0x18);
  return (PVOID)*buffer;
}

PVOID driver_Method61E164(PVOID buffer, PVOID building, PVOID player) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x21E164);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(buffer,
         player, 
         building,
        (PVOID)0x1);
}

PVOID driver_Repair_ClassInit(PVOID buffer, PVOID unit, PVOID building) {
  PVOID player = driver_Unit_GetPlayer(unit);
  return driver_Method61E164(buffer, building, player);
}

PVOID driver_Method5FE863(PVOID buffer, PVOID unit) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FE863);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(unit, 
                buffer,
                (PVOID)0x0,
                (PVOID)0x0);
}

PVOID driver_Method5FDFA5(PVOID unit, PVOID command) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FDFA5);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(unit, 
                command);
}

void driver_Repair_PushCommandToUnit(PVOID buffer, PVOID unit) {
  driver_Method5FE863(buffer, unit);
  driver_Method5FDFA5(unit, (PVOID)0x1F51);
}

void driver_RepairBuilding(PVOID unit, PVOID building) {
  PVOID pntTarget = driver_New(0x5C);
  (void)driver_Repair_ClassInit(pntTarget, unit, building);
  (void)driver_Repair_PushCommandToUnit(pntTarget, unit);
}

void driver_Gather_Method621E95(PVOID self, PVOID targetResource) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x221E95);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
        targetResource,
        (PVOID)0x0,
        (PVOID)0x1);
}

void driver_Gather_Method5FE863(PVOID self, PVOID buffer) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FE863);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
         buffer,
         (PVOID)0x0,
         (PVOID)0x0);
}

void driver_Gather_Method5FDFA5(PVOID self) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x1FDFA5);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
        (PVOID)0xFB7);
}

PVOID driver_Gather_ClassInit(PVOID buffer, PVOID targetResource) {
  driver_Gather_Method621E95(buffer, targetResource);
}

void driver_Gather_Citizen_QueueCommand(PVOID unit, PVOID buffer) {
  driver_Gather_Method5FE863(unit, buffer);
  driver_Gather_Method5FDFA5(unit);
}

PVOID driver_Gather_UnitClassStruct(PVOID unit) {
  size_t *unitMetaData = (size_t *)util_Pointer(unit, 0x2C, POINTER_TYPE);
  return (PVOID)*unitMetaData;
}

PVOID driver_Gather_UnitClass(PVOID unit) {
  size_t *unitMetaData = (size_t *)util_Pointer(unit, 0x2C, POINTER_TYPE);
  return (PVOID)unitMetaData;
}

PVOID driver_Gather_UnitClassMethod(PVOID unit) {
  size_t rsp = *(size_t *)driver_Gather_UnitClass(unit);
  return (PVOID)*(size_t *)(rsp + 0x74);
}

void driver_Register(PVOID unit) {
  PVOID className = driver_Gather_UnitClass(unit);
  PVOID methodStruct = driver_Gather_UnitClassMethod(unit);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(className, unit);
}

void driver_Citizen_Gather(PVOID unit, PVOID resource) {
  PVOID buffer = driver_New(0x44);
  driver_Gather_ClassInit(buffer, resource);
  driver_Gather_Citizen_QueueCommand(unit, buffer);
  driver_Register(unit);
}

void driver_Command_Method627742(PVOID self, Point point, uint8_t move) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x227742);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID) = 
      (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  PVOID specialConst = (PVOID)(0x26080500 ^ move);
  method(self, 
         (PVOID)&point,
         specialConst,
         specialConst,
         specialConst,
         (PVOID)0x3F594D40,
         (PVOID)0x0,
         (PVOID)(0x1 ^ (!move)));
}

void driver_IssueCommand(PVOID unit, PVOID buffer, PVOID command) {
  driver_Gather_Method5FE863(unit, buffer);
  driver_Method5FDFA5(unit, command);
  driver_Register(unit);
}

void driver_Unit_Command(PVOID unit, Point position, UnitAction action) {
  PVOID buffer = driver_New(0x38);
  switch (action)
  {
    case UNIT_ATTACK:
      driver_Command_Method627742(buffer, position, 0);
      break;
    case UNIT_MOVE:
      driver_Command_Method627742(buffer, position, 1);
      break;
    
    default:
      break;
  }
  driver_IssueCommand(unit, buffer, (PVOID)0x1F40);
}

PVOID driver_Player_FromUnit(PVOID unit) {
  return util_Pointer((PVOID)unit, 0x18, POINTER_TYPE);
}

void driver_Command_Method6209C9(PVOID self, PVOID unit, TilePoint tile, AbilityTypes ability) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x2209C9);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID) = 
      (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
         unit,
         (PVOID)0x0,
         (PVOID)tile.x,
         (PVOID)tile.y,
         (PVOID)ability,
         (PVOID)0x1);
}

void driver_CastAbility_Remade(PVOID unit, Point target, AbilityTypes ability) {
  PVOID buffer = driver_New(0x34);
  TilePoint tile = geom_Tile_FromPoint(target);
  driver_Command_Method6209C9(buffer, unit, tile, ability);
  driver_IssueCommand(unit, buffer, (PVOID)0x1F40);
}

void driver_Command_Method61D337(PVOID self, PVOID unit, PVOID target) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x21D337);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID, PVOID) = 
      (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
         unit,
         target,
         (PVOID)0x1,
         (PVOID)0x0);
}

void driver_Convert_Remade(PVOID unit, PVOID target) {
  PVOID buffer = driver_New(0x44);
  driver_Command_Method61D337(buffer, unit, target);
  driver_IssueCommand(unit, buffer, (PVOID)0x7D4);
}

PVOID createArray(vector<PVOID> &units) {
  PVOID unitBuffer = driver_New(sizeof(PVOID) * units.size());
  for(size_t i = 0, c = units.size(); i < c; i++) {
    memcpy((PVOID)((size_t)unitBuffer + i * sizeof(PVOID)), &units[i], sizeof(PVOID));
  }
  return unitBuffer;
}

void driver_Command_Method627286(PVOID self, vector<PVOID> &units, PVOID transport) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x227286);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = 
      (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  size_t physicsBuffer = *(size_t *)((size_t)lib_BaseAddress() + 0x5318F0);
  PVOID variablePointer = (PVOID)((physicsBuffer & 0xFFFFFF00) | 0x2);
  PVOID unitBuffer = createArray(units);
  PVOID input[] = {variablePointer, unitBuffer, (PVOID)((size_t)unitBuffer + 0x4 * units.size())};
  method(self, 
         transport,
         &input,
         (PVOID)0x1);
}

void driver_Transport_Load(vector<PVOID> &units, PVOID transport) {
  size_t bufferSize = 0x68;
  PVOID buffer = driver_New(bufferSize);
  driver_Command_Method627286(buffer, units, transport);
  driver_IssueCommand(transport, buffer, (PVOID)0x1F40);
  for(size_t i = 0, c = units.size(); i < c; i++) {
    PVOID cpyBuffer = driver_New(bufferSize);
    driver_Command_Method627286(cpyBuffer, units, transport);
    driver_IssueCommand(units[i], cpyBuffer, (PVOID)0x1F40);
  }
}

PVOID driver_Transport_Ref(PVOID unit) {
  return (PVOID)(*(size_t *)((size_t)unit + 0x70));
}

size_t driver_Transport_UnitsCount(PVOID unit) {
  return (*(size_t *)((size_t)unit + 0x74) - *(size_t *)((size_t)unit + 0x70)) / 0x4;
}

PVOID driver_TechNode(TechTree tree, AbilityTypes ability) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x18A4);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(ply_TechTree_Ref(tree), 
         (PVOID)ability);
}

PVOID driver_AbilityPointer(PVOID manager, size_t abilityIndex) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0xAE71D);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(manager, 
         (PVOID)&abilityIndex);
}

int32_t driver_AbilityEnergy(PVOID reference) {
  PVOID energyMethod = (PVOID)*(size_t *)(*(size_t *)reference + 0x10);
  PVOID __thiscall (*method)(PVOID) = (PVOID __thiscall (*)(PVOID)) ((uint8_t *)energyMethod);
  return (int32_t)method(reference);
}

void driver_Command_Method6283FF(PVOID self, PVOID transport, TilePoint tile) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x2283FF);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID, PVOID) = 
      (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self,
         transport,
         (PVOID)tile.x,
         (PVOID)tile.y,
         (PVOID)0x1);
}

void driver_Transport_Unload(PVOID transport, TilePoint tile) {
  PVOID buffer = driver_New(0x40);
  driver_Command_Method6283FF(buffer, transport, tile);
  driver_IssueCommand(transport, buffer, (PVOID)0x1F40);
}

PVOID driver_Building_Ref() {
  return (PVOID)*(size_t *)((size_t)lib_BaseAddress() + 0x449520 + 0x30);
}

PVOID driver_Building_GetTemplate(size_t index) {
  return (PVOID)*(size_t *)(index * 0x4 + 0x5636AC + (size_t)lib_BaseAddress());
}

void driver_Building_629A56(PVOID unit) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x229A56);
  PVOID __thiscall (*method)(PVOID, PVOID) = 
      (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);

  method(unit, (PVOID)0x1);
}

PVOID driver_Building_CreateBuilding(PVOID buildingTemplate, TilePoint position) {
  PVOID methodBaseAddress = driver_Building_Ref();
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID) = 
      (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodBaseAddress);
  PVOID unit = method(
    buildingTemplate,
    (PVOID)position.x,
    (PVOID)position.y,
    (PVOID)0x1,
    (PVOID)0x1,
    (PVOID)0x0
  );
  driver_Building_629A56(unit);
  return unit;
}

void driver_Building_Create(PVOID citizen, TilePoint position, PVOID type) {
  PVOID building = driver_Building_CreateBuilding(type, position);
  if(!citizen || !building) {
    return ;
  }
  driver_RepairBuilding(citizen, building);
}

void driver_Command_Target_Method6209C9(PVOID self, PVOID unit, PVOID target, TilePoint tile, AbilityTypes ability) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x2209C9);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID) = 
      (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
         unit,
         target,
         (PVOID)tile.x,
         (PVOID)tile.y,
         (PVOID)ability,
         (PVOID)0x1);
}

void driver_CastAbility_Target(PVOID unit, PVOID target, Point targetPoint, AbilityTypes ability) {
  PVOID buffer = driver_New(0x34);
  TilePoint tile = geom_Tile_FromPoint(targetPoint);
  driver_Command_Target_Method6209C9(buffer, unit, target, tile, ability);
  driver_IssueCommand(unit, buffer, (PVOID)0x1F40);
}