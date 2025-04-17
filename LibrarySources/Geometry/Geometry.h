#pragma once
#include "EETwa.h"
#include "EETwaTypes.h"
#include "PartialStructs.h"
#include "InjectUtilities.h"

typedef struct Circle_t {
  Point p;
  float radius;
} Circle;

Unit geom_GetClosestUnitFrom(Unit unit, int8_t player, uint8_t (*filter)(Unit));
uint8_t geom_IsPointInCircle(Point p, Circle c);

