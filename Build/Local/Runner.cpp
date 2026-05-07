#include <math.h>
#include "AttackDecision.h"
#include "PlayerState.h"
#include "ResourceManager.h"
#include "EETwa.h"
#include "Unit.h"
#include "EETypes.h"
#include "CitizenManager.h"

typedef struct SpawnLocation_t {
  TilePoint point;
  uint8_t player;
} SpawnLocation;

vector<SpawnLocation> capitolsLocations;

void att_LoadTransports(PVOID _);

void cleanDataChecker() {
  if(GetAsyncKeyState('J') & 0x8000) {
    Beep (300, 250);
  }
}

void randomIndexing(size_t *indexes, size_t sz) {
  for(size_t i = 0; i < sz; i++) {
    indexes[i] = i;
  }
  for(size_t i = 0; i < sz; i++) {
    size_t randomIndex = (rand() * rand()) % (sz - i) + i;
    swap(indexes[i], indexes[randomIndex]);
  }
}

uint8_t nonWorkersFilter(Unit unit) {
  UnitType def = unit_Type(unit);
  return !eeTypes_IsCitizen(def);
}

uint8_t workersFilter(Unit unit) {
  UnitType def = unit_Type(unit);
  return eeTypes_IsCitizen(def);
}

int8_t shouldBuildAttackUnit() {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  vector<Unit> filteredUnits = eeTa_Filter(units, nonWorkersFilter);
  size_t currentPop = 0;
  for(size_t i = 0, c = filteredUnits.size(); i < c; i++) {
    currentPop += eeTa_UnitPopulation(filteredUnits[i]);
  }
  return (float)currentPop < (float)eeTa_TotalPop() * 0.6f;
}

int8_t shouldBuildWorkers() {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  vector<Unit> filteredUnits = eeTa_Filter(units, workersFilter);
  size_t currentPop = 0;
  for(size_t i = 0, c = filteredUnits.size(); i < c; i++) {
    currentPop += eeTa_UnitPopulation(filteredUnits[i]);
  }
  return (float)currentPop < (float)eeTa_TotalPop() * 0.4f;
}

uint8_t navalAirCarrierFilter(Unit unit) {
  return att_IsUnitCarrier(unit);
}

void buildUnit_t(PVOID attr) {
  vector<Unit> buildings = unit_IdleBuildings(eeTa_SelfPlayer());
  if(!buildings.size() || !shouldBuildAttackUnit()) {
    return ;
  }
  size_t *indexes = (size_t *)malloc(sizeof(size_t) * buildings.size());
  randomIndexing(indexes, buildings.size());
  int32_t maxBuildings = 4;
  for(int32_t i = 0, c = buildings.size(); i < c; i++) {
    Unit building = buildings[indexes[i]];
    if(eeTypes_CanProduceWorkers(unit_Type(building))) {
      continue;
    }
    vector<int32_t> types = eeTa_AllBuildableTypes(building);
    if(!types.size()) {
      continue;
    }
    eeTa_BuildUnit(building, (PVOID)types[rand() % types.size()]);
    if(!maxBuildings) {
      break;
    }
    maxBuildings--;
  }
  free(indexes);
}

void buildAirCarrierUnits(PVOID attr) {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  vector<Unit> filteredUnits = eeTa_Filter(units, navalAirCarrierFilter);
  for(int32_t i = 0, c = filteredUnits.size(); i < c; i++) {
    Unit naval = filteredUnits[i];
    if(!eeTa_IsIdle(naval)) {
      continue;
    }
    vector<int32_t> types = eeTa_AllBuildableTypes(naval);
    if(!types.size()) {
      continue;
    }
    eeTa_BuildUnit(naval, (PVOID)types[rand() % types.size()]);
  }
}

uint8_t capitolFilter(Unit unit) {
  UnitType def = unit_Type(unit);
  return eeTypes_CanProduceWorkers(def);
}

__declspec(dllexport) void bt_BuildWorkers(PVOID attr) {
  if(!shouldBuildWorkers()) {
    return ;
  }
  vector<Unit> buildings = unit_IdleBuildings(eeTa_SelfPlayer());
  vector<Unit> capitols = eeTa_Filter(buildings, capitolFilter);
  if(!capitols.size()) {
    return ;
  }
  int32_t maxBuildings = 3;
  for(int32_t i = 0, c = capitols.size(); i < c; i++) {
    Unit building = capitols[i];
    eeTa_BuildUnit(building, (PVOID)CITIZEN);
    if(!maxBuildings) {
      break;
    }
    maxBuildings--;
  }
}

string convertToHex(int number) {
  char buffer[100] = {0};
  sprintf(buffer, "%x", number);
  return std::string(buffer);
}

void checkEpochPointer() {
  if(GetAsyncKeyState('P') & 0x8000) {
    Beep (400, 250);
  }
}

void convert() {
  if(GetAsyncKeyState('P') & 0x8000) {
    vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
    att_ConvertIfNecessary(units);
    Beep(400, 250);
  }
}

void bt_HuntTransports(PVOID attributes) {
  vector<Unit> units = unit_GetUnits(eeTa_AllPlayers());
  att_AttackTransportWithNavals(units);
}

