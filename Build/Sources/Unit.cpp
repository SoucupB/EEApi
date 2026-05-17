#include "LibManager.h"
#include "MapData.h"
#include <unordered_map>
#include "Unit.h"
#include "InjectUtilities.h"
#include "Resource.h"
#include "EETwa.h"
#include "Player.h"
#include "Game.h"
#include "Ability.h"
#include "EETypesStructPrivate.h"
#include "Offset.h"

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

vector<Unit> unit_Player_GetBuildings(Player ply) {
  vector<Unit> buildingsPointer;
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;
  const uint8_t playerIndex = ply_PlayerIndex(ply);
  for(auto &it : *(unitPresence[playerIndex])) {
    Unit currentUnit = (Unit) {
      ._payload = it.first
    };
    if(!unit_IsDead(currentUnit) && unit_IsBuilding(currentUnit)) {
      buildingsPointer.push_back(currentUnit);
    }
  }
  return buildingsPointer;
}

vector<Unit> unit_Player_GetUnits(Player ply) {
  vector<Unit> units;
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;
  const uint8_t playerIndex = ply_PlayerIndex(ply);
  for(auto &it : *(unitPresence[playerIndex])) {
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

int32_t unit_BuildablesCount(Unit unit) {
  size_t *typeMetaPointer = (size_t *)util_Pointer(unit_Reference(unit), UNIT_CLASS, POINTER_TYPE);
  int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + UNIT_METHOD_BUILDABLE_COUNT);
  return method(typeMetaPointer);
}

PVOID unit_EpochStruct(PVOID building, PVOID unitType) {
  size_t *buildingMetaData = (size_t *)util_Pointer((PVOID)building, UNIT_PLAYER_OFFSET, POINTER_TYPE);
  size_t *epochStruct = (size_t *)util_Pointer((PVOID)buildingMetaData, UNIT_EPOCH_STRUCT, POINTER_TYPE);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)lib_BaseAddress() + UNIT_METHOD_EPOCH_STRUCT);

  return method((PVOID)epochStruct, unitType);
}

uint8_t unit_CanBuild(Unit building, UnitType type) {
  size_t *epochStruct = (size_t *)unit_EpochStruct(unit_Reference(building), (PVOID)type);
  if(!epochStruct) {
    return 0;
  }

  size_t *checkMethod = (size_t *)util_Pointer((PVOID)(epochStruct[0]), UNIT_METHOD_INSTANCE_OFFSET_EPOCH_STRUCT, POINTER_TYPE);
  int8_t __thiscall (*method)(PVOID) = (int8_t __thiscall (*)(PVOID)) ((uint8_t *)checkMethod);

  return method((PVOID)epochStruct);
}

vector<UnitType> unit_AllBuildableTypes(Unit unit) {
  size_t *typeMetaPointer = (size_t *)util_Pointer(unit_Reference(unit), UNIT_CLASS, POINTER_TYPE);
  size_t *buildableTypes = (size_t *)util_Pointer((PVOID)typeMetaPointer, UNIT_BUILDABLE_OBJECTS, POINTER_TYPE);
  vector<UnitType> types;
  if(!buildableTypes) {
    return types;
  }

  int32_t totalBuildables = unit_BuildablesCount(unit);
  for(int32_t i = 0; i < totalBuildables; i++) {
    if(unit_CanBuild(unit, (UnitType)buildableTypes[i])) {
      types.push_back((UnitType)buildableTypes[i]);
    }
  }

  return types;
}

char *unit_Name(Unit unit) {
  const PVOID ref = unit_Reference(unit);
  return (char *)((size_t *)(*(size_t *)ref + UNIT_CLASS) + UNIT_CLASS_NAME);
}

void unit_Build(Unit building, UnitType type) {
  Player currentPlayer = ply_GetPlayer(building);
  if(ply_CurrentPopulation(currentPlayer) >= ply_TotalPop(currentPlayer)) {
    return ;
  }
  int32_t __thiscall (*method)(PVOID, PVOID, PVOID) = (int32_t __thiscall (*)(PVOID, PVOID, PVOID)) ((uint8_t *)lib_BaseAddress() + UNIT_METHOD_BUILD);
  PEETwa eeTwa = game_EETwa();
  eeTwa->shouldCostBeReduced = 1;
  method(building._payload, (PVOID)type, 0);
  eeTwa->shouldCostBeReduced = 0;
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

vector<Unit> unit_FilterWithBuffer(uint8_t (*method)(Unit, PVOID), PVOID buffer) {
  vector<Unit> units;
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **unitPresence = eeTwa->unitPresence;
  for(size_t i = 0; i < 20; i++) {
    for(auto &it : *(unitPresence[i])) {
      Unit unit = (Unit) {
        ._payload = it.first
      };
      if(!unit_IsDead(unit) && method(unit, buffer)) {
        units.push_back(unit);
      }
    }
  }
  return units;
}

int32_t unit_CurrentlyBuilding(Unit building) {
  return *(int32_t *)util_Pointer((PVOID)building._payload, UNIT_CURRENTLY_BUILDING, INT32_T_TYPE);
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
  return unit_CurrentlyBuilding(building) == IDLE;
}

int8_t unit_IsBuildingComplete(Unit unit) {
  int8_t *isBuildingRef = (int8_t *)util_Pointer(unit_Reference(unit), UNIT_IS_BUILDING_COMPLETE, INT8_T_TYPE);
  
  return *isBuildingRef;
}

vector<Unit> unit_FilterFromArray(vector<Unit> &units, uint8_t (*method)(Unit)) {
  vector<Unit> filteredUnits;
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(method(units[i])) {
      filteredUnits.push_back(units[i]);
    }
  }
  return filteredUnits;
}

