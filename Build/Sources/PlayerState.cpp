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
  int32_t health = eeTa_CurrentHp((Unit) {._payload = unit});
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

int8_t pls_IsUnitAttacked(Unit unit) {
  return unitsHealth[unit._payload].isBeingAttacked;
}

void pls_OnUnitIteration(Unit unit) {
  pls_ProcessHealth(unit._payload);
}

void pls_OnUnitDestory(Unit unit) {
  unitsHealth.erase(unit._payload);
}

void pls_OnInit(PVOID remoteMethod) {
  method = remoteMethod;
}