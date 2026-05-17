#include "Geometry.h"
#include <cmath>
#include <float.h>
#include <iterator>
#include "EETwa.h"
#include "Unit.h"
#include "Player.h"

float distancef(Point a, Point b) {
  return fabs(a.x - b.x) + fabs(a.y - b.y);
}

float distanceEuclidf(Point a, Point b) {
  return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Point geom_Point_Invalid() {
  return (Point) {
    .x = -10000.0f,
    .y = -10000.0f
  };
}

uint8_t geom_Point_IsInvalid(Point point) {
  return point.x == -10000.0f && point.y == -10000.0f;
}

Unit geom_GetClosestUnitFrom(Unit unit, Player ply, uint8_t (*filter)(Unit)) {
  vector<Unit> units = unit_Player_GetUnits(ply);
  float minDist = FLT_MAX;
  Unit selectedUnit;
  selectedUnit._payload = NULL;
  Point unitPos = unit_Point_Position(unit);
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(!filter(units[i])) {
      continue;
    }
    Point currentPosition = unit_Point_Position(units[i]);
    float distance = distancef(currentPosition, unitPos);
    if(minDist > distance) {
      minDist = distance;
      selectedUnit = units[i];
    }
  }
  return selectedUnit;
}

uint8_t geom_IsPointInCircle(Point p, Circle c) {
  return distanceEuclidf(p, c.p) <= c.radius;
}

TilePoint geom_Tile_FromPoint(Point target) {
  return (TilePoint) {
    .x = (int32_t)target.x,
    .y = (int32_t)target.y
  };
}