uint8_t unit_IsNeutral(Unit unit) {
  PEETwa eeTwa = game_EETwa();
  return eeTa_Player(unit) == eeTwa->neutralPlayer;
}

int8_t unit_IsIdle(Unit unit) {
  return !util_Pointer(unit_Reference(unit), UNIT_ACTION_POINTER_INSTANCE, POINTER_TYPE);
}

Point unit_Point_Position(Unit unit) {
  float *x = (float *)util_Pointer(unit_Reference(unit), UNIT_POINT_X, FLOAT_TYPE);
  float *y = (float *)util_Pointer(unit_Reference(unit), UNIT_POINT_Y, FLOAT_TYPE);
  return (Point) {
    .x = *x,
    .y = *y
  };
}

int8_t unit_IsBuilding(Unit unit) {
  return eeTypes_IsBuilding(unit_Type(unit));
}

TilePoint unit_Tile_Position(Unit unit) {
  int32_t x = *(int32_t *)util_Pointer(unit_Reference(unit), UNIT_TILE_X, INT32_T_TYPE);
  int32_t y = *(int32_t *)util_Pointer(unit_Reference(unit), UNIT_TILE_Y, INT32_T_TYPE);
  return (TilePoint) {
    .x = x,
    .y = y
  };
}

uint8_t unit_CurrentEnergy(Unit unit) {
  uint8_t *energyPointer = (uint8_t *)util_Pointer(unit_Reference(unit), UNIT_ENERGY, INT8_T_TYPE);
  return *energyPointer;
}

Unit unit_Null() {
  return (Unit) {
    ._payload = NULL
  };
}

vector<AbilityTypes> unit_Abilities(Unit unit) {
  return eeTypes_Abilities(unit_Type(unit));
}

PVOID unit_Reference(Unit unit) {
  return unit._payload;
}

uint16_t unit_GetPlaneID(Unit unit) {
  return map_Tile_GetPlaneID(unit_Tile_Position(unit));
}

void unit_Point_CastAbility(Unit unit, Point target, AbilityTypes ability) {
  if(!unit_CanCast(unit, ability)) {
    return ;
  }
  driver_CastAbility_Remade(unit_Reference(unit), target, ability);
}

void unit_Object_CastAbility(Unit unit, Unit target, AbilityTypes ability) {
  if(!unit_CanCast(unit, ability)) {
    return ;
  }
  driver_CastAbility_Target(unit_Reference(unit), unit_Reference(target), unit_Point_Position(target), ability);
}

UnitType unit_Type(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer(unit_Reference(unit), UNIT_CLASS, POINTER_TYPE);
  return (UnitType)*(int32_t *)util_Pointer((PVOID)unitMetaData, UNIT_TYPE, INT32_T_TYPE);
}

int32_t unit_CurrentHp(Unit unit) {
  return *(int32_t *)util_Pointer(unit_Reference(unit), UNIT_CURRENT_HP, INT32_T_TYPE);
}

int32_t unit_TotalHP(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer(unit_Reference(unit), UNIT_CIV_ATTRIBUTE, POINTER_TYPE);
  return *(int32_t *)util_Pointer((PVOID)unitMetaData, UNIT_CIV_ATTRIBUTE_TOTAL_HP, INT32_T_TYPE);
}

float unit_TotalDamage(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer(unit_Reference(unit), UNIT_CIV_ATTRIBUTE, POINTER_TYPE);
  return *(float *)util_Pointer((PVOID)unitMetaData, UNIT_CIV_ATTRIBUTE_TOTAL_DAMAGE, FLOAT_TYPE);
}

// This method is not good yet.
float unit_TotalSpeed(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer(unit_Reference(unit), UNIT_CIV_ATTRIBUTE, POINTER_TYPE);
  return *(float *)util_Pointer((PVOID)unitMetaData, UNIT_CIV_ATTRIBUTE_TOTAL_SPEED, FLOAT_TYPE);
}

uint8_t unit_GetPlayerIndex(Unit unit) {
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
  driver_RepairBuilding(unit_Reference(unit), unit_Reference(target));
}

void unit_Citizen_Farm(Unit unit, Resource resource) {
  UnitType unitType = unit_Type(unit);
  NeutralUnitType neutralRes = res_Type(resource);
  if(!eeTypes_IsCitizen(unitType) || neutralRes == RES_FISH) {
    return ;
  }
  driver_Citizen_Gather(unit_Reference(unit), res_Reference(resource));
}

