#include "AttackDecision.h"
#include "Geometry.h"
#include "EETwa.h"
#include "EETypes.h"
#include "PlayerState.h"
#include <math.h>
#include <map>
#include "Unit.h"
#include "Player.h"

static map<pair<float, float>, uint8_t> attackedUnits;

uint8_t idleAttackingWaterUnits(Unit unit);

size_t min(size_t a, size_t b) {
  return a < b ? a : b;
}

uint8_t navalAttackFilter(Unit unit) {
  UnitType def = unit_Type(unit);
  return idleAttackingWaterUnits(unit) && eeTa_Player(unit) == eeTa_SelfPlayer();
}

uint8_t priestsFilters(Unit unit) {
  UnitType def = unit_Type(unit);
  return eeTypes_IsPriest(def);
}

uint8_t enemyFilter(Unit unit) {
  uint8_t target = eeTa_Player(unit);
  uint8_t self = eeTa_SelfPlayer();

  return target != self && !ply_Index_AreAllies(target, self);
}

uint8_t enemyTransporters(Unit unit) {
  UnitType def = unit_Type(unit);
  int8_t unitPlayer = eeTa_Player(unit);
  int8_t playerTeam = eeTa_SelfPlayer();
  return eeTypes_IsTransport(def) &&
         unitPlayer != playerTeam && !ply_Index_AreAllies(unitPlayer, playerTeam);
}

uint8_t enemyUnits(Unit unit) {
  UnitType def = unit_Type(unit);
  int8_t unitPlayer = eeTa_Player(unit);
  int8_t playerTeam = eeTa_SelfPlayer();
  return unitPlayer != playerTeam && !ply_Index_AreAllies(unitPlayer, playerTeam) &&
         !eeTa_IsNeutral(unit);
}

uint8_t idleAttackingGroundUnits(Unit unit) {
  UnitType def = unit_Type(unit);
  return unit_IsIdle(unit) && eeTypes_IsGroundUnit(def) && !eeTypes_IsFromClass(CLASS_CITIZENS, def);
}

uint8_t idleAttackingAirUnits(Unit unit) {
  UnitType def = unit_Type(unit);
  return unit_IsIdle(unit) && eeTypes_IsAirUnit(def) && !eeTypes_IsFromClass(CLASS_AIR_TRANSPORT, def);
}

uint8_t idleAttackingWaterUnits(Unit unit) {
  UnitType def = unit_Type(unit);
  return unit_IsIdle(unit) && (eeTypes_IsFromClass(CLASS_WATER_BOATS, def) || eeTypes_IsFromClass(CLASS_SUBMARINES, def));
}

void att_AddDamagedUnits(Unit unit) {
  if(eeTa_Player(unit) != eeTa_SelfPlayer()) {
    return ;
  }
  UnitType def = unit_Type(unit);
  if(!unit_IsBuilding(unit) && !eeTypes_IsCitizen(def)) {
    return ;
  }
  Point pos = unit_Point_Position(unit);
  attackedUnits[make_pair(pos.x, pos.y)] = 1;
}

void att_AttackTransportWithNavals(vector<Unit> &units) {
  vector<Unit> filteredUnits = eeTa_Filter(units, navalAttackFilter);

  for(uint32_t i = 0, c = min(6, filteredUnits.size()); i < c; i++) {
    vector<Unit> enemyTransports = eeTa_Filter(units, enemyTransporters);

    for(uint32_t j = 0, p = enemyTransports.size(); j < p; j++) {
      Point currPos = unit_Point_Position(filteredUnits[i]);
      Point enemyPos = unit_Point_Position(enemyTransports[j]);

      Circle circle = (Circle) {
        .p = currPos,
        .radius = 15.0f
      };

      if(geom_IsPointInCircle(enemyPos, circle)) {
        help_UnitMove(filteredUnits[i]._payload, enemyPos, UNIT_ATTACK);
        break;
      }
    }
  }
}

void att_ConvertIfNecessary(vector<Unit> &units) {
  vector<Unit> filteredPriests = eeTa_Filter(units, priestsFilters);
  for(size_t i = 0; i < filteredPriests.size(); i++) {
    Unit enemy = geom_GetClosestUnitFrom(filteredPriests[i], eeTa_AllPlayers(), enemyFilter);
    if(!enemy._payload) {
      continue;
    }
    help_ConvertTarget(filteredPriests[i]._payload, enemy._payload);
  }
}

Point airNextPosition(Unit unit) {
  Point currentPosition = unit_Point_Position(unit);
  Point copyPosition = currentPosition;
  currentPosition.x = copyPosition.x + sinf(rand()) * 20.0f;
  currentPosition.y = copyPosition.y + sinf(rand()) * 20.0f;
  int32_t index = 5;
  while(index && map_Tile_GetPlaneID(geom_Tile_FromPoint(currentPosition)) == INVALID_TILE_ID) {
    currentPosition.x = copyPosition.x + sinf(rand()) * 20.0f;
    currentPosition.y = copyPosition.y + sinf(rand()) * 20.0f;
    index--;
  }
  if(!index) {
    return geom_Point_Invalid();
  }
  return currentPosition;
}

Point getNextPosition(Unit unit) {
  if(eeTypes_IsAirUnit(unit_Type(unit))) {
    return airNextPosition(unit);
  }
  Point currentPosition = unit_Point_Position(unit);
  Point copyPosition = currentPosition;
  currentPosition.x += sinf(rand()) * 20.0f;
  currentPosition.y += sinf(rand()) * 20.0f;
  int32_t index = 5;
  while(index && map_Tile_GetPlaneID(geom_Tile_FromPoint(copyPosition)) != map_Tile_GetPlaneID(geom_Tile_FromPoint(currentPosition))) {
    currentPosition.x = copyPosition.x + sinf(rand()) * 20.0f;
    currentPosition.y = copyPosition.y + sinf(rand()) * 20.0f;
    index--;
  }
  if(!index) {
    return geom_Point_Invalid();
  }
  return currentPosition;
}

