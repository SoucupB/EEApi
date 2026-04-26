#pragma once
#include "EETypes.h"

typedef struct Point_t {
  float x;
  float y;
} Point;

typedef struct TilePoint_t {
  int32_t x;
  int32_t y;
} TilePoint;

typedef struct Circle_t {
  Point p;
  float radius;
} Circle;

Unit geom_GetClosestUnitFrom(Unit unit, int8_t player, uint8_t (*filter)(Unit));
uint8_t geom_IsPointInCircle(Point p, Circle c);
TilePoint geom_Tile_FromPoint(Point target);