void unit_Fishboat_Farm(Unit unit, Resource resource) {
  UnitType unitType = unit_Type(unit);
  NeutralUnitType neutralRes = res_Type(resource);
  if(eeTypes_IsFishBoat(unitType) && neutralRes != RES_FISH) {
    return ;
  }
  driver_Citizen_Gather(unit_Reference(unit), res_Reference(resource));
}

void unit_Farm(Unit unit, Resource resource) {
  if(!res_IsPresent(resource)) {
    return ;
  }
  unit_Citizen_Farm(unit, resource);
  unit_Fishboat_Farm(unit, resource);
}

uint8_t unit_CanCast(Unit unit, AbilityTypes ability) {
  uint8_t found = 0;
  vector<AbilityTypes> abilityTypes = unit_Abilities(unit);
  for(size_t i = 0, c = abilityTypes.size(); i < c; i++) {
    if(abilityTypes[i] == ability) {
      found = 1;
      break;
    }
  }
  if(!found) {
    return 0;
  }
  return ability_Energy(ability) <= unit_CurrentEnergy(unit);
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
  driver_Convert_Remade(unit_Reference(src), unit_Reference(dst));
}

void unit_Action(Unit unit, Point point, UnitAction action) {
  if(!unit_IsPresent(unit)) {
    return ;
  }
  driver_Unit_Command(unit_Reference(unit), point, action);
}

uint8_t unit_IsTransport(Unit unit) {
  return eeTypes_IsTransport(unit_Type(unit));
}

void unit_Transport_Load(Unit transport, vector<Unit> &units) {
  if(!unit_IsTransport(transport) || !units.size()) {
    return ;
  }
  vector<PVOID> bufferUnits;
  for(size_t i = 0, c = units.size(); i < c; i++) {
    bufferUnits.push_back(unit_Reference(units[i]));
  }
  driver_Transport_Load(bufferUnits, unit_Reference(transport));
}

vector<Unit> unit_Transport_UnitsInside(Unit transport) {
  vector<Unit> response;
  if(!unit_IsTransport(transport)) {
    return response;
  }
  size_t unitsCount = driver_Transport_UnitsCount(unit_Reference(transport));
  if(!unitsCount) {
    return response;
  }
  PVOID unitRef = driver_Transport_Ref(unit_Reference(transport));
  for(size_t i = 0; i < unitsCount; i++) {
    response.push_back((Unit) {
      ._payload = (PVOID)*(size_t *)((size_t)unitRef + (i * 0x4))
    });
  }
  return response;
}

size_t unit_Transport_Population(Unit transport) {
  if(!unit_IsTransport(transport)) {
    return 0;
  }
  size_t unitsCount = driver_Transport_UnitsCount(unit_Reference(transport));
  if(!unitsCount) {
    return 0;
  }
  size_t totalPop = 0;
  PVOID unitRef = driver_Transport_Ref(unit_Reference(transport));
  for(size_t i = 0; i < unitsCount; i++) {
    totalPop += unit_Population((Unit) {
      ._payload = (PVOID)*(size_t *)((size_t)unitRef + (i * 0x4))
    });
  }
  return totalPop;
}

void unit_Transport_Unload(Unit transport, TilePoint tile) {
  if(!unit_IsTransport(transport)) {
    return ;
  }
  driver_Transport_Unload(unit_Reference(transport), tile);
}

float unit_Range(Unit unit) {
  size_t *unitMetaData = (size_t *)util_Pointer(unit_Reference(unit), UNIT_CIV_ATTRIBUTE, POINTER_TYPE);
  return *(float *)util_Pointer((PVOID)unitMetaData, UNIT_CIV_ATTRIBUTE_TOTAL_RANGE, FLOAT_TYPE);
}

float unit_Distance(Unit first, Unit dst) {
  return distanceEuclidf(unit_Point_Position(first), unit_Point_Position(dst));
}

void unit_Building_Build(Unit citizen, TilePoint tile, UnitType unitType) {
  if(!eeTypes_IsCitizen(unit_Type(citizen)) || !eeTypes_IsBuilding(unitType)) {
    return ;
  }
  PVOID unitClass = eeTypes_GetTemplate(unitType);
  if(!unitClass) {
    return ;
  }
  driver_Building_Create(unit_Reference(citizen), tile, unitClass);
}

int32_t unit_Population(Unit unit) {
  PVOID unitTypeStruct = util_Pointer(unit_Reference(unit), UNIT_CLASS, POINTER_TYPE);
  PVOID callerMethods = util_Pointer(unitTypeStruct, 0x0, POINTER_TYPE);
  PVOID callee = util_Pointer(callerMethods, UNIT_METHOD_INSTANCE_OFFSET_POPULATION, POINTER_TYPE);
  int32_t __fastcall (*method)(PVOID) = (int32_t __fastcall (*)(PVOID)) ((uint8_t *)callee);

  return method(unitTypeStruct);
}