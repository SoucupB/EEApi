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
#include "Offset.h"

using namespace std;

PVOID __cdecl driver_New(size_t size) {
  PVOID __cdecl (*method)(size_t) = (PVOID __cdecl (*)(size_t)) ((uint8_t *)lib_BaseAddress() + DRIVER_REMOTE_METHOD_NEW);
  PVOID response = method(size);
  if(!response) {
    return NULL;
  }
  memset(response, 0, size);
  return response;
}

void __cdecl driver_Delete(PVOID pointer) {
  PVOID __cdecl (*method)(PVOID) = (PVOID __cdecl (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + DRIVER_REMOTE_METHOD_DELETE);
  method(pointer);
}

PVOID driver_Unit_GetPlayer(PVOID unit) {
  size_t *buffer = (size_t *)((size_t)unit + DRIVER_PLAYER_FROM_UNIT);
  return (PVOID)*buffer;
}

PVOID driver_Method61E164(PVOID buffer, PVOID building, PVOID player) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_REPAIR_001);
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
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_REPAIR_002);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(unit, 
                buffer,
                (PVOID)0x0,
                (PVOID)0x0);
}

PVOID driver_Method5FDFA5(PVOID unit, PVOID command) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_REPAIR_003);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(unit, 
                command);
}

void driver_Repair_PushCommandToUnit(PVOID buffer, PVOID unit) {
  driver_Method5FE863(buffer, unit);
  driver_Method5FDFA5(unit, (PVOID)DRIVER_REPAIR_SPECIAL_CONST);
}

void driver_RepairBuilding(PVOID unit, PVOID building) {
  driver_StopCommand(unit);
  PVOID pntTarget = driver_New(0x5C);
  (void)driver_Repair_ClassInit(pntTarget, unit, building);
  (void)driver_Repair_PushCommandToUnit(pntTarget, unit);
}

void driver_Gather_Method621E95(PVOID self, PVOID targetResource) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_GATHER_001);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
        targetResource,
        (PVOID)0x0,
        (PVOID)0x1);
}

void driver_Gather_Method5FE863(PVOID self, PVOID buffer) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_GATHER_002);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
         buffer,
         (PVOID)0x0,
         (PVOID)0x0);
}

void driver_Gather_Method5FDFA5(PVOID self) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_GATHER_003);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
        (PVOID)DRIVER_GATHER_SPECIAL_CONST);
}

void driver_Gather_ClassInit(PVOID buffer, PVOID targetResource) {
  driver_Gather_Method621E95(buffer, targetResource);
}

void driver_Gather_Citizen_QueueCommand(PVOID unit, PVOID buffer) {
  driver_Gather_Method5FE863(unit, buffer);
  driver_Gather_Method5FDFA5(unit);
}

PVOID driver_Gather_UnitClassStruct(PVOID unit) {
  size_t *unitMetaData = (size_t *)util_Pointer(unit, DRIVER_UNIT_CLASS, POINTER_TYPE);
  return (PVOID)*unitMetaData;
}

PVOID driver_Gather_UnitClass(PVOID unit) {
  size_t *unitMetaData = (size_t *)util_Pointer(unit, DRIVER_UNIT_CLASS, POINTER_TYPE);
  return (PVOID)unitMetaData;
}

PVOID driver_Gather_UnitClassMethod(PVOID unit) {
  size_t rsp = *(size_t *)driver_Gather_UnitClass(unit);
  return (PVOID)*(size_t *)(rsp + DRIVER_METHOD_INSTANCE_UNIT_CLASS);
}

void driver_Register(PVOID unit) {
  PVOID className = driver_Gather_UnitClass(unit);
  PVOID methodStruct = driver_Gather_UnitClassMethod(unit);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(className, unit);
}

void driver_Citizen_Gather(PVOID unit, PVOID resource) {
  driver_StopCommand(unit);
  PVOID buffer = driver_New(0x44);
  driver_Gather_ClassInit(buffer, resource);
  driver_Gather_Citizen_QueueCommand(unit, buffer);
  driver_Register(unit);
}

