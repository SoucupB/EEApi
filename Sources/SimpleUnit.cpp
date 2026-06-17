#include "SimpleUnit.h"
#include "Game.h"
#include "Resource.h"

vector<SimpleUnit> su_Filter(uint8_t (*method)(SimpleUnit)) {
  vector<SimpleUnit> units;
  PEETwa eeTwa = game_EETwa();
  unordered_map<PVOID, uint8_t> **simpleUnitPresence = eeTwa->simpleUnitPresence;
  for(size_t i = 0; i < 20; i++) {
    for(auto &it : *(simpleUnitPresence[i])) {
      SimpleUnit unit = (SimpleUnit) {
        ._payload = it.first
      };
      if(method(unit)) {
        units.push_back(unit);
      }
    }
  }
  return units;
}

PVOID su_Reference(SimpleUnit unit) {
  return unit._payload;
}

NeutralUnitType su_Type(SimpleUnit unit) {
  return res_Type((Resource) {
    ._payload = su_Reference(unit)
  });
}

SimpleUnit su_FromPayload(PVOID unit) {
  return (SimpleUnit) {
    ._payload = unit
  };
}

SimpleUnit su_Null() {
  return (SimpleUnit) {
    ._payload = NULL
  };
}