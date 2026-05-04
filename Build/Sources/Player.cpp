#include "Player.h"
#include "Game.h"
#include "InjectUtilities.h"
#include "Helpers.h"
#include "LibManager.h"

// void ply_RegisterSelf(PVOID self);

// void ply_Inits(PVOID self) {
//   PPlayers playerData = game_Players();
//   PVOID currentPlayer = helper_Player_FromUnit(self);
//   (*playerData->playerData)[currentPlayer] = 1;
//   ply_RegisterSelf(playerData, self);
// }

// uint8_t ply_GetPlayerIndex(Unit unit) {
//   PVOID nextStruct = ply_Reference(ply_GetPlayer(unit));
//   return *(uint8_t *)util_Pointer((PVOID)nextStruct, 0x45C, INT8_T_TYPE);
// }

// Player ply_GetPlayer(Unit unit) {
//   return (Player) {
//     ._payload = helper_Player_FromUnit(unit_Reference(unit))
//   };
// }

// PVOID ply_Reference(Player self) {
//   return self._payload;
// }

// void ply_RegisterSelf(PPlayers playerData, PVOID self) {

// }