void driver_Command_Method627742(PVOID self, Point point, uint8_t move) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_COMMAND);
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
  driver_StopCommand(unit);
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
  driver_IssueCommand(unit, buffer, (PVOID)DRIVER_DEFAULT_SPECIAL_CONST);
}

PVOID driver_Player_FromUnit(PVOID unit) {
  return util_Pointer((PVOID)unit, DRIVER_PLAYER_FROM_UNIT, POINTER_TYPE);
}

void driver_Command_Method6209C9(PVOID self, PVOID unit, TilePoint tile, AbilityTypes ability) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_CAST_ABILITY_001);
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
  driver_StopCommand(unit);
  PVOID buffer = driver_New(0x34);
  TilePoint tile = geom_Tile_FromPoint(target);
  driver_Command_Method6209C9(buffer, unit, tile, ability);
  driver_IssueCommand(unit, buffer, (PVOID)DRIVER_DEFAULT_SPECIAL_CONST);
}

void driver_Command_Method61D337(PVOID self, PVOID unit, PVOID target) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_CONVERT_001);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID, PVOID) = 
      (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self, 
         unit,
         target,
         (PVOID)0x1,
         (PVOID)0x0);
}

void driver_Convert_Remade(PVOID unit, PVOID target) {
  driver_StopCommand(unit);
  PVOID buffer = driver_New(0x44);
  driver_Command_Method61D337(buffer, unit, target);
  driver_IssueCommand(unit, buffer, (PVOID)DRIVER_CONVERT_SPECIAL_CONST);
}

PVOID createArray(vector<PVOID> &units) {
  PVOID unitBuffer = driver_New(sizeof(PVOID) * units.size());
  for(size_t i = 0, c = units.size(); i < c; i++) {
    memcpy((PVOID)((size_t)unitBuffer + i * sizeof(PVOID)), &units[i], sizeof(PVOID));
  }
  return unitBuffer;
}

void driver_Command_Method627286(PVOID self, vector<PVOID> &units, PVOID transport) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_LOAD_001);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = 
      (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  size_t physicsBuffer = *(size_t *)((size_t)lib_BaseAddress() + DRIVER_PHYSICS_BUFFER_OFFSET);
  PVOID variablePointer = (PVOID)((physicsBuffer & 0xFFFFFF00) | 0x2);
  PVOID unitBuffer = createArray(units);
  PVOID input[] = {variablePointer, unitBuffer, (PVOID)((size_t)unitBuffer + sizeof(PVOID) * units.size())};
  method(self, 
         transport,
         &input,
         (PVOID)0x1);
}

void driver_Transport_Load(vector<PVOID> &units, PVOID transport) {
  driver_StopCommand(transport);
  size_t bufferSize = 0x68;
  PVOID buffer = driver_New(bufferSize);
  driver_Command_Method627286(buffer, units, transport);
  driver_IssueCommand(transport, buffer, (PVOID)DRIVER_DEFAULT_SPECIAL_CONST);
  for(size_t i = 0, c = units.size(); i < c; i++) {
    PVOID cpyBuffer = driver_New(bufferSize);
    driver_Command_Method627286(cpyBuffer, units, transport);
    driver_IssueCommand(units[i], cpyBuffer, (PVOID)DRIVER_DEFAULT_SPECIAL_CONST);
  }
}

PVOID driver_Transport_Ref(PVOID unit) {
  return (PVOID)(*(size_t *)((size_t)unit + DRIVER_TRANSPORT_UNITS_OFFSET_START));
}

size_t driver_Transport_UnitsCount(PVOID unit) {
  return (*(size_t *)((size_t)unit + DRIVER_TRANSPORT_UNITS_OFFSET_END) - *(size_t *)((size_t)unit + DRIVER_TRANSPORT_UNITS_OFFSET_START)) / sizeof(PVOID);
}

PVOID driver_TechNode(TechTree tree, AbilityTypes ability) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_TECH_NODE);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(ply_TechTree_Ref(tree), 
         (PVOID)ability);
}

