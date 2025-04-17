#include "AttackDecision.h"
#include "Geometry/Geometry.h"
#include "EETwa.h"
#include "EETwaTypes.h"
#include "PlayerState.h"
#include <math.h>
#include <map>

static map<pair<float, float>, uint8_t> attackedUnits;

size_t min(size_t a, size_t b) {
  return a < b ? a : b;
}

uint8_t navalAttackFilter(Unit unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  return eeTypes_IsWater(def) && !eeTypes_IsTransport(def) && !att_IsUnitCarrier(unit) && !eeTypes_IsFishBoat(def) &&
         eeTa_Player(unit) == eeTa_SelfPlayer();
}

uint8_t airAttackFilter(Unit unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  return eeTypes_IsAir(def) && !eeTypes_IsTransport(def) &&
         eeTa_Player(unit) == eeTa_SelfPlayer();
}

uint8_t enemyTransporters(Unit unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  int8_t unitPlayer = eeTa_Player(unit);
  int8_t playerTeam = eeTa_SelfPlayer();
  return eeTypes_IsWater(def) && eeTypes_IsTransport(def) &&
         unitPlayer != playerTeam && !eeTa_AreAllies(unitPlayer, playerTeam);
}

uint8_t enemyUnits(Unit unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  int8_t unitPlayer = eeTa_Player(unit);
  int8_t playerTeam = eeTa_SelfPlayer();
  return unitPlayer != playerTeam && !eeTa_AreAllies(unitPlayer, playerTeam) &&
         !eeTa_IsNeutral(unit);
}

uint8_t idleGroundUnits(Unit unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  return eeTa_IsUnitIdle(unit) && !eeTypes_IsWorker(def) && !eeTypes_IsTransport(def) && eeTypes_IsGround(def);
}

uint8_t idleAirUnits(Unit unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  return eeTa_IsUnitIdle(unit) && !eeTypes_IsTransport(def) && eeTypes_IsAir(def);
}

uint8_t idleWaterUnits(Unit unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  return eeTa_IsUnitIdle(unit) && !att_IsUnitCarrier(unit) && !eeTypes_IsFishBoat(def) && !eeTypes_IsTransport(def) && eeTypes_IsWater(def);
}

void att_AddDamagedUnits(Unit unit) {
  if(eeTa_Player(unit) != eeTa_SelfPlayer()) {
    return ;
  }
  UnitTypeDef def = eeTa_UnitType(unit);
  if(!eeTa_IsBuilding(unit) && !eeTypes_IsWorker(def)) {
    return ;
  }
  
  Point pos = eeTa_CurrentPosition(unit);
  attackedUnits[make_pair(pos.x, pos.y)] = 1;
}

void att_AttackTransportWithNavals(vector<Unit> &units) {
  vector<Unit> filteredUnits = eeTa_Filter(units, navalAttackFilter);

  for(uint32_t i = 0, c = min(6, filteredUnits.size()); i < c; i++) {
    vector<Unit> enemyTransports = eeTa_Filter(units, enemyTransporters);

    for(uint32_t j = 0, p = enemyTransports.size(); j < p; j++) {
      Point currPos = eeTa_CurrentPosition(filteredUnits[i]);
      Point enemyPos = eeTa_CurrentPosition(enemyTransports[j]);

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

void att_AttackEnemiesWithPlanes(vector<Unit> &units) {
  vector<Unit> filteredUnits = eeTa_Filter(units, airAttackFilter);

  for(uint32_t i = 0, c = min(6, filteredUnits.size()); i < c; i++) {
    vector<Unit> enemyTransports = eeTa_Filter(units, enemyUnits);

    for(uint32_t j = 0, p = enemyTransports.size(); j < p; j++) {
      Point currPos = eeTa_CurrentPosition(filteredUnits[i]);
      Point enemyPos = eeTa_CurrentPosition(enemyTransports[j]);

      Circle circle = (Circle) {
        .p = currPos,
        .radius = 30.0f
      };

      if(geom_IsPointInCircle(enemyPos, circle)) {
        help_UnitMove(filteredUnits[i]._payload, enemyPos, UNIT_ATTACK);
        break;
      }
    }
  }
}

Point att_RandomMove(PVOID unit) {
  Point currentPosition = eeTa_CurrentPosition((Unit) {._payload = unit});
  currentPosition.x += sinf(rand()) * 20.0f;
  currentPosition.y += sinf(rand()) * 20.0f;
  return currentPosition;
}

uint8_t att_ScanAndAttack(vector<Unit> &selfUnits) {
  if(!attackedUnits.size()) {
    return 0;
  }
  int32_t maxCommands = 12;
  for(auto &it : attackedUnits) {
    uint8_t hasTroopsBeenSent = 0;
    for(size_t i = 0, c = selfUnits.size(); i < c; i++) {
      if(idleGroundUnits(selfUnits[i]) || idleAirUnits(selfUnits[i])) {
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
      help_UnitMove(selfUnits[i]._payload, p, UNIT_ATTACK);
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
  Point randomPos = att_RandomMove(filteredUnits[0]._payload);
  for(size_t i = 0, c = filteredUnits.size(); i < c; i++) {
    maxCommands--;
    help_UnitMove(filteredUnits[i]._payload, randomPos, UNIT_ATTACK);
    if(!maxCommands) {
      return ;
    }
  }
}

uint8_t att_IsUnitCarrier(Unit unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  return def == UNIT_WW2_CARRIER || def == UNIT_WWM_CARRIER ||
         def == UNIT_DIGITAL_PORT_NAVAL || def == UNIT_NANO_PORT_NAVAL ||
         def == UNIT_SPACE_SPACE_CARRIER;
}

void att_PatrolRandomPositions(vector<Unit> &selfUnits) {
  att_PatrolRandomPositions_t(selfUnits, idleAirUnits, 6);
  att_PatrolRandomPositions_t(selfUnits, idleGroundUnits, 7);
  att_PatrolRandomPositions_t(selfUnits, idleWaterUnits, 3);
}

void att_OnFrame() {

}