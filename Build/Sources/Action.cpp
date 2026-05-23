#include "Action.h"
#include "Offset.h"
#include "LibManager.h"
#include "Game.h"
#include "UnitPrivate.h"

Action act_GetAction(PVOID actionInstance);

Action act_Get(Unit unit) {
  if(unit_IsIdle(unit)) {
    return (Action) {
      .type = ACTION_IDLE
    };
  }

  return act_GetAction((PVOID)*(size_t *)((size_t)unit_Reference(unit) + UNIT_ACTION_POINTER_INSTANCE));
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

Action act_GetAction(PVOID actionInstance)
{
  Action action;
  ActionType type = act_GetActionType(actionInstance);
  action.type = type;
  switch (type)
  {
    case ACTION_ATTACK_TARGET: {
      action.target = unit_FromPayload((PVOID)*(size_t *)((size_t)actionInstance + 0x20));
      break;
    }

    case ACTION_ATTACK_AREA: {
      action.targetPoint.x = *(float *)((size_t)actionInstance + 0x20);
      action.targetPoint.y = *(float *)((size_t)actionInstance + 0x24);
      break;
    }

    case ACTION_MOVE: {
      action.targetPoint.x = *(float *)((size_t)actionInstance + 0x20);
      action.targetPoint.y = *(float *)((size_t)actionInstance + 0x24);
      break;
    }

    case ACTION_CAST_TARGET: {
      action.target = unit_FromPayload((PVOID)*(size_t *)((size_t)actionInstance + 0x24));
      break;
    }

    case ACTION_CAST_AREA: {
      action.targetTile.x = *(size_t *)((size_t)actionInstance + 0x28);
      action.targetTile.y = *(size_t *)((size_t)actionInstance + 0x2C);
      break;
    }

    case ACTION_GATHER: {
      action.targetSimpleUnit = su_FromPayload((PVOID)*(size_t *)((size_t)actionInstance + 0x38));
      break;
    }

    case ACTION_REPAIR: {
      action.target = unit_FromPayload((PVOID)*(size_t *)((size_t)actionInstance + 0x3C));
      break;
    }

    case ACTION_LOAD: {
      action.loadStartTarget = (PVOID)*(size_t *)((size_t)actionInstance + 0x28);
      action.loadEndTarget = (PVOID)*(size_t *)((size_t)actionInstance + 0x2C);
      break;
    }

    case ACTION_UNLOAD: {
      action.targetTile.x = *(size_t *)((size_t)actionInstance + 0x34);
      action.targetTile.y = *(size_t *)((size_t)actionInstance + 0x38);
      break;
    }

    case ACTION_IDLE: {
    default:
      action.type = ACTION_IDLE;
      break;
    }
  }

  return action;
}