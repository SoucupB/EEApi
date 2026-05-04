#include <algorithm>
#include <unordered_map>
#include "PlayerState.h"
#include "Game.h"
#include "Unit.h"

using namespace std;

void pls_AddMethod(PVOID unit) {
  PPlayerState ps = game_GetPlayerState();

  if(!unit || !ps->method) {
    return ;
  }
  void (*remoteMethod)(PVOID) = (void (*)(PVOID))ps->method;
  remoteMethod(unit);
}

void pls_ProcessHealth(PVOID unit) {
  PPlayerState ps = game_GetPlayerState();
  unordered_map<PVOID, UnitHealth> *unitsHealth = ps->unitsHealth;
  
  int32_t health = unit_CurrentHp((Unit) {._payload = unit});
  if(unitsHealth->find(unit) == unitsHealth->end()) {
    (*unitsHealth)[unit] = (UnitHealth) {
      .isBeingAttacked = 0,
      .health = health,
    };
    return ;
  }
  if((*unitsHealth)[unit].health > health) {
    (*unitsHealth)[unit].health = health;
    (*unitsHealth)[unit].isBeingAttacked = 1;
    pls_AddMethod(unit);
    return ;
  }
  (*unitsHealth)[unit].isBeingAttacked = 0;
}

int8_t pls_IsUnitAttacked(Unit unit) {
  PPlayerState ps = game_GetPlayerState();
  return (*ps->unitsHealth)[unit._payload].isBeingAttacked;
}

void pls_OnUnitIteration(Unit unit) {
  pls_ProcessHealth(unit._payload);
}

void pls_OnUnitDestory(Unit unit) {
  PPlayerState ps = game_GetPlayerState();
  (*ps->unitsHealth).erase(unit._payload);
}

void pls_OnInit(PVOID remoteMethod) {
  PPlayerState ps = game_GetPlayerState();
  ps->method = remoteMethod;
}