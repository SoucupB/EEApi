#include "Player.h"
#include "Game.h"
#include "InjectUtilities.h"
#include "Helpers.h"
#include "LibManager.h"

void ply_RegisterSelf(PPlayers playerData, Player self);
void ply_RegisterNeutral(PPlayers playerData, Player self);

void ply_Inits(PVOID self) {
  PPlayers playerData = game_Players();
  PVOID currentPlayer = helper_Player_FromUnit(self);
  (*playerData->playerData)[currentPlayer] = 1;
  ply_RegisterSelf(playerData, (Player) {
    ._payload = self
  });
  ply_RegisterNeutral(playerData, (Player) {
    ._payload = self
  });
}

uint8_t ply_PlayerIndex(Player player) {
  return *(uint8_t *)util_Pointer(ply_Reference(player), 0x45C, INT8_T_TYPE);
}

uint8_t ply_GetPlayerIndex(Unit unit) {
  return ply_PlayerIndex(ply_GetPlayer(unit));
}

Player ply_GetPlayer(Unit unit) {
  return (Player) {
    ._payload = helper_Player_FromUnit(unit_Reference(unit))
  };
}

PVOID ply_Reference(Player self) {
  return self._payload;
}

Player ply_Null() {
  return (Player) {
    ._payload = NULL
  };
}

void ply_RegisterSelf(PPlayers playerData, Player self) {
  size_t playerClass = *(size_t *)ply_Reference(self);
  if(playerClass != 0x43C330 + (size_t)lib_BaseAddress()) {
    return ;
  }
  uint8_t playerIndex = ply_PlayerIndex(self);
  if(playerIndex == 1) {
    playerData->self = self;
  }
}

void ply_RegisterNeutral(PPlayers playerData, Player player) {
  size_t playerClass = *(size_t *)ply_Reference(player);
  if(playerClass != 0x43C330 + (size_t)lib_BaseAddress()) {
    return ;
  }
  uint8_t playerIndex = ply_PlayerIndex(player);
  if(playerIndex != 1) {
    playerData->neutral = player;
  }
}