#include "ResourceManager.h"
#include "EETwa.h"
#include <vector>
#include "../Geometry/Geometry.h"

using namespace std;

uint8_t fishBoatsFilter(Unit unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  return eeTypes_IsFishBoat(def);
}

uint8_t fishFilter(Unit unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  return def == IDLE; // Fishes don't have a type for some reason.
}

void fishTheFishes(PVOID fishBoat) {
  Unit unit = geom_GetClosestUnitFrom((Unit) {._payload = fishBoat}, eeTa_NeutralPlayer(), fishFilter);
  if(!unit._payload) {
    return ;
  }

  help_MoveToTarget(fishBoat, unit._payload);
}

void res_MoveResourceBoats() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  vector<Unit> fishBoats = eeTa_Filter(units, fishBoatsFilter);

  int32_t maxSearches = 5;

  for(size_t i = 0, c = fishBoats.size(); i < c; i++) {
    Unit unit = fishBoats[i];
    if(eeTa_IsUnitIdle(unit)) {
      fishTheFishes(unit._payload);
      maxSearches--;
    }
    if(!maxSearches) {
      return ;
    }
  }
}