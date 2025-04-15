#include "EETwaTypes.h"
#include <unordered_map>

using namespace std;

static unordered_map<size_t, uint8_t> navalUnits;
static unordered_map<size_t, uint8_t> airUnits;
static unordered_map<size_t, uint8_t> transportUnits;

void eeTypes_InitNavalUnits() {
  size_t unitTypes[] = {
    UNIT_STONE_FISING_BOAT,
    UNIT_STONE_TRANSPORT,
    UNIT_STONE_FRIGATE,
    UNIT_COPPER_FISING_BOAT,
    UNIT_COPPER_TRANSPORT,
    UNIT_COPPER_BATTLESHIP,
    UNIT_COPPER_FRIGATE,
    UNIT_COPPER_GALLEY,
    UNIT_BRONZE_FISING_BOAT,
    UNIT_BRONZE_TRANSPORT,
    UNIT_BRONZE_BATTLESHIP,
    UNIT_BRONZE_FRIGATE,
    UNIT_BRONZE_GALLEY,
    UNIT_DARK_TRANSPORT,
    UNIT_DARK_FISING_BOAT,
    UNIT_DARK_FISING_BATTLESHIP,
    UNIT_DARK_FISING_FRIGATE,
    UNIT_DARK_GALLEY,
    UNIT_MIDDLE_FISING_BOAT,
    UNIT_MIDDLE_TRANSPORT,
    UNIT_MIDDLE_BATTLESHIP,
    UNIT_MIDDLE_FRIGATE,
    UNIT_MIDDLE_GALLEY,
    UNIT_RENA_TRANSPORT,
    UNIT_RENA_FISING_BOAT,
    UNIT_RENA_BATTLESHIP,
    UNIT_RENA_FRIGATE,
    UNIT_RENA_GALLEY,
    UNIT_IMPERIAL_TRANSPORT,
    UNIT_IMPERIAL_FISING_BOAT,
    UNIT_IMPERIAL_BATTLESHIP,
    UNIT_IMPERIAL_FRIGATE,
    UNIT_IMPERIAL_GALLEY,
    UNIT_INDUSTRIAL_TRANSPORT,
    UNIT_INDUSTRIAL_FISING_BOAT,
    UNIT_INDUSTRIAL_BATTLESHIP,
    UNIT_INDUSTRIAL_FRIGATE,
    UNIT_INDUSTRIAL_GALLEY,
    UNIT_INDUSTRIAL_CRUISER,
    UNIT_WW1_TRANSPORT,
    UNIT_WW1_FISING_BOAT,
    UNIT_WW1_BATTLESHIP,
    UNIT_WW1_FRIGATE,
    UNIT_WW1_CRUISER,
    UNIT_WW2_TRANSPORT,
    UNIT_WW2_FISING_BOAT,
    UNIT_WW2_BATTLESHIP,
    UNIT_WW2_FRIGATE,
    UNIT_WW2_CRUISER,
    UNIT_WW2_SUBMARIN,
    UNIT_WW2_CARRIER,
    UNIT_WWM_TRANSPORT,
    UNIT_WWM_FISING_BOAT,
    UNIT_WWM_BATTLESHIP,
    UNIT_WWM_FRIGATE,
    UNIT_WWM_CRUISER,
    UNIT_WWM_SUBMARIN,
    UNIT_WWM_CARRIER,
    UNIT_DIGITAL_TRANSPORT,
    UNIT_DIGITAL_FISING_BOAT,
    UNIT_DIGITAL_BATTLESHIP,
    UNIT_DIGITAL_FRIGATE,
    UNIT_DIGITAL_CRUISER,
    UNIT_DIGITAL_PORT_NAVAL,
    UNIT_NANO_TRANSPORT,
    UNIT_NANO_FISING_BOAT,
    UNIT_NANO_BATTLESHIP,
    UNIT_NANO_FRIGATE,
    UNIT_NANO_CRUISER,
    UNIT_NANO_PORT_NAVAL,
    UNIT_SPACE_CAPITAL_SHIP,
    UNIT_SPACE_SPACE_CARRIER,
    UNIT_SPACE_CAPITAL_TRANSPORT,
    UNIT_SPACE_CAPITAL_CORVETTE,
  };

  for(int32_t i = 0, c = sizeof(unitTypes) / sizeof(size_t); i < c; i++) {
    navalUnits[unitTypes[i]] = 1;
  }
}

uint8_t eeTypes_IsFishBoat(UnitTypeDef type) {
  size_t unitTypes[] = {
    UNIT_STONE_FISING_BOAT,
    UNIT_COPPER_FISING_BOAT,
    UNIT_BRONZE_FISING_BOAT,
    UNIT_DARK_FISING_BOAT,
    UNIT_MIDDLE_FISING_BOAT,
    UNIT_RENA_FISING_BOAT,
    UNIT_IMPERIAL_FISING_BOAT,
    UNIT_INDUSTRIAL_FISING_BOAT,
    UNIT_WW1_FISING_BOAT,
    UNIT_WW2_FISING_BOAT,
    UNIT_WWM_FISING_BOAT,
    UNIT_DIGITAL_FISING_BOAT,
    UNIT_NANO_FISING_BOAT,
  }; 
  for(int8_t i = 0, c = sizeof(unitTypes) / sizeof(size_t); i < c; i++) {
    if(unitTypes[i] == type) {
      return 1;
    }
  }
  return 0;
}

