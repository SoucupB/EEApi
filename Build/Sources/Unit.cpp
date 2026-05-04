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

int8_t unit_IsIdle(Unit unit) {
  return !util_Pointer(unit._payload, 0x1F0, POINTER_TYPE);
}

Point unit_Point_Position(Unit unit) {
  float *x = (float *)util_Pointer(unit_Reference(unit), 0x13C, FLOAT_TYPE);
  float *y = (float *)util_Pointer(unit_Reference(unit), 0x14C, FLOAT_TYPE);
  return (Point) {
    .x = *x,
    .y = *y
  };
}

int8_t unit_IsBuilding(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer((PVOID)unit._payload, 0x2C, POINTER_TYPE);
  size_t *callerStruct = (size_t *)util_Pointer((PVOID)unitMetaData[0], 0xB8, POINTER_TYPE);
  return (size_t)callerStruct == (size_t)lib_BaseAddress() + 0x20FD9D;
}

TilePoint unit_Tile_Position(Unit unit) {
  int32_t x = *(int32_t *)util_Pointer(unit_Reference(unit), 0x1C, INT32_T_TYPE);
  int32_t y = *(int32_t *)util_Pointer(unit_Reference(unit), 0x20, INT32_T_TYPE);
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

uint8_t unit_ProphetAbility_CanCast(Unit unit, Point target, Ability ability) {
  if(ability == PROPHET_TORNADO && !map_Tile_IsWater(geom_Tile_FromPoint(target))) {
    return 0;
  }
  return 1;
}

uint8_t unit_IsSpellValid(Unit unit, Point target, Ability ability) {
  if(eeTa_UnitType(unit) == PROPHET) {
    return unit_ProphetAbility_CanCast(unit, target, ability);
  }

  return 1;
}

// This method does not check whether it can cast things,
// Need to fix this.
void unit_CastAbility(Unit unit, Point target, Ability ability) {
  if(!unit_IsSpellValid(unit, target, ability)) {
    return ;
  }
  helper_CastAbility(unit_Reference(unit), target, ability);
}

UnitType unit_Type(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer((PVOID)unit._payload, 0x2C, POINTER_TYPE);
  return (UnitType)*(int32_t *)util_Pointer((PVOID)unitMetaData, 0x1E4, INT32_T_TYPE);
}

int32_t unit_CurrentHp(Unit unit) {
  return *(int32_t *)util_Pointer((PVOID)unit._payload, 0x3C, INT32_T_TYPE);
}

int32_t unit_TotalHP(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer((PVOID)unit._payload, 0x34, POINTER_TYPE);
  return *(int32_t *)util_Pointer((PVOID)unitMetaData, 0x144, INT32_T_TYPE);
}

uint8_t unit_GetPlayer(Unit unit) {
  PVOID nextStruct = util_Pointer(unit_Reference(unit), 0x18, POINTER_TYPE);
  return *(uint8_t *)util_Pointer((PVOID)nextStruct, 0x45C, INT8_T_TYPE);
}

void unit_Repair(Unit unit, Unit target) {
  UnitType unitType = unit_Type(unit);
  UnitType targetType = unit_Type(target);
  if(!eeTypes_IsCitizen(unitType) || !eeTypes_IsBuilding(targetType)) {
    return ;
  }
  if(unit_CurrentHp(target) >= unit_TotalHP(target)) {
    return ;
  }
  if(unit_GetPlayer(unit) != unit_GetPlayer(target)) {
    return ;
  }
  helper_RepairBuilding(unit_Reference(unit), unit_Reference(target));
}

void unit_Citizen_Farm(Unit unit, Resource resource) {
  UnitType unitType = unit_Type(unit);
  NeutralUnitType neutralRes = res_Type(resource);
  if(!eeTypes_IsCitizen(unitType) || neutralRes == RES_FISH) {
    return ;
  }
  helper_Citizen_Gather(unit_Reference(unit), res_Reference(resource));
}

void unit_Fishboat_Farm(Unit unit, Resource resource) {
  UnitType unitType = unit_Type(unit);
  NeutralUnitType neutralRes = res_Type(resource);
  if(eeTypes_IsFishBoat(unitType) && neutralRes != RES_FISH) {
    return ;
  }
  helper_Citizen_Gather(unit_Reference(unit), res_Reference(resource));
}

void unit_Farm(Unit unit, Resource resource) {
  if(!res_IsPresent(resource)) {
    return ;
  }
  unit_Citizen_Farm(unit, resource);
  unit_Fishboat_Farm(unit, resource);
}

void unit_Action(Unit unit, Point point, UnitAction action) {
  helper_Unit_Command(unit_Reference(unit), point, action);
}