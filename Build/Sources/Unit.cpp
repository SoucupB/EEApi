#include "LibManager.h"
#include "MapData.h"
#include <unordered_map>
#include "Unit.h"
#include "InjectUtilities.h"
#include "Resource.h"
#include "EETwa.h"
#include "Player.h"
#include "Game.h"

uint8_t unit_IsPresent(Unit unit);

vector<Unit> unit_GetBuildings(int8_t player) {
  vector<Unit> buildingsPointer;
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;

  for(auto &it : *(unitPresence[player])) {
    Unit currentUnit = (Unit) {
      ._payload = it.first
    };
    if(!unit_IsDead(currentUnit) && unit_IsBuilding(currentUnit)) {
      buildingsPointer.push_back(currentUnit);
    }
  }
  return buildingsPointer;
}

vector<Unit> unit_GetUnits(int8_t player) {
  vector<Unit> units;
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;
  for(auto &it : *(unitPresence[player])) {
    if(!unit_IsDead((Unit) {
      ._payload = it.first
    }) && !unit_IsBuilding((Unit) {
      ._payload = (PVOID)it.first
    })) {
      units.push_back((Unit) {
        ._payload = (PVOID)it.first
      });
    }
  }
  return units;
}

vector<Unit> unit_Filter(uint8_t (*method)(Unit)) {
  vector<Unit> units;
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;
  for(size_t i = 0; i < 20; i++) {
    for(auto &it : *(unitPresence[i])) {
      Unit unit = (Unit) {
        ._payload = it.first
      };
      if(!unit_IsDead(unit) && method(unit)) {
        units.push_back(unit);
      }
    }
  }
  return units;
}

int32_t unit_CurrentlyBuilding(Unit building) {
  return *(int32_t *)util_Pointer((PVOID)building._payload, 0x260, INT32_T_TYPE);
}

vector<Unit> unit_IdleBuildings(int8_t player) {
  vector<Unit> buildingsPointer;
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;
  for(auto &it : *(unitPresence[player])) {
    if(!unit_IsDead((Unit) {
      ._payload = it.first
    }) && unit_IsBuildingComplete((Unit) {
      ._payload = it.first
    }) && unit_IsBuilding((Unit) {
      ._payload = it.first
    }) && unit_Building_IsIdle((Unit) {
      ._payload = it.first
    })) {
      buildingsPointer.push_back((Unit) {
        ._payload = it.first
      });
    }
  }
  return buildingsPointer;
}

int8_t unit_Building_IsIdle(Unit building) {
  return eeTa_CurrentlyBuilding(building) == IDLE;
}

int8_t unit_IsBuildingComplete(Unit unit) {
  int8_t *isBuildingRef = (int8_t *)util_Pointer((PVOID)unit._payload, 0x34C, INT8_T_TYPE);
  
  return *isBuildingRef;
}

int8_t unit_IsIdle(Unit unit) {
  return !util_Pointer(unit_Reference(unit), 0x1F0, POINTER_TYPE);
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
  return eeTypes_IsBuilding(unit_Type(unit));
}

TilePoint unit_Tile_Position(Unit unit) {
  int32_t x = *(int32_t *)util_Pointer(unit_Reference(unit), 0x1C, INT32_T_TYPE);
  int32_t y = *(int32_t *)util_Pointer(unit_Reference(unit), 0x20, INT32_T_TYPE);
  return (TilePoint) {
    .x = x,
    .y = y
  };
}

uint8_t unit_CurrentEnergy(Unit unit) {
  uint8_t *energyPointer = (uint8_t *)util_Pointer(unit_Reference(unit), 0x2D4, INT8_T_TYPE);
  return *energyPointer;
}

Unit unit_Null() {
  return (Unit) {
    ._payload = NULL
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
  if(unit_Type(unit) == PROPHET) {
    return unit_ProphetAbility_CanCast(unit, target, ability);
  }
  return 1;
}

void unit_CastAbility(Unit unit, Point target, Ability ability) {
  if(!unit_IsSpellValid(unit, target, ability)) {
    return ;
  }
  helper_CastAbility_Remade(unit_Reference(unit), target, ability);
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

uint8_t unit_GetPlayerIndex(Unit unit) {
  // PVOID nextStruct = util_Pointer(unit_Reference(unit), 0x18, POINTER_TYPE);
  // return *(uint8_t *)util_Pointer((PVOID)nextStruct, 0x45C, INT8_T_TYPE);
  return ply_GetPlayerIndex(unit);
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
  if(unit_GetPlayerIndex(unit) != unit_GetPlayerIndex(target)) {
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

uint8_t unit_Prophet_CanCast(Unit unit, Ability ability) {
  switch (ability)
  {
    case PROPHET_EARTHQUAKE:
      return unit_CurrentEnergy(unit) >= 50;
    case PROPHET_MALARIA:
      return unit_CurrentEnergy(unit) >= 100;
    case PROPHET_TORNADO:
      return unit_CurrentEnergy(unit) >= 100;
    
    default:
      break;
  }
  return 0;
}

uint8_t unit_CanCast(Unit unit, Ability ability) {
  switch(unit_Type(unit)) {
    case PROPHET:
      return unit_Prophet_CanCast(unit, ability);

    default:
      break;
  }
  return 0;
}

uint8_t unit_IsPresent(Unit unit) {
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;
  for(size_t i = 0, c = eeTwa->playersCount; i < c; i++) {
    if(unitPresence[i]->find(unit_Reference(unit)) != unitPresence[i]->end()) {
      return 1;
    }
  }
  return 0;
}

int8_t unit_IsDead(Unit unit) {
  return !unit_CurrentHp(unit);
}

void unit_Convert(Unit src, Unit dst) {
  if(!eeTypes_IsPriest(unit_Type(src))) {
    return ;
  }
  if(unit_IsBuilding(dst) || unit_IsDead(dst)) {
    return ;
  }
  helper_Convert_Remade(unit_Reference(src), unit_Reference(dst));
}

void unit_Action(Unit unit, Point point, UnitAction action) {
  if(!unit_IsPresent(unit)) {
    return ;
  }
  helper_Unit_Command(unit_Reference(unit), point, action);
}

uint8_t unit_IsTransport(Unit unit) {
  return eeTypes_IsTransport(unit_Type(unit));
}

void unit_Load(Unit transport, Unit unit) {
  if(!unit_IsTransport(transport) || !eeTypes_IsGroundUnit(unit_Type(unit))) {
    return ;
  }
  helper_TransportLoad(unit_Reference(unit), unit_Reference(transport));
}

float unit_Range(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer((PVOID)unit._payload, 0x34, POINTER_TYPE);
  return *(float *)util_Pointer((PVOID)unitMetaData, 0x9C, FLOAT_TYPE);
}

float unit_Distance(Unit first, Unit dst) {
  return distanceEuclidf(unit_Point_Position(first), unit_Point_Position(dst));
}