PVOID driver_AbilityPointer(PVOID manager, size_t abilityIndex) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_ABILITY_POINTER);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(manager, 
         (PVOID)&abilityIndex);
}

int32_t driver_AbilityEnergy(PVOID reference) {
  PVOID energyMethod = (PVOID)*(size_t *)(*(size_t *)reference + DRIVER_INSTANCE_ABILITY_COST);
  PVOID __thiscall (*method)(PVOID) = (PVOID __thiscall (*)(PVOID)) ((uint8_t *)energyMethod);
  return (int32_t)method(reference);
}

void driver_Command_Method6283FF(PVOID self, PVOID transport, TilePoint tile) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_UNLOAD);
  PVOID __thiscall (*method)(PVOID, PVOID, PVOID, PVOID, PVOID) = 
      (PVOID __thiscall (*)(PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(self,
         transport,
         (PVOID)tile.x,
         (PVOID)tile.y,
         (PVOID)0x1);
}

void driver_Transport_Unload(PVOID transport, TilePoint tile) {
  driver_StopCommand(transport);
  PVOID buffer = driver_New(0x40);
  driver_Command_Method6283FF(buffer, transport, tile);
  driver_IssueCommand(transport, buffer, (PVOID)DRIVER_DEFAULT_SPECIAL_CONST);
}

PVOID driver_Building_Ref() {
  return (PVOID)*(size_t *)((size_t)lib_BaseAddress() + DRIVER_BUILDING_REF);
}

PVOID driver_Building_GetTemplate(size_t index) {
  return (PVOID)*(size_t *)(index * sizeof(PVOID) + DRIVER_BUILDING_TEMPLATE + (size_t)lib_BaseAddress());
}

void driver_Building_629A56(PVOID unit) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_BUILDING_001);
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
  driver_StopCommand(citizen);
  PVOID building = driver_Building_CreateBuilding(type, position);
  if(!citizen || !building) {
    return ;
  }
  driver_RepairBuilding(citizen, building);
}

void driver_Command_Target_Method6209C9(PVOID self, PVOID unit, PVOID target, TilePoint tile, AbilityTypes ability) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_CAST_UNIT_ABILITY);
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
  driver_StopCommand(unit);
  PVOID buffer = driver_New(0x34);
  TilePoint tile = geom_Tile_FromPoint(targetPoint);
  driver_Command_Target_Method6209C9(buffer, unit, target, tile, ability);
  driver_IssueCommand(unit, buffer, (PVOID)DRIVER_DEFAULT_SPECIAL_CONST);
}

PVOID driver_CanBuildAt_GetSelectorRef(PVOID player) {
  PVOID selectedGroupRef = (PVOID)*(size_t *)((size_t)player + DRIVER_SELECTED_GROUP_OFFSET);
  return (PVOID)*(size_t *)((size_t)selectedGroupRef + DRIVER_SELECTED_GROUP_OFFSET_LIST_START);
}

void driver_CanBuildAt_SetSelectorRef(PVOID player, PVOID selectedGroup) {
  PVOID selectedGroupRef = (PVOID)*(size_t *)((size_t)player + DRIVER_SELECTED_GROUP_OFFSET);
  *(size_t *)((size_t)selectedGroupRef + DRIVER_SELECTED_GROUP_OFFSET_LIST_START) = (size_t)selectedGroup;
}

size_t driver_CanBuiltAt_64F264(PVOID player, TilePoint tile, size_t buildingTypeID) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_CAN_BUILD);
  size_t __cdecl (*method)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID) = 
       (size_t __cdecl (*)(PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  uint8_t fixStack = 0x0;
  PVOID selectorRef = driver_CanBuildAt_GetSelectorRef(player);
  driver_CanBuildAt_SetSelectorRef(player, (PVOID)0x0);
  size_t canBuild = method(player,
         (PVOID)0x0,
         (PVOID)buildingTypeID,
         (PVOID)tile.x,
         (PVOID)tile.y,
         (PVOID)0x0,
         (PVOID)0x0,
         (PVOID)&fixStack);
  driver_CanBuildAt_SetSelectorRef(player, selectorRef);
  return canBuild;
}

size_t driver_CanBuiltAt(PVOID player, TilePoint tile, size_t buildingTypeID) {
  return driver_CanBuiltAt_64F264(player, tile, buildingTypeID);
}

size_t driver_CanBuiltAt_603DD6(PVOID player, TilePoint tile, size_t buildingTypeID) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_CAN_BUILD_SHADOW_UNIT);
  size_t __thiscall (*method)(PVOID) = 
       (size_t __thiscall (*)(PVOID)) ((uint8_t *)methodStruct);
  PVOID shadowUnit = 0x0;
  size_t canBuild = method(shadowUnit);
  return canBuild;
}

