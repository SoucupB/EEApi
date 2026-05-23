#pragma once
#include "Unit.h"
#include "Geometry.h"
#include "Ability.h"

enum ActionType {
  ACTION_IDLE,
  ACTION_MOVE,
  ACTION_REPAIR,
  ACTION_ATTACK_AREA,
  ACTION_ATTACK_TARGET,
  ACTION_LOAD,
  ACTION_UNLOAD,
  ACTION_CAST_AREA,
  ACTION_CAST_TARGET,
  ACTION_GATHER
};

typedef struct Action_t {
  ActionType type;
  Unit target;
  TilePoint targetTile;
  Point targetPoint;
  PVOID loadStartTarget;
  PVOID loadEndTarget;
} Action;

Action act_Get(Unit unit);