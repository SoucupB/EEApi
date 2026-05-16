#include <math.h>
#include "PlayerState.h"
#include "MethodsDefinitions.h"
#include "Unit.h"
#include "LibManager.h"
#include "EETypes.h"
#include <map>
#include <algorithm>
#include "Resource.h"
#include "Player.h"
#include "Ability.h"

void test_PrintUnits();
void printSpells();
void printTechNodes();

// I think I find the water tile.
void onLosingHealth(Unit unit) {
  eeTa_FilePrintf("Unit %p taking damage\n", unit_Reference(unit));
}

void test_PrintUnits() {
  vector<Unit> units = unit_Player_GetUnits(ply_Null());
  if(units.size()) {
    for(int32_t i = 0; i < units.size(); i++) {
      Point currentPoint = unit_Point_Position(units[i]);
      eeTa_FilePrintf("Unit pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p with hp %d and range %f\n", units[i]._payload, 
                      unit_Type(units[i]), eeTa_Player(units[i]), currentPoint.x, currentPoint.y, 
                      eeTypes_UnitClass(unit_Type(units[i])), unit_TotalHP(units[i]), unit_Range(units[i]));
    }
  }
  vector<Unit> buildings = unit_Player_GetBuildings(ply_Null());
  if(buildings.size()) {
    for(int32_t i = 0; i < buildings.size(); i++) {
      Point currentPoint = unit_Point_Position(buildings[i]);
      eeTa_FilePrintf("Building pointer: %p, unit type: %p unit team %d, position: (%f, %f) class %p with hp %d\n", 
                      buildings[i]._payload, unit_Type(buildings[i]), 
                      eeTa_Player(buildings[i]), currentPoint.x, currentPoint.y, eeTypes_UnitClass(unit_Type(buildings[i])), unit_TotalHP(buildings[i]));
    }
  }
}

void execDataPengus() {
  if(GetAsyncKeyState('J') & 0x8000) {
    test_PrintUnits();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('P') & 0x8000) {
    // test_ConvertEnemy();
    ply_Print();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('F') & 0x8000) {
    // repairBuildings();
    // farmUnit();
    // farmFish();
    // castEarthquake();
    // convertUnit();
    // loadTransport();
    printTechNodes();
    Beep (300, 250);
  }
  if(GetAsyncKeyState('T') & 0x8000) {
    // printTransport();
    printSpells();
    Beep (300, 250);
  }
}

PVOID techNode(TechTree tree, Ability ability) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0x18A4);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(ply_TechTree_Ref(tree), 
         (PVOID)ability);
}

char *techNodeTextureName(PVOID techNode) {
  PVOID textureRef = (PVOID)*(size_t *)((size_t)techNode + 0x10);

  return (char *)*(size_t *)((size_t)textureRef + 0x8);
}

size_t findCallerIndex(PVOID cTechNodes) {
  return *(size_t *)((*(size_t *)((size_t)cTechNodes + 0x8)) + 0x44);
}

void printTechNodes() {
  TechTree tree = ply_TechTree(ply_Self());
  PVOID cTechNode = techNode(tree, PROPHET_TORNADO);
  eeTa_FilePrintf("Tech node is %p, name is %s\n", cTechNode, techNodeTextureName(cTechNode));
}

PVOID getAbilityManager() {
  return (PVOID)*(size_t *)((size_t)lib_BaseAddress() + 0x4E5FB0);
}

PVOID abilityPointer(PVOID manager, size_t abilityIndex) {
  PVOID methodStruct = (PVOID)((size_t)lib_BaseAddress() + 0xAE71D);
  PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)methodStruct);
  return method(manager, 
         (PVOID)&abilityIndex);
}

PVOID getAbilityInstance(Ability ability) {
  TechTree tree = ply_TechTree(ply_Self());
  PVOID cTechNode = techNode(tree, ability);
  if(!cTechNode) {
    return 0;
  }
  size_t abilityIndex = findCallerIndex(cTechNode);
  PVOID abilityInstance = abilityPointer(getAbilityManager(), abilityIndex);
  return abilityInstance;
}

int32_t abilityEnergy(Ability ability) {
  PVOID reference = getAbilityInstance(ability);
  if(!reference) {
    return 0;
  }
  PVOID energyMethod = (PVOID)*(size_t *)(*(size_t *)reference + 0x10);
  PVOID __thiscall (*method)(PVOID) = (PVOID __thiscall (*)(PVOID)) ((uint8_t *)energyMethod);
  return (int32_t)method(reference);
}

char *abilityName(char *textureName) {
  const size_t strSize = strlen(textureName);
  char *response = (char *)malloc(strSize + 1);
  size_t k = 0;
  memset(response, 0, strSize + 1);
  for(size_t i = 0; i < strSize; i++) {
    if(textureName[i] >= 'a' && textureName[i] <= 'z') {
      response[k++] = textureName[i] - ('a' - 'A');
      continue;
    }
    if(textureName[i] >= '0' && textureName[i] <= '9') {
      response[k++] = textureName[i];
      continue;
    }
    if(!(textureName[i] >= 'A' && textureName[i] <= 'Z')) {
      if(k && response[k - 1] == '_') {
        continue;
      }
      response[k++] = '_';
      continue;
    }
    response[k++] = textureName[i];
  }
  return response;
}

void printSpells() {
  PEETypes types = game_GetEETypes();
  map<UnitType, vector<Ability> > *abilityPointers = types->abilityPointers;
  TechTree tree = ply_TechTree(ply_Self());
  for(auto &it : *abilityPointers) {
    if(!it.second.size() || eeTypes_IsBuilding(it.first)) {
      continue;
    }
    eeTa_FilePrintf("Unit %p with %s ", it.first, eeTypes_Name(it.first));
    for(size_t i = 0; i < it.second.size(); i++) {
      eeTa_FilePrintf("Cast: %p Template: %p ", it.second[i], eeTypes_GetTemplate((UnitType)it.second[i]));
    }
    eeTa_FilePrintf("\n");
  }
  for(auto &it : *abilityPointers) {
    if(!it.second.size() || eeTypes_IsBuilding(it.first)) {
      continue;
    }
    for(size_t i = 0; i < it.second.size(); i++) {
      PVOID cTechNode = techNode(tree, it.second[i]);
      if(!cTechNode) {
        continue;
      }
      eeTa_FilePrintf("Spell: %p, node %p, index is %p, instance: %p, energy: %d, name: '%s'\n", 
        it.second[i], cTechNode, findCallerIndex(cTechNode), getAbilityInstance(it.second[i]), ability_Energy(it.second[i]), abilityName(techNodeTextureName(cTechNode)));
    }
  }
}

void bt_OnInit() {
  printSpells();
  eeTa_FilePrintf("Spookly\n");
}

void bt_OnFrame() {
  execDataPengus();
}

void bt_OnUnitDestroy(Unit unit) {

}