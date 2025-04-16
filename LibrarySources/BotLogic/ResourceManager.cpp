#include "ResourceManager.h"
#include "EETwa.h"
#include <vector>
#include "../Geometry/Geometry.h"

using namespace std;

uint8_t fishBoatsFilter(PVOID unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  return eeTypes_IsFishBoat(def);
}

uint8_t fishFilter(PVOID unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  return def == IDLE; // Fishes don't have a type for some reason.
}

void fishTheFishes(PVOID fishBoat) {
  PVOID unit = geom_GetClosestUnitFrom(fishBoat, eeTa_NeutralPlayer(), fishFilter);
  if(!unit) {
    return ;
  }

  help_MoveToTarget(fishBoat, unit);
}

void res_MoveResourceBoats() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  vector<PVOID> fishBoats = eeTa_Filter(units, fishBoatsFilter);

  int32_t maxSearches = 5;

  for(size_t i = 0, c = fishBoats.size(); i < c; i++) {
    PVOID unit = fishBoats[i];
    if(eeTa_IsUnitIdle(unit)) {
      fishTheFishes(unit);
      maxSearches--;
    }
    if(!maxSearches) {
      return ;
    }
  }
}