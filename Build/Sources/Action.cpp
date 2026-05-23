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

Action act_GetAction(PVOID actionInstance) {
  ActionType type = act_GetActionType(actionInstance);
  switch (type)
  {
    case ACTION_ATTACK_TARGET:
      return (Action) {
        .type = ACTION_ATTACK_TARGET,
        .target = unit_FromPayload((PVOID)*(size_t *)((size_t)actionInstance + 0x20))
      };
    case ACTION_ATTACK_AREA:
      return (Action) {
        .type = ACTION_ATTACK_AREA,
        .targetPoint = (Point) {
          .x = *(float *)((size_t)actionInstance + 0x20),
          .y = *(float *)((size_t)actionInstance + 0x24),
        }
      };
    case ACTION_MOVE:
      return (Action) {
        .type = ACTION_MOVE,
        .targetPoint = (Point) {
          .x = *(float *)((size_t)actionInstance + 0x20),
          .y = *(float *)((size_t)actionInstance + 0x24),
        }
      };
    case ACTION_CAST_TARGET:
      return (Action) {
        .type = ACTION_CAST_TARGET,
        .target = unit_FromPayload((PVOID)*(size_t *)((size_t)actionInstance + 0x24))
      };
    case ACTION_CAST_AREA:
      return (Action) {
        .type = ACTION_CAST_AREA,
        .targetTile = (TilePoint) {
          .x = *(size_t *)((size_t)actionInstance + 0x28),
          .y = *(size_t *)((size_t)actionInstance + 0x2C),
        }
      };
    case ACTION_GATHER:
      return (Action) {
        .type = ACTION_GATHER,
        .targetSimpleUnit = su_FromPayload((PVOID)*(size_t *)((size_t)actionInstance + 0x38))
      };
    case ACTION_REPAIR:
      return (Action) {
        .type = ACTION_REPAIR,
        .target = unit_FromPayload((PVOID)*(size_t *)((size_t)actionInstance + 0x3C))
      };
    case ACTION_LOAD:
      return (Action) {
        .type = ACTION_LOAD,
        .loadStartTarget = (PVOID)*(size_t *)((size_t)actionInstance + 0x28),
        .loadEndTarget = (PVOID)*(size_t *)((size_t)actionInstance + 0x2C)
      };
    case ACTION_UNLOAD:
      return (Action) {
        .type = ACTION_UNLOAD,
        .targetTile = (TilePoint) {
          .x = *(size_t *)((size_t)actionInstance + 0x34),
          .y = *(size_t *)((size_t)actionInstance + 0x38),
        }
      };
    
    default:
      break;
  }

  return (Action) {
    .type = ACTION_IDLE
  };
}