#pragma once
#include "EETwa.h"
#include "EETwaTypes.h"
#include "PartialStructs.h"
#include "InjectUtilities.h"

typedef struct Circle_t {
  Point p;
  float radius;
} Circle;

PVOID geom_GetClosestUnitFrom(PVOID unit, int8_t player, uint8_t (*filter)(PVOID));
uint8_t geom_IsPointInCircle(Point p, Circle c);

