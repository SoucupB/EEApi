#include "CitizenManager.h"
#include "Unit.h"

uint8_t needyBuilding(Unit unit) {
  return unit_TotalHP(unit) > unit_CurrentHp(unit);
}

Unit citizen_GetBuilding() {
  vector<Unit> buildings = unit_GetBuildings(eeTa_SelfPlayer());
  vector<Unit> needyBuildings = eeTa_Filter(buildings, needyBuilding);
  if(!needyBuildings.size()) {
    return unit_Null();
  }
  return needyBuildings[rand() % needyBuildings.size()];
}

void citizen_RepairBuilding(Unit citizen) {
  Unit currentBuilding = citizen_GetBuilding();
  if(!unit_Reference(currentBuilding)) {
    return ;
  }
  unit_Repair(citizen, currentBuilding);
}

void citizen_RepairBuildings(PVOID _) {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(unit_IsIdle(units[i]) && eeTypes_IsCitizen(unit_Type(units[i]))) {
      citizen_RepairBuilding(units[i]);
    }
  }
}