Point att_RandomMove(Unit unit) {
  return getNextPosition(unit);
}

uint8_t att_ScanAndAttack(vector<Unit> &selfUnits) {
  if(!attackedUnits.size()) {
    return 0;
  }
  int32_t maxCommands = 12;
  for(auto &it : attackedUnits) {
    uint8_t hasTroopsBeenSent = 0;
    for(size_t i = 0, c = selfUnits.size(); i < c; i++) {
      if(idleAttackingGroundUnits(selfUnits[i]) || idleAttackingAirUnits(selfUnits[i])) {
        hasTroopsBeenSent = 1;
        maxCommands--;
      }
      else {
        continue;
      }
      Point p = (Point) {
        .x = it.first.first,
        .y = it.first.second
      };
      unit_Action(selfUnits[i], p, UNIT_ATTACK);
      if(!maxCommands) {
        return 1;
      }
    }
    attackedUnits.erase(it.first);
    return hasTroopsBeenSent;
  }
  return 0;
}

void att_PatrolRandomPositions_t(vector<Unit> &selfUnits, uint8_t (*checker)(Unit), int32_t maxCommands) {
  vector<Unit> filteredUnits = eeTa_Filter(selfUnits, checker);
  if(!filteredUnits.size()) {
    return ;
  }
  if(att_ScanAndAttack(filteredUnits)) {
    return ;
  }
  Point randomPos = att_RandomMove(filteredUnits[0]);
  for(size_t i = 0, c = filteredUnits.size(); i < c; i++) {
    maxCommands--;
    unit_Action(filteredUnits[i], randomPos, UNIT_ATTACK);
    if(!maxCommands) {
      return ;
    }
  }
}

uint8_t att_IsUnitCarrier(Unit unit) {
  return eeTypes_IsFromClass(CLASS_WATER_CARRIERS, unit_Type(unit));
}

uint8_t isPriest(Unit unit) {
  return unit_GetPlayerIndex(unit) == eeTa_SelfPlayer() && unit_Type(unit) == PRIEST;
}

uint8_t isProphet(Unit unit) {
  return unit_GetPlayerIndex(unit) == eeTa_SelfPlayer() && unit_Type(unit) == PROPHET;
}

uint8_t enemyUnit(Unit unit) {
  return unit_GetPlayerIndex(unit) != eeTa_SelfPlayer() && !unit_IsBuilding(unit);
}

uint8_t enemyBuilding(Unit unit) {
  return unit_GetPlayerIndex(unit) != eeTa_SelfPlayer() && unit_IsBuilding(unit);
}

void priest_ConvertIfPossible(Unit priest) {
  vector<Unit> units = unit_Filter(enemyUnit);
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(unit_Distance(priest, units[i]) <= unit_Range(priest)) {
      unit_Convert(priest, units[i]);
      return ;
    }
  }
}

void att_ProcessPriests() {
  size_t total = 3;
  vector<Unit> units = unit_Filter(isPriest);
  for(size_t i = 0, c = min(total, units.size()); i < c; i++) {
    priest_ConvertIfPossible(units[i]);
  }
}

void prophet_CastMalaria(Unit prophet, vector<Unit> &units, uint8_t *casted) {
  if(*casted) {
    return ;
  }
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(unit_Distance(prophet, units[i]) <= unit_Range(prophet)) {
      unit_CastAbility(prophet, unit_Point_Position(units[i]), PROPHET_MALARIA);
      *casted = 1;
      return ;
    }
  }
}

void prophet_CastTornado(Unit prophet, vector<Unit> &units, uint8_t *casted) {
  if(*casted) {
    return ;
  }
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(unit_Distance(prophet, units[i]) <= unit_Range(prophet) && eeTypes_IsWaterUnit(unit_Type(units[i]))) {
      unit_CastAbility(prophet, unit_Point_Position(units[i]), PROPHET_TORNADO);
      *casted = 1;
      return ;
    }
  }
}

void prophet_CastEarthquake(Unit prophet, uint8_t *casted) {
  if(*casted) {
    return ;
  }
  vector<Unit> units = unit_Filter(enemyBuilding);
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(unit_Distance(prophet, units[i]) <= unit_Range(prophet)) {
      unit_CastAbility(prophet, unit_Point_Position(units[i]), PROPHET_EARTHQUAKE);
      *casted = 1;
      return ;
    }
  }
}

void prophet_CastAbilities(Unit prophet) {
  vector<Unit> units = unit_Filter(enemyUnit);
  uint8_t casted = 0;
  prophet_CastMalaria(prophet, units, &casted);
  prophet_CastTornado(prophet, units, &casted);
  prophet_CastEarthquake(prophet, &casted);
}

void att_ProcessProphets() {
  size_t total = 4;
  vector<Unit> units = unit_Filter(isProphet);
  for(size_t i = 0, c = min(total, units.size()); i < c; i++) {
    prophet_CastAbilities(units[i]);
  }
}

void att_ProcessSpecialAbilityUnits(PVOID _) {
  att_ProcessPriests();
  att_ProcessProphets();
}

void att_PatrolRandomPositions(vector<Unit> &selfUnits) {
  att_PatrolRandomPositions_t(selfUnits, idleAttackingAirUnits, 6);
  att_PatrolRandomPositions_t(selfUnits, idleAttackingGroundUnits, 7);
  att_PatrolRandomPositions_t(selfUnits, idleAttackingWaterUnits, 3);
}

void att_OnFrame() {

}