#include "SimpleUnit.h"
#include "Game.h"

vector<SimpleUnit> su_Filter(uint8_t (*method)(SimpleUnit)) {
  vector<SimpleUnit> units;
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **simpleUnitPresence = eeTwa->simpleUnitPresence;
  for(size_t i = 0; i < 20; i++) {
    for(auto &it : *(simpleUnitPresence[i])) {
      Unit unit = (Unit) {
        ._payload = it.first
      };
      if(method(units[i])) {
        units.push_back(units[i]);
      }
    }
  }
  return units;
}