void convert();

void bt_OnFrame() {
}

void bt_InitBuildingCreation() {
  TimeAtom atom;
  atom.method = (PVOID)buildUnit_t;
  atom.arguments = NULL;
  atom.time = 5048;
  eeTa_AddFrameMethod(atom);
}

void bt_InitTransportHunting() {
  TimeAtom atom;
  atom.method = (PVOID)bt_HuntTransports;
  atom.arguments = NULL;
  atom.time = 3548;
  eeTa_AddFrameMethod(atom);
}

void patrolCommand(PVOID attr) {
  vector<Unit> units = unit_GetUnits(eeTa_SelfPlayer());
  att_PatrolRandomPositions(units);
}

void bt_InitUnitMovement() {
  TimeAtom atom;
  atom.method = (PVOID)patrolCommand;
  atom.arguments = NULL;
  atom.time = 1115;
  eeTa_AddFrameMethod(atom);
}

void bt_InitWorkerCreators() {
  TimeAtom atom;
  atom.method = (PVOID)bt_BuildWorkers;
  atom.arguments = NULL;
  atom.time = 1928;
  eeTa_AddFrameMethod(atom);
}

void bt_InitPortNaval() {
  TimeAtom atom;
  atom.method = (PVOID)buildAirCarrierUnits;
  atom.arguments = NULL;
  atom.time = 1528;
  eeTa_AddFrameMethod(atom);
}

void startFishCommand(PVOID attr) {
  res_MoveResourceBoats();
}

void bt_InitFisherBoats() {
  TimeAtom atom;
  atom.method = (PVOID)startFishCommand;
  atom.arguments = NULL;
  atom.time = 2115;
  eeTa_AddFrameMethod(atom);
}

void bt_RepairBuildings() {
  TimeAtom atom;
  atom.method = (PVOID)citizen_RepairBuildings;
  atom.arguments = NULL;
  atom.time = 2450;
  eeTa_AddFrameMethod(atom);
}

void bt_ConvertUnits() {
  TimeAtom atom;
  atom.method = (PVOID)att_ProcessSpecialAbilityUnits;
  atom.arguments = NULL;
  atom.time = 2550;
  eeTa_AddFrameMethod(atom);
}

void bt_AttackWithBombers() {
  TimeAtom atom;
  atom.method = (PVOID)att_AttackWithBombers;
  atom.arguments = NULL;
  atom.time = 1432;
  eeTa_AddFrameMethod(atom);
}

void bt_AttackWithShipsEverywhere() {
  TimeAtom atom;
  atom.method = (PVOID)att_AttackWithShips;
  atom.arguments = NULL;
  atom.time = 25034;
  eeTa_AddFrameMethod(atom);
}

void bt_OnUnitIteration(Unit unit) {
  pls_OnUnitIteration(unit);
}

void bt_OnUnitDestroy(Unit unit) {
  pls_OnUnitDestory(unit);
}

void bt_InitAggresiveness() {
  int8_t *playerIndexes = eeTa_PlayerIDs();
  for(size_t i = 0; i < 20; i++) {
    if(i != 1 && playerIndexes[i]) {
      eeTa_SetCvCAggression(i, 50.0f);
    }
  }
}

SpawnLocation getSelfCapitol() {
  for(size_t i = 0; i < capitolsLocations.size(); i++) {
    if(capitolsLocations[i].player == eeTa_SelfPlayer()) {
      return capitolsLocations[i];
    }
  }

  return (SpawnLocation){
    .point = (TilePoint) {
      .x = 0,
      .y = 0
    },
    .player = 0xFF
  };
}

void att_LoadTransports(PVOID _) {
  SpawnLocation capitol = getSelfCapitol();
  eeTa_FilePrintf("DSADSADSADSA %d\n", capitol.player);
  if(capitol.player == 0xFF) {
    return ;
  }
  att_FillTransports(capitol.point);
}

void bt_LoadUnits() {
  TimeAtom atom;
  atom.method = (PVOID)att_LoadTransports;
  atom.arguments = NULL;
  atom.time = 16500;
  eeTa_AddFrameMethod(atom);
}

void initCapitolPositions() {
  vector<Unit> units = unit_GetBuildings(eeTa_AllPlayers());
  for(size_t i = 0; i < units.size(); i++) {
    if(!eeTypes_CanProduceWorkers(unit_Type(units[i]))) {
      continue;
    }
    capitolsLocations.push_back((SpawnLocation) {
      .point = unit_Tile_Position(units[i]),
      .player = unit_GetPlayerIndex(units[i])
    });
  }
}

void bt_OnInit() {
  initCapitolPositions();
  bt_InitUnitMovement();
  bt_InitBuildingCreation();
  bt_InitTransportHunting();
  bt_InitWorkerCreators();
  bt_InitPortNaval();
  bt_InitFisherBoats();
  bt_RepairBuildings();
  bt_ConvertUnits();
  bt_AttackWithBombers();
  bt_AttackWithShipsEverywhere();
  bt_LoadUnits();
  pls_OnInit((PVOID)att_AddDamagedUnits);
  // bt_InitPlaneHunters();
}