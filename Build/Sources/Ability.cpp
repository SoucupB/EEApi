#include "Ability.h"
#include "Player.h"
#include "Helpers.h"
#include "LibManager.h"
#include "EETypesStructPrivate.h"
#include "Offset.h"

PVOID ability_Manager() {
  return (PVOID)*(size_t *)((size_t)lib_BaseAddress() + ABILITY_REMOTE_METHOD_MANAGER);
}

size_t abiliy_FindCallerIndex(PVOID cTechNodes) {
  return *(size_t *)((*(size_t *)((size_t)cTechNodes + ABILITY_TECH_NODE_INDEX_HOLDER)) + ABILITY_TECH_NODE_INDEX);
}

PVOID ability_Instance(AbilityTypes ability) {
  TechTree tree = ply_TechTree(ply_Self());
  PVOID cTechNode = helper_TechNode(tree, ability);
  if(!cTechNode) {
    return 0;
  }
  size_t abilityIndex = abiliy_FindCallerIndex(cTechNode);
  return helper_AbilityPointer(ability_Manager(), abilityIndex);
}

int32_t ability_Energy(AbilityTypes ability) {
  PVOID reference = ability_Instance(ability);
  if(!reference) {
    return 0;
  }
  return helper_AbilityEnergy(reference);
}