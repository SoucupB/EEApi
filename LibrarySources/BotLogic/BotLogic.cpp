#include "BotLogic.h"
#include <math.h>
#include "AttackDecision.h"
#include "PlayerState.h"
#include "ResourceManager.h"

__declspec(dllexport) PVOID fakeStdin;

void cleanDataChecker() {
  if(GetAsyncKeyState('J') & 0x8000) {
    eeTa_Clean();
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
  UnitTypeDef def = eeTa_UnitType(unit);
  return !eeTypes_IsWorker(def);
}

uint8_t workersFilter(Unit unit) {
  UnitTypeDef def = eeTa_UnitType(unit);
  return eeTypes_IsWorker(def);
}

int8_t shouldBuildAttackUnit() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  vector<Unit> filteredUnits = eeTa_Filter(units, nonWorkersFilter);
  size_t currentPop = 0;
  for(size_t i = 0, c = filteredUnits.size(); i < c; i++) {
    currentPop += eeTa_UnitPopulation(filteredUnits[i]);
  }
  return (float)currentPop < (float)eeTa_TotalPop() * 0.6f;
}

int8_t shouldBuildWorkers() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
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
  vector<Unit> buildings = eeTa_IdleBuildings(eeTa_SelfPlayer());
  if(!buildings.size() || !shouldBuildAttackUnit()) {
    return ;
  }
  size_t *indexes = (size_t *)malloc(sizeof(size_t) * buildings.size());
  randomIndexing(indexes, buildings.size());
  int32_t maxBuildings = 4;
  for(int32_t i = 0, c = buildings.size(); i < c; i++) {
    Unit building = buildings[indexes[i]];
    if(eeTypes_CanProduceWorkers(eeTa_UnitType(building))) {
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
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
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
  UnitTypeDef def = eeTa_UnitType(unit);
  return eeTypes_CanProduceWorkers(def);
}

void bt_BuildWorkers(PVOID attr) {
  if(!shouldBuildWorkers()) {
    return ;
  }
  vector<Unit> buildings = eeTa_IdleBuildings(eeTa_SelfPlayer());
  vector<Unit> capitols = eeTa_Filter(buildings, capitolFilter);
  if(!capitols.size()) {
    return ;
  }
  int32_t maxBuildings = 3;
  for(int32_t i = 0, c = capitols.size(); i < c; i++) {
    Unit building = capitols[i];
    eeTa_BuildUnit(building, (PVOID)UNIT_MALE_CITIZEN);
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

void printBuildableTypes(PVOID unit) {
  vector<int32_t> types = eeTa_AllBuildableTypes((Unit) {._payload = unit});

  string response = "The pointer at " + convertToHex((int32_t)unit) + " can build ";

  for(int32_t i = 0; i < types.size(); i++) {
    response += convertToHex(types[i]) + " ";
  }

  eeTa_Printf("%s\n", &response[0]);
}

void getMetaData() {
  vector<PVOID> buildings = eeTa_Buildings(eeTa_SelfPlayer());

  if(fakeStdin) {
    printBuildableTypes(fakeStdin);
    return ;
  }

  for(int32_t i = 0; i < buildings.size(); i++) {
    if(eeTa_UnitType((Unit) {._payload = buildings[i]}) == BUILDING_BRONZE_ARCHERY) {
      printBuildableTypes(buildings[i]);
    }
  }
}

void checkEpochPointer() {
  if(GetAsyncKeyState('P') & 0x8000) {
    getMetaData();
    Beep (400, 250);
  }
}

void moveRandomly() {
  if(GetAsyncKeyState('O') & 0x8000) {
    vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
    if(!units.size()) {
      return ;
    }
    Point destCommand = eeTa_GetDestinationCommand(units[0]);
    eeTa_Printf("Dest for %p pos %f and %f\n", units[0], destCommand.x, destCommand.y);
  }
}

Point randomMove(PVOID unit) {
  Point currentPosition = eeTa_CurrentPosition((Unit) {._payload = unit});
  currentPosition.x += sinf(rand()) * 20.0f;
  currentPosition.y += sinf(rand()) * 20.0f;
  return currentPosition;
}

uint8_t impulseIdleUnit(PVOID unit) {
  if(!eeTa_IsUnitIdle((Unit) {._payload = unit})) {
    return 0;
  }
  Point pnt = randomMove(unit);
  if(pnt.x == -1) {
    return 0;
  }
  help_UnitMove(unit, pnt, UNIT_ATTACK);
  return 1;
}

void iterateTroughExistingUnits(PVOID attr) {
  int32_t maxCommands = 10;
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0, c = units.size(); i < c; i++) {
    UnitTypeDef def = eeTa_UnitType(units[i]);
    if(!eeTypes_IsWorker(def) && !eeTypes_IsTransport(def) && impulseIdleUnit(units[i]._payload)) {
      maxCommands--;
    }
    if(!maxCommands) {
      return ;
    }
  }
}

void buildRandomUnit() {
  if(GetAsyncKeyState('K') & 0x8000) {
    PVOID unit = eeTa_Unit_Sample(eeTa_SelfPlayer());
    if(!unit) {
      return ;
    }
    PVOID targetUnit = eeTa_Unit_Sample(0);
    help_UnitMove(unit, eeTa_CurrentPosition((Unit) {._payload = targetUnit}), UNIT_ATTACK);
    Beep (400, 250);
  }
}

void moveToResources() {
  if(GetAsyncKeyState('P') & 0x8000) {
    PVOID unit = eeTa_Unit_Sample(eeTa_SelfPlayer());
    PVOID target = eeTa_Unit_Sample(eeTa_NeutralPlayer());

    help_MoveToTarget(unit, target);
  }
}

void addListeners() {
  // cleanDataChecker();
  // checkEpochPointer();
  moveToResources();
  // moveRandomly();
}

void bt_HuntTransports(PVOID attributes) {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  att_AttackTransportWithNavals(units);
}

void bt_HuntWithPlanes(PVOID attributes) {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  att_AttackEnemiesWithPlanes(units);
}

void bt_OnFrame() {
  // addListeners();
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
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  att_PatrolRandomPositions(units);
}

void bt_InitUnitMovement() {
  TimeAtom atom;
  atom.method = (PVOID)patrolCommand;
  atom.arguments = NULL;
  atom.time = 1115;
  eeTa_AddFrameMethod(atom);
}

void bt_InitPlaneHunters() {
  TimeAtom atom;
  atom.method = (PVOID)bt_HuntWithPlanes;
  atom.arguments = NULL;
  atom.time = 4048;
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

void bt_OnUnitIteration(Unit unit) {
  pls_OnUnitIteration(unit);
}

void bt_OnUnitDestroy(Unit unit) {
  pls_OnUnitDestory(unit);
}

void bt_InitAggresiveness() {
  // eeTa_SetCvCAggression(2, 100.0f);
  // eeTa_Printf("ZUGA %d", eeTa_PlayerCount());
  int8_t *playerIndexes = eeTa_PlayerIDs();
  // int8_t playerInd = eeTa_PlayerIndex();
  for(size_t i = 0; i < 20; i++) {
    if(i != 1 && playerIndexes[i]) {
      eeTa_SetCvCAggression(i, 90.0f);
    }
  }

  // for(size_t i = 2, c = eeTa_PlayerCount(); i <= c; i++) {
  //   eeTa_SetCvCAggression(i, 100.0f);
  // }
  // int32_t zr = 0;
  // int8_t *cnt = eeTa_PlayerIDs();
  // for(size_t i = 0; i < 16; i++) {
  //   if(cnt[i]) {
  //     zr++;
  //   }
  // }
}

void bt_OnInit() {
  bt_InitUnitMovement();
  bt_InitBuildingCreation();
  bt_InitTransportHunting();
  bt_InitWorkerCreators();
  bt_InitPortNaval();
  bt_InitFisherBoats();
  bt_InitAggresiveness();
  pls_OnInit((PVOID)att_AddDamagedUnits);
  // bt_InitPlaneHunters();
}