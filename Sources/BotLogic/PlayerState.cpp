#include <algorithm>
#include <unordered_map>
#include "PlayerState.h"

using namespace std;
typedef struct Player_t {
  int8_t isBeingAttacked;
  int32_t health;
} Player;

static unordered_map<PVOID, Player> unitsHealth;
static PVOID method;

void pls_AddMethod(PVOID unit) {
  if(!unit || !method) {
    return ;
  }
  void (*remoteMethod)(PVOID) = (void (*)(PVOID))method;
  remoteMethod(unit);
}

void pls_ProcessHealth(PVOID unit) {
  int32_t health = eeTa_CurrentHp(unit);
  if(unitsHealth.find(unit) == unitsHealth.end()) {
    unitsHealth[unit] = (Player) {
      .isBeingAttacked = 0,
      .health = health,
    };
    return ;
  }
  if(unitsHealth[unit].health > health) {
    unitsHealth[unit].health = health;
    unitsHealth[unit].isBeingAttacked = 1;
    pls_AddMethod(unit);
    return ;
  }
  unitsHealth[unit].isBeingAttacked = 0;
}

int8_t pls_IsUnitAttacked(PVOID unit) {
  return unitsHealth[unit].isBeingAttacked;
}

void pls_OnUnitIteration(PVOID unit) {
  pls_ProcessHealth(unit);
}

void pls_OnUnitDestory(PVOID unit) {
  unitsHealth.erase(unit);
}

void pls_OnInit(PVOID remoteMethod) {
  method = remoteMethod;
}