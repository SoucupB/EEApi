#include "Action.h"
#include "Offset.h"
#include "LibManager.h"
#include "Game.h"
#include "UnitPrivate.h"

Action act_Get(Unit unit) {
  if(unit_IsIdle(unit)) {
    return (Action) {
      .type = ACTION_IDLE
    };
  }


}

ActionType act_GeneralGetAction(PVOID actionInstance) {
  size_t actInstance = *(size_t *)((size_t)actionInstance + 0x30);
  switch (actInstance)
  {
    case OFFSET_ACTION_GENERAL_ATTACK:
      return ACTION_ATTACK_AREA;
    case OFFSET_ACTION_GENERAL_MOVE:
      return ACTION_MOVE;
    
    default:
      break;
  }
  return ACTION_IDLE;
}

ActionType act_CastSpellAction(PVOID actionInstance) {
  PVOID actInstance = (PVOID)*(size_t *)((size_t)actionInstance + 0x24);
  if(unit_Exists((Unit) {
    ._payload = actInstance
  })) {
    return ACTION_CAST_TARGET;
  }
  return ACTION_CAST_AREA;
}

ActionType act_GetActionType(PVOID actionInstance) {
  size_t actionClassInstance = *(size_t *)actionInstance;
  PVOID actionClassOffset = (PVOID)(actionClassInstance - (size_t)lib_BaseAddress());
  switch ((size_t)actionClassInstance)
  {
    case OFFSET_ACTION_GENERAL_POSITION:
      return act_GeneralGetAction(actionInstance);

    case OFFSET_ACTION_ATTACK_TARGET:
      return ACTION_ATTACK_TARGET;

    case OFFSET_ACTION_CASTSPELL:
      return act_CastSpellAction(actionInstance);

    case OFFSET_ACTION_GATHER:
      return ACTION_GATHER;

    case OFFSET_ACTION_REPAIR:
      return ACTION_REPAIR;

    case OFFSET_ACTION_LOAD:
      return ACTION_LOAD;

    case OFFSET_ACTION_UNLOAD:
      return ACTION_UNLOAD;
    
    default:
      break;
  }

  return ACTION_IDLE;
}