void driver_InstantiateClass_6042C7(PVOID buffer, PVOID player, size_t buildingTypeID) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_CAN_BUILD_SHADOW_UNIT_INSTANTIATE);
  void __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = 
  (void __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  method(buffer, (PVOID)buildingTypeID, player, (PVOID)0x0);
}

void driver_FillTileData(PVOID buffer, TilePoint tile) {
  float xPos = (float)tile.x;
  float yPos = (float)tile.y;

  *(size_t *)((size_t)buffer + DRIVER_BUILDABLE_POSITION_INT_32_POS_X) = tile.x;
  *(size_t *)((size_t)buffer + DRIVER_BUILDABLE_POSITION_INT_32_POS_Y) = tile.y;
  *(float *)((size_t)buffer + DRIVER_BUILDABLE_POSITION_FLOAT_POS_X) = xPos;
  *(float *)((size_t)buffer + DRIVER_BUILDABLE_POSITION_FLOAT_POS_Y) = yPos;
}

typedef struct SelectedGroupArray_t {
  PVOID _begin;
  PVOID _end;
} SelectedGroupArray;

SelectedGroupArray driver_SelectedGroup_GetArray(PVOID player) {
  SelectedGroupArray response = {0};
  PVOID selectedGroupRef = (PVOID)*(size_t *)((size_t)player + DRIVER_SELECTED_GROUP_OFFSET);
  response._begin = (PVOID)*(size_t *)((size_t)selectedGroupRef + DRIVER_SELECTED_GROUP_OFFSET_LIST_START);
  response._end = (PVOID)*(size_t *)((size_t)selectedGroupRef + DRIVER_SELECTED_GROUP_OFFSET_LIST_END);
  return response;
}

void driver_SelectedGroup_SetArray(SelectedGroupArray *arr, PVOID player) {
  PVOID selectedGroupRef = (PVOID)*(size_t *)((size_t)player + DRIVER_SELECTED_GROUP_OFFSET);
  *(size_t *)((size_t)selectedGroupRef + DRIVER_SELECTED_GROUP_OFFSET_LIST_START) = (size_t)arr->_begin;
  *(size_t *)((size_t)selectedGroupRef + DRIVER_SELECTED_GROUP_OFFSET_LIST_END) = (size_t)arr->_end;
}

void driver_SelectedGroup_SetUnit(PVOID player, PVOID unit) {
  PVOID selectedGroupRef = (PVOID)*(size_t *)((size_t)player + DRIVER_SELECTED_GROUP_OFFSET);
  *(size_t *)((size_t)selectedGroupRef + DRIVER_SELECTED_GROUP_OFFSET_LIST_START) = (size_t)&unit;
  *(size_t *)((size_t)selectedGroupRef + DRIVER_SELECTED_GROUP_OFFSET_LIST_END) = (size_t)&unit + sizeof(PVOID);
}

size_t driver_CanBuildHere_603DD6(PVOID buffer, PVOID player, PVOID unit) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_CAN_BUILD_HERE);
  size_t __thiscall (*method)(PVOID) = 
  (size_t __thiscall (*)(PVOID)) ((uint8_t *)methodStruct);
  SelectedGroupArray currentArr = driver_SelectedGroup_GetArray(player);
  driver_SelectedGroup_SetUnit(player, unit);
  size_t canBuild = method(buffer);
  driver_SelectedGroup_SetArray(&currentArr, player);
  return canBuild;
}

