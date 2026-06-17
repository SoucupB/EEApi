#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "InjectUtilities.h"
#include "MethodsDefinitions.h"
#include "LibManager.h"
#include "EETwa.h"
#include "Driver.h"
#include "Resource.h"
#include "ResourcePrivate.h"
#include "EETypesStructPrivate.h"
#include "EETwaPrivate.h"
#include "Offset.h"
#include "MethodsDefinitions.h"
#include "Game.h"
#include "CheckSum.h"

static size_t mapPolygons;
void eeTa_RebuildExtraDataStructure();
void eeTa_RebuildDTs();
void rebuildDataStructures();

extern "C" {
  __declspec(dllexport) int32_t __thiscall onUnitIteration(PVOID self) {
    int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + REMOTE_METHOD_ON_UNIT_ITERATION);
    PEmpireEarthHook hook = game_EmpHook();
    eeTa_OnUnitFrame((Unit) {
      ._payload = self
    });
    hook->hasIterationBeenExecuted = 1;
    return method(self);
  }

  __declspec(dllexport) int32_t __thiscall onFrame(PVOID self) {
    int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + REMOTE_METHOD_ON_FRAME);
    PEmpireEarthHook hook = game_EmpHook();
    if(!hook->hasIterationBeenExecuted) {
      return method(self);
    }
    if(!hook->onInitFlag) {
      if(!eeTa_ShouldOnInitExecute()) {
        return method(self);
      }
      eeTa_OnInit();
      hook->onInitFlag = 1;
      return method(self);
    }
    eeTa_OnFrame();
    return method(self);
  }

  __declspec(dllexport) int32_t __thiscall onUnitDeath(PVOID self, PVOID _a, PVOID _b, PVOID _c) {
    int32_t __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (int32_t __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)lib_BaseAddress() + REMOTE_METHOD_ON_UNIT_DEATH);
    eeTa_OnUnitDeath((Unit) {
      ._payload = self
    });
    return method(self, _a, _b, _c);
  }

  __declspec(dllexport) int32_t __thiscall onPlanePark(PVOID self) {
    int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + REMOTE_METHOD_ON_PLANE_PARK);
    eeTa_OnUnitDeath((Unit) {
      ._payload = self
    });
    return method(self);
  }

  __declspec(dllexport) int32_t __fastcall onUnitDelete(PVOID self) {
    int32_t __fastcall (*method)(PVOID) = (int32_t __fastcall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + REMOTE_METHOD_ON_UNIT_DELETE);
    eeTa_OnUnitDeath((Unit) {
      ._payload = self
    });
    return method(self);
  }

  __declspec(dllexport) int32_t __thiscall onResourceRelease(PVOID self) {
    int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + REMOTE_METHOD_ON_UNIT_DELETE);
    res_OnRelease((Resource) {
      ._payload = self
    });
    return method(self);
  }

  __declspec(dllexport) int32_t onUnitBuy(long double multiplier) {
    int32_t (*method)(long double) = (int32_t (*)(long double)) ((uint8_t *)lib_BaseAddress() + REMOTE_METHOD_ON_UNIT_BUY);
    return eeTa_OnUnitBuy(multiplier, method);
  }

  __declspec(dllexport) PVOID __thiscall onResourceInit(PVOID player, PVOID unit) {
    PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)lib_BaseAddress() + REMOTE_METHOD_ON_RESOURCE_INIT);
    eeTa_OnUnitCreate(unit);
    res_InitResource(unit);
    return method(player, unit);
  }

  __declspec(dllexport) int32_t __thiscall onGameStart(PVOID self, PVOID _a, PVOID _b) {
    rebuildDataStructures();
    int32_t __thiscall (*method)(PVOID, PVOID, PVOID) = (int32_t __thiscall (*)(PVOID, PVOID, PVOID)) ((uint8_t *)mapPolygons);
    return method(self, _a, _b);
  }

  __declspec(dllexport) PVOID __thiscall onMapInitialization(PVOID self, PVOID _1) {
    PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)lib_BaseAddress() + REMOTE_METHOD_ON_MAP_INIT);
    PVOID response = method(self, _1);
    eeTa_OnMapInit();
    return response;
  }
}

void rebuildDataStructures() {
  eeTa_RebuildExtraDataStructure();
}

void setMapStart() {
  size_t *methodPointer = (size_t *)((size_t)(lib_BaseAddress()) + REMOTE_METHOD_SET_MAP_START);
  builder_AllowRules(methodPointer, sizeof(size_t) * 2);
  mapPolygons = *methodPointer;
  *methodPointer = (size_t)onGameStart;
}

void addBotMethodsHooks() {
  cs_CheckSum();
  builder_Definition((PVOID)METHOD_HOOK_ON_UNIT_ITERATION, (PVOID)onUnitIteration);
  builder_Definition((PVOID)METHOD_HOOK_ON_FRAME, (PVOID)onFrame);
  builder_Definition((PVOID)METHOD_HOOK_ON_UNIT_DELETE, (PVOID)onUnitDelete);
  builder_Definition((PVOID)METHOD_HOOK_ON_PLANE_PARK, (PVOID)onPlanePark);
  builder_Definition((PVOID)METHOD_HOOK_ON_UNIT_BUY, (PVOID)onUnitBuy);
  builder_Definition((PVOID)METHOD_HOOK_ON_RESOURCE_INIT, (PVOID)onResourceInit);
  builder_Definition((PVOID)METHOD_HOOK_ON_RESOURCE_RELEASE, (PVOID)onResourceRelease);
  builder_Definition((PVOID)METHOD_HOOK_ON_MAP_INITIALIZATION, (PVOID)onMapInitialization);
  #if defined(REPLACE_MMU)
    builder_ReplaceMMUMethods();
  #endif
  setMapStart();
}

extern "C"  __declspec(dllexport) void __cdecl DLLMain() {
  if(!lib_IsLoaded()) {
    exit(0xC0000135); // libs DLL not found
  }

  addBotMethodsHooks();
}