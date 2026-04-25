#include "Unit.h"
#include "LibManager.h"
#include "MapData.h"
#include <unordered_map>

unordered_map<PVOID, uint8_t> *eeTa_GetUnitPresence();

vector<Unit> unit_GetBuildings(int8_t player) {
  vector<Unit> buildingsPointer;
  unordered_map<PVOID, uint8_t> *unitPresence = eeTa_GetUnitPresence();

  for(auto &it : unitPresence[player]) {
    Unit currentUnit = (Unit) {
      ._payload = it.first
    };
    if(!eeTa_IsUnitDead(currentUnit) && unit_IsBuilding(currentUnit)) {
      buildingsPointer.push_back(currentUnit);
    }
  }
  return buildingsPointer;
}

vector<Unit> unit_GetUnits(int8_t player) {
  vector<Unit> units;
  unordered_map<PVOID, uint8_t> *unitPresence = eeTa_GetUnitPresence();
  for(auto &it : unitPresence[player]) {
    if(!eeTa_IsUnitDead((Unit) {
      ._payload = it.first
    }) && eeTa_IsUnit((Unit) {
      ._payload = (PVOID)it.first
    })) {
      units.push_back((Unit) {
        ._payload = (PVOID)it.first
      });
    }
  }
  return units;
}

int32_t unit_CurrentlyBuilding(Unit building) {
  return *(int32_t *)util_Pointer((PVOID)building._payload, 0x260, INT32_T_TYPE);
}

vector<Unit> unit_IdleBuildings(int8_t player) {
  vector<Unit> buildingsPointer;
  unordered_map<PVOID, uint8_t> *unitPresence = eeTa_GetUnitPresence();
  for(auto &it : unitPresence[player]) {
    if(!eeTa_IsUnitDead((Unit) {
      ._payload = it.first
    }) && eeTa_IsBuildingComplete((Unit) {
      ._payload = it.first
    }) && unit_IsBuilding((Unit) {
      ._payload = it.first
    }) && eeTa_IsIdle((Unit) {
      ._payload = it.first
    })) {
      buildingsPointer.push_back((Unit) {
        ._payload = it.first
      });
    }
  }
  return buildingsPointer;
}

int8_t unit_IsBuilding(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer((PVOID)unit._payload, 0x2C, POINTER_TYPE);
  size_t *callerStruct = (size_t *)util_Pointer((PVOID)unitMetaData[0], 0xB8, POINTER_TYPE);
  return (size_t)callerStruct == (size_t)lib_BaseAddress() + 0x20FD9D;
}

TilePoint unit_Tile_Position(Unit unit) {
  int32_t x = *(int32_t *)util_Pointer(eeTa_Unit_Reference(unit), 0x1C, INT32_T_TYPE);
  int32_t y = *(int32_t *)util_Pointer(eeTa_Unit_Reference(unit), 0x20, INT32_T_TYPE);
  return (TilePoint) {
    .x = x,
    .y = y
  };
}

PVOID unit_Reference(Unit unit) {
  return unit._payload;
}

uint16_t unit_GetPlaneID(Unit unit) {
  return map_Tile_GetPlaneID(unit_Tile_Position(unit));
}

// This method does not check whether it can cast things,
// Need to fix this.
void unit_CastAbility(Unit unit, Point target, Ability ability) {
  helper_CastAbility(unit_Reference(unit), target, ability);
}