PVOID driver_BuildStruct(PVOID player, size_t buildingTypeID) {
  PVOID buffer = driver_New(0x334);
  driver_InstantiateClass_6042C7(buffer, player, buildingTypeID);
  return buffer;
}

size_t driver_CanBuild_WO_Buffer(PVOID player, PVOID buffer, PVOID citizen, TilePoint tile, size_t buildingTypeID) {
  driver_FillTileData(buffer, tile);
  return driver_CanBuildHere_603DD6(buffer, player, citizen);
}

size_t driver_CanBuiltAt_Complete(PVOID player, PVOID citizen, TilePoint tile, size_t buildingTypeID) {
  PVOID buffer = driver_BuildStruct(player, buildingTypeID);
  driver_FillTileData(buffer, tile);
  const size_t canBuildHere = driver_CanBuildHere_603DD6(buffer, player, citizen);
  driver_Delete(buffer);
  return canBuildHere;
}

void driver_StopCommand(PVOID unit) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_STOP);
  size_t __thiscall (*method)(PVOID) = 
  (size_t __thiscall (*)(PVOID)) ((uint8_t *)methodStruct);
  (void)!method(unit);
}

void driver_AttackUnit_Instantiate_61D337(PVOID buffer, PVOID attackerUnit, PVOID attackedUnit) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + DRIVER_REMOTE_METHOD_ATTACK_TARGET_INSTANTIATE);
  void __thiscall (*method)(PVOID, PVOID, PVOID, PVOID, PVOID) = 
  (void __thiscall (*)(PVOID, PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)methodStruct);
  const size_t bufferFlag = (((size_t)buffer & 0xFFFFFF00) | 0x1);
  method(buffer, attackerUnit, attackedUnit, (PVOID)bufferFlag, (PVOID)0x0);
}

void driver_AttackUnit(PVOID attackerUnit, PVOID attackedUnit) {
  driver_StopCommand(attackerUnit);
  PVOID buffer = driver_New(0x44);
  driver_AttackUnit_Instantiate_61D337(buffer, attackerUnit, attackedUnit);
  driver_IssueCommand(attackerUnit, buffer, (PVOID)DRIVER_ATTACK_TARGET_SPECIAL_CONST);
}

PVOID driver_WideStringMethod() {
  return (PVOID)*(size_t *)((size_t)lib_BaseAddress() + 0x4372C0);
}

void driver_CreateWideString(PVOID buffer, char *input) {
  PVOID methodRef = driver_WideStringMethod();
  size_t __thiscall (*method)(PVOID, PVOID) = (size_t __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodRef);
  method(buffer, input);
}

void driver_MessageConstrucor(PVOID eeMessage, PVOID wideString) {
  void __thiscall (*method)(PVOID, PVOID) = (void __thiscall (*)(PVOID, PVOID)) ((uint8_t *)lib_BaseAddress() + 0x1D3196);
  method(eeMessage, wideString);
}

PVOID driver_Server_Get() {
  return (PVOID)*(size_t *)((size_t)lib_BaseAddress() + 0x5319C8);
}

PVOID driver_Server_MessageMethodRef() {
  PVOID server = driver_Server_Get();
  return (PVOID)*(size_t *)(*(size_t *)server + 0x18);
}

void driver_Server_PushMessage(PVOID eeMessage) {
  PVOID server = driver_Server_Get();
  PVOID messageRef = driver_Server_MessageMethodRef();
  void __thiscall (*method)(PVOID, PVOID, PVOID) = (void __thiscall (*)(PVOID, PVOID, PVOID)) ((uint8_t *)messageRef);
  method(server, eeMessage, (PVOID)0xFF);
}

__declspec(dllexport) void driver_Write(char *input) {
  uint32_t buffer[24];
  driver_CreateWideString((PVOID)buffer, input);
  PVOID eeMessage = driver_New(0x24);
  driver_MessageConstrucor(eeMessage, buffer);
  driver_Server_PushMessage(eeMessage);
}