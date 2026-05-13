#include "Player.h"
#include "Game.h"
#include "InjectUtilities.h"
#include "Helpers.h"
#include "LibManager.h"
#include "EETwa.h"

void ply_RegisterSelf(PPlayers playerData, Player self);
void ply_RegisterNeutral(PPlayers playerData, Player self);

void ply_Inits(PVOID self) {
  PPlayers playerData = game_Players();
  PVOID currentPlayer = helper_Player_FromUnit(self);
  (*playerData->playerData)[currentPlayer] = 1;
  ply_RegisterSelf(playerData, (Player) {
    ._payload = currentPlayer
  });
  ply_RegisterNeutral(playerData, (Player) {
    ._payload = currentPlayer
  });
}

int8_t ply_PlayerCount() {
  return game_Players()->playerData->size();
}

uint8_t ply_PlayerIndex(Player player) {
  if(!ply_Reference(player)) {
    return 20;
  }
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

vector<Player> ply_All() {
  vector<Player> response;
  PPlayers playerData = game_Players();
  for(auto &iter : *playerData->playerData) {
    response.push_back((Player) {
      ._payload = iter.first
    });
  }
  return response;
}

PVOID ply_Reference(Player self) {
  return self._payload;
}

Player ply_Null() {
  return (Player) {
    ._payload = NULL
  };
}

Player ply_Self() {
  PPlayers playerData = game_Players();
  return playerData->self;
}

Player ply_Neutral() {
  PPlayers playerData = game_Players();
  return playerData->neutral;
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

int8_t ply_Index_AreAllies(uint8_t plySrc, uint8_t plyDst) {
  PVOID _1 = util_Pointer(lib_BaseAddress(), 0x530DB4 + 0x4 * plySrc, POINTER_TYPE);
  if(!_1) {
    return 0;
  }
  uint8_t *_2 = (uint8_t *)util_Pointer(_1, 0x4 * plyDst + 0x9DC, INT8_T_TYPE);

  return !*_2;
}

uint8_t ply_AreAllies(Player a, Player b) {
  uint8_t indexA = ply_PlayerIndex(a);
  uint8_t indexB = ply_PlayerIndex(b);
  return ply_Index_AreAllies(indexA, indexB);
}

void ply_Print() {
  PPlayers playerData = game_Players();
  eeTa_FilePrintf("Self player is %p\n", ply_Reference(playerData->self));
  eeTa_FilePrintf("Neutral player is %p\n", ply_Reference(playerData->neutral));
  unordered_map<PVOID, uint8_t> *dt = playerData->playerData;
  eeTa_FilePrintf("Total players count is %d\n", dt->size());
  for(auto &iter : *dt) {
    eeTa_FilePrintf("Player %p with index %d\n", iter.first, ply_PlayerIndex((Player) {
      ._payload = iter.first
    }));
  }

  for(auto &iter : *dt) {
    eeTa_FilePrintf("Player %p is allied with: ", iter.first);
    for(auto &index : *dt) {
      if(index.first == iter.first) {
        continue;
      }
      Player a = (Player) {
        ._payload = iter.first
      };
      Player b = (Player) {
        ._payload = index.first
      };
      if(ply_AreAllies(a, b)) {
        eeTa_FilePrintf("%d ", ply_PlayerIndex((Player) {
          ._payload = index.first
        }));
      }
    }
    eeTa_FilePrintf("\n");
  }
}