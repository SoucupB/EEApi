#include <math.h>
#include "PlayerState.h"
#include "MethodsDefinitions.h"
#include "Unit.h"
#include "LibManager.h"
#include "EETypes.h"
#include <map>
#include <algorithm>

void test_PrintUnits();
void test_ConvertEnemy();
void printPositions();
void testCastSomeSpell(Unit unit, Point pos);
void randomChecker();
void replaceCoc(Unit unit, Point pos);
Unit getProphet();
Unit getPriest();
void printAllUnitTypes();

// I think I find the water tile.
void pulas() {
  Unit currentPriest = getPriest();
  if(!eeTa_Unit_Reference(currentPriest)) {
    return ;
  }
  eeTa_FilePrintf("Is water for %p tile %d\n", currentPriest, eeTa_Tile_IsWater(eeTa_Unit_TilePosition(currentPriest)));
}

void onLosingHealth(Unit unit) {
  eeTa_FilePrintf("Unit %p taking damage\n", eeTa_Unit_Reference(unit));
}

__declspec(dllexport) void printAllTiles() {
  Unit currentPriest = getPriest();
  if(!eeTa_Unit_Reference(currentPriest)) {
    return ;
  }
  eeTa_FilePrintf("Current plane ID %d\n", unit_GetPlaneID(currentPriest));
  // pulas();
  // eeTa_Map_PrintBitMap();
}

void execDataPengus() {
  if(GetAsyncKeyState('J') & 0x8000) {
    test_PrintUnits();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('P') & 0x8000) {
    test_ConvertEnemy();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('F') & 0x8000) {
    randomChecker();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('T') & 0x8000) {
    // printAllTiles();
    // printAllUnitTypes();
    Beep (300, 250);
  }
}

void test_PrintUnits() {
  vector<Unit> units = unit_Player_GetUnits(ply_Null());
  if(units.size()) {
    for(int32_t i = 0; i < units.size(); i++) {
      Point currentPoint = eeTa_CurrentPosition(units[i]);
      eeTa_FilePrintf("Unit pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p\n", units[i]._payload, 
                      eeTa_UnitType(units[i]), eeTa_Player(units[i]), currentPoint.x, currentPoint.y, eeTypes_UnitClass(eeTa_EETypes_UnitType(units[i])));
    }
  }
  vector<Unit> buildings = unit_Player_GetBuildings(ply_Null());
  if(buildings.size()) {
    for(int32_t i = 0; i < buildings.size(); i++) {
      Point currentPoint = eeTa_CurrentPosition(buildings[i]);
      eeTa_FilePrintf("Building pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p\n", 
                      buildings[i]._payload, eeTa_UnitType(buildings[i]), eeTa_Player(buildings[i]), currentPoint.x, currentPoint.y, eeTypes_UnitClass(eeTa_EETypes_UnitType(buildings[i])));
    }
  }
}

Unit getPriest() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_UnitType(units[i]) == PRIEST) {
      return units[i];
    }
  }
  return eeTa_Unit_Null();
}

Unit getProphet() {
  vector<Unit> units = eeTa_Units(eeTa_SelfPlayer());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_UnitType(units[i]) == PROPHET) {
      return units[i];
    }
  }
  return eeTa_Unit_Null();
}

Unit getEnemy() {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  for(size_t i = 0; i < units.size(); i++) {
    if(eeTa_Player(units[i]) != eeTa_SelfPlayer()) {
      return units[i];
    }
  }
  return eeTa_Unit_Null();
}

Unit getEnemyBuilding() {
  vector<Unit> buildings = eeTa_Buildings(eeTa_AllPlayers());
  for(size_t i = 0; i < buildings.size(); i++) {
    if(eeTa_Player(buildings[i]) != eeTa_SelfPlayer()) {
      return buildings[i];
    }
  }
  return eeTa_Unit_Null();
}

void printPositions() {
  vector<Unit> units = eeTa_Units(eeTa_AllPlayers());
  for(size_t i = 0; i < units.size(); i++) {
    Point pnt = eeTa_GetDestinationCommand(units[i]);
    if(pnt.x != -1) {
      eeTa_FilePrintf("Position for %p is X: %f, Y: %f\n", eeTa_Unit_Reference(units[i]), pnt.x, pnt.y);
    }
  }
}

uint8_t isUnitChar(char element) {
  return isdigit(element) || isalpha(element);
}

uint8_t upperCase(char element) {
  if(element >= 'a' && element <= 'z') {
    return element - ('a' - 'A');
  }
  return element;
}

char *tokenizeUnitName(char *unitName) {
  size_t cnt = strlen(unitName);
  char *newChar = (char *)malloc(cnt);
  size_t index = 0;
  uint8_t lowerBar = 0;
  for(size_t i = 0; i < cnt; i++) {
    if(!i && isdigit(unitName[i])) {
      newChar[index++] = 'A';
      newChar[index++] = upperCase(unitName[i]);
      lowerBar = 0;
      continue;
    }
    if(isUnitChar(unitName[i])) {
      newChar[index++] = upperCase(unitName[i]);
      lowerBar = 0;
      continue;
    }
    if(!lowerBar) {
      lowerBar = 1;
      newChar[index++] = '_';
    }
  }
  newChar[index] = '\0';
  return newChar;
}

