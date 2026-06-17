#include <algorithm>
#include <unordered_map>
#include "PlayerState.h"
#include "Game.h"
#include "Unit.h"
#include "PlayerStatePrivate.h"
#include "EETypesStructPrivate.h"

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

void pls_OnUnitDestory(Unit unit) {
  PPlayerState ps = game_GetPlayerState();
  (*ps->unitsHealth).erase(unit._payload);
}

void pls_OnInit(void (*remoteMethod)(Unit)) {
  PPlayerState ps = game_GetPlayerState();
  ps->method = (PVOID)remoteMethod;
}