#include "Geometry.h"
#include <cmath>
#include <float.h>
#include <iterator>

float distancef(Point a, Point b) {
  return fabs(a.x - b.x) + fabs(a.y - b.y);
}

float distanceEuclidf(Point a, Point b) {
  return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

PVOID geom_GetClosestUnitFrom(PVOID unit, int8_t player, uint8_t (*filter)(PVOID)) {
  vector<Unit> units = eeTa_Units(player);
  float minDist = FLT_MAX;
  PVOID selectedUnit = NULL;
  Point unitPos = eeTa_CurrentPosition((Unit) {._payload = unit});
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(!filter(units[i]._payload)) {
      continue;
    }
    Point currentPosition = eeTa_CurrentPosition(units[i]);
    float distance = distancef(currentPosition, unitPos);
    if(minDist > distance) {
      minDist = distance;
      selectedUnit = units[i]._payload;
    }
  }
  return selectedUnit;
}

uint8_t geom_IsPointInCircle(Point p, Circle c) {
  return distanceEuclidf(p, c.p) <= c.radius;
}