char *getNumber(PVOID addr) {
  return tokenizeUnitName((char *)*(size_t *)((size_t)addr + 0x1C));
}

size_t unitTypeID(PVOID addr) {
  return *(size_t *)((size_t)addr + 0x1E4);
}

size_t neutralUnitTypeID(PVOID addr) {
  return *(size_t *)((size_t)addr + 0x260);
}

void createClasses(map<size_t, vector<size_t> > &unitsChecker, char *className, char *classVariable, char *unitTypeDiff) {
  eeTa_FilePrintf("enum %s {\n", className);
  for(auto &it : unitsChecker) {
    eeTa_FilePrintf("  CLASS_%p,\n", (PVOID)it.first);
  }
  eeTa_FilePrintf("};\n\n");
  eeTa_FilePrintf("map<%s, vector<%s> > %s = {\n", className, unitTypeDiff, classVariable);
  for(auto &it : unitsChecker) {
    eeTa_FilePrintf(" {CLASS_%p, {", (PVOID)it.first);
    for(size_t i = 0; i < it.second.size(); i++) {
      if(i != it.second.size() - 1) {
        eeTa_FilePrintf("%s, ", getNumber((PVOID)it.second[i]));
      }
      else {
        eeTa_FilePrintf("%s", getNumber((PVOID)it.second[i]));
      }
    }
    eeTa_FilePrintf("}},\n");
  }
  eeTa_FilePrintf("};\n");
}

__declspec(dllexport) void printAllUnitTypes() {
  size_t *startingPointer = (size_t *)((size_t)lib_BaseAddress() + 0x5636B0);
  int32_t total = 10000;
  map<size_t, vector<size_t> > unitsClass;
  map<size_t, vector<size_t> > specialUnitsClass;
  vector< pair<char *, size_t> > unitTypes;
  vector< pair<char *, size_t> > specialUnitTypes;
  while(total--) {
    uint8_t valid;
    if(!builder_IsMemoryValid((PVOID)startingPointer)) {
      continue;
    }
    size_t unitType = *(size_t *)startingPointer;
    startingPointer++;
    if(!unitType) {
      continue;
    }
    size_t bufferSize = builder_BufferSize((PVOID)unitType, &valid);
    if(!valid || bufferSize != 0x32C) {
      continue;
    }
    int32_t type = unitTypeID((PVOID)unitType);
    if(type != 0x186A0) {
      unitsClass[*(size_t *)unitType].push_back(unitType);
      unitTypes.push_back(make_pair(getNumber((PVOID)unitType), type));
    }
    else {
      specialUnitsClass[*(size_t *)unitType].push_back(unitType);
      specialUnitTypes.push_back(make_pair(getNumber((PVOID)unitType), neutralUnitTypeID((PVOID)unitType)));
    }
  }
  sort(unitTypes.begin(), unitTypes.end(), [](const std::pair<char*, size_t>& a, const std::pair<char*, size_t>& b) {
    return strcmp(a.first, b.first) < 0;
  });
  sort(specialUnitTypes.begin(), specialUnitTypes.end(), [](const std::pair<char*, size_t>& a, const std::pair<char*, size_t>& b) {
    return strcmp(a.first, b.first) < 0;
  });
  eeTa_FilePrintf("enum UnitTypeDef {\n");
  for(size_t i = 0; i < unitTypes.size(); i++) {
    eeTa_FilePrintf("   %s = 0x%p,\n", unitTypes[i].first, unitTypes[i].second);
  }
  eeTa_FilePrintf("};\n\n");
  
  eeTa_FilePrintf("enum NeutralUnitType {\n");
  for(size_t i = 0; i < specialUnitTypes.size(); i++) {
    eeTa_FilePrintf("   %s = 0x%p,\n", specialUnitTypes[i].first, specialUnitTypes[i].second);
  }
  eeTa_FilePrintf("};\n\n");
  createClasses(unitsClass, (char *)"ClassTypeDef", (char *)"classDefUnits", (char *)"UnitTypeDef");
  createClasses(specialUnitsClass, (char *)"NeutralClassType", (char *)"neutralClassDefUnits", (char *)"NeutralUnitType");
}

void test_ConvertEnemy() {
  Unit priest = getPriest();
  Unit enemy = getEnemy();
  if(!eeTa_Unit_Reference(priest) || !eeTa_Unit_Reference(enemy)) {
    eeTa_FilePrintf("Not found\n");
    return ;
  }
  eeTa_Unit_Convert(priest, enemy);
  eeTa_FilePrintf("Rombububico\n");
}

__declspec(dllexport) void randomChecker() {
  Unit priest = getProphet();
  if(!priest._payload) {
    return ;
  }
  Unit building = getEnemyBuilding();
  if(!building._payload) {
    return ;
  }
  eeTa_Unit_CastPoint(priest, eeTa_CurrentPosition(building), PROPHET_EARTHQUAKE);
  eeTa_FilePrintf("Moved\n");
}

void bt_OnInit() {
  eeTa_FilePrintf("Changing at %p\n", (size_t)GetModuleHandleA("EE-AOC.exe") + (size_t)0xBB9D8);
}

void bt_OnFrame() {
  execDataPengus();
  pls_OnInit((PVOID)onLosingHealth);
}

void bt_OnUnitDestroy(Unit unit) {

}