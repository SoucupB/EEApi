#include "CitizenManager.h"
#include "Unit.h"
#include "EETwa.h"

uint8_t needyBuilding(Unit unit) {
  return unit_GetPlayerIndex(unit) == eeTa_SelfPlayer() && unit_IsBuilding(unit) && unit_TotalHP(unit) > unit_CurrentHp(unit);
}

void citizen_RepairBuildings(PVOID _) {
  vector<Unit> needyBuildings = unit_Filter(needyBuilding);
  if(!needyBuildings.size()) {
    return ;
  }
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(unit_IsIdle(units[i]) && eeTypes_IsCitizen(unit_Type(units[i]))) {
      unit_Repair(units[i], needyBuildings[(rand() * rand()) % needyBuildings.size()]);
    }
  }
}