void eeTypes_InitAirUnits() {
  size_t unitTypes[] = {
    UNIT_WW1_LIGHTER_BOMBER,
    UNIT_WW1_SOPWITH,
    UNIT_WW1_GOTHA_BOMBER,
    UNIT_WW1_BALOON,
    UNIT_WW2_M246_LIGHTER_BOMBER,
    UNIT_WW2_SOPWITH,
    UNIT_WW2_M1_BOMBER,
    UNIT_WW2_B29_BOMBER,
    UNIT_WW2_ANTI_TANK,
    UNIT_WW2_BALOON,
    UNIT_WWM_M246_LIGHTER_BOMBER,
    UNIT_WWM_LIGHTER,
    UNIT_WWM_B_2_BOMBER,
    UNIT_WWM_B52_BOMBER,
    UNIT_WWM_ANTI_TANK,
    UNIT_WWM_SEE_KING,
    UNIT_WW2_CARRIER_PLANE,
    UNIT_WWM_HELICOPTER,
    UNIT_WWM_HELICOPTER_ANTI_TANK,
    UNIT_WWM_HELICOPTER_GUNSHIP,
    UNIT_WWM_CARRIER_PLANE,
    UNIT_DIGITAL_TALON_LIGHTER_BOMBER,
    UNIT_DIGITAL_JACKAL,
    UNIT_DIGITAL_WIVERN,
    UNIT_DIGITAL_TITAN_BOMBER,
    UNIT_DIGITAL_ANTI_TANK,
    UNIT_DIGITAL_SEE_KING,
    UNIT_DIGITAL_PEGASUS,
    UNIT_DIGITAL_SPECTRE,
    UNIT_DIGITAL_REAPER,
    UNIT_DIGITAL_ARES,
    UNIT_DIGITAL_APOLLO,
    UNIT_DIGITAL_NAVAL_BOMBER,
    UNIT_NANO_PHOENIX,
    UNIT_NANO_NEBULA,
    UNIT_NANO_WIVERN,
    UNIT_NANO_TITAN_BOMBER,
    UNIT_NANO_ANTI_TANK,
    UNIT_NANO_SEE_KING_2,
    UNIT_NANO_PEGASUS,
    UNIT_NANO_SPECTRE,
    UNIT_NANO_REAPER,
    UNIT_NANO_ARES_2,
    UNIT_NANO_APOLLO,
    UNIT_NANO_NAVAL_BOMBER,
    UNIT_SPACE_PANE
  };

  for(int32_t i = 0, c = sizeof(unitTypes) / sizeof(size_t); i < c; i++) {
    airUnits[unitTypes[i]] = 1;
  }
}

void eeTypes_InitTransportUnits() {
  size_t unitTypes[] = {
    UNIT_STONE_TRANSPORT,
    UNIT_BRONZE_TRANSPORT,
    UNIT_COPPER_TRANSPORT,
    UNIT_DARK_TRANSPORT,
    UNIT_MIDDLE_TRANSPORT,
    UNIT_RENA_TRANSPORT,
    UNIT_IMPERIAL_TRANSPORT,
    UNIT_INDUSTRIAL_TRANSPORT,
    UNIT_WW1_TRANSPORT,
    UNIT_WW2_TRANSPORT,
    UNIT_WWM_TRANSPORT,
    UNIT_WWM_HELICOPTER,
    UNIT_DIGITAL_TRANSPORT,
    UNIT_DIGITAL_PEGASUS,
    UNIT_NANO_TRANSPORT,
    UNIT_NANO_PEGASUS,
    UNIT_SPACE_CAPITAL_TRANSPORT
  };

  for(int32_t i = 0, c = sizeof(unitTypes) / sizeof(size_t); i < c; i++) {
    transportUnits[unitTypes[i]] = 1;
  }
}

uint8_t eeTypes_IsTransport(UnitTypeDef type) {
  if(transportUnits.find(type) != transportUnits.end()) {
    return 1;
  }
  return 0;
}

uint8_t eeTypes_IsAir(UnitTypeDef type) {
  if(airUnits.find(type) != airUnits.end()) {
    return 1;
  }
  return 0;
}

uint8_t eeTypes_IsWater(UnitTypeDef type) {
  if(navalUnits.find(type) != navalUnits.end()) {
    return 1;
  }
  return 0;
}

uint8_t eeTypes_IsGround(UnitTypeDef type) {
  return !(eeTypes_IsAir(type) || eeTypes_IsWater(type));
}

void eeTypes_OnInit() {
  eeTypes_InitTransportUnits();
  eeTypes_InitNavalUnits();
  eeTypes_InitAirUnits();
}

uint8_t eeTypes_IsWorker(UnitTypeDef type) {
  return type == UNIT_FEMALE_CITIZEN ||
         type == UNIT_MALE_CITIZEN;
}

uint8_t eeTypes_CanProduceWorkers(UnitTypeDef type) {
  return type == CAPITOL ||
         type == TOWN_CENTER;
}