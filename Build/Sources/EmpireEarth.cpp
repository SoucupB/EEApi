#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "InjectUtilities.h"
#include "MethodsDefinitions.h"
#include "LibManager.h"
#include "EETwa.h"
#include "Helpers.h"
#include "Resource.h"

static uint8_t onInitFlag = 0;
static size_t mapPolygons;
void eeTa_RebuildExtraDataStructure();
void eeTa_RebuildDTs();
void rebuildDataStructures();

extern "C" {
  __declspec(dllexport) int32_t __thiscall onUnitIteration(PVOID self) {
    int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0x1540DC);
    eeTa_OnUnitFrame((Unit) {
      ._payload = self
    });
    return method(self);
  }

  __declspec(dllexport) int32_t __thiscall onFrame(PVOID self) {
    int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0x15401E);
    if(!onInitFlag) {
      if(!eeTa_ShouldOnInitExecute()) {
        return method(self);
      }
      eeTa_OnInit();
      onInitFlag = 1;
    }
    eeTa_OnFrame();
    return method(self);
  }

  __declspec(dllexport) int32_t __thiscall onUnitDeath(PVOID self, PVOID _a, PVOID _b, PVOID _c) {
    int32_t __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (int32_t __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)lib_BaseAddress() + 0x22975A);
    eeTa_OnUnitDeath((Unit) {
      ._payload = self
    });
    return method(self, _a, _b, _c);
  }

  __declspec(dllexport) int32_t __thiscall onPlanePark(PVOID self) {
    int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0x1FDAF3);
    eeTa_OnUnitDeath((Unit) {
      ._payload = self
    });
    return method(self);
  }

  __declspec(dllexport) int32_t __fastcall onUnitDelete(PVOID self) {
    int32_t __fastcall (*method)(PVOID) = (int32_t __fastcall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0x1DB4);
    eeTa_OnUnitDeath((Unit) {
      ._payload = self
    });
    return method(self);
  }

  __declspec(dllexport) int32_t __thiscall onResourceRelease(PVOID self) {
    int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)lib_BaseAddress() + 0x1DB4);
    res_OnRelease((Resource) {
      ._payload = self
    });
    return method(self);
  }

  __declspec(dllexport) int32_t onUnitBuy(long double multiplier) {
    int32_t (*method)(long double) = (int32_t (*)(long double)) ((uint8_t *)lib_BaseAddress() + 0x148291);
    return eeTa_OnUnitBuy(multiplier, method);
  }

  __declspec(dllexport) PVOID __thiscall onResourceInit(PVOID player, PVOID unit) {
    PVOID __thiscall (*method)(PVOID, PVOID) = (PVOID __thiscall (*)(PVOID, PVOID)) ((uint8_t *)lib_BaseAddress() + 0x16F49E);
    res_InitResource(unit);
    return method(player, unit);
  }

  __declspec(dllexport) int32_t __thiscall onGameStart(PVOID self, PVOID _a, PVOID _b) {
    rebuildDataStructures();
    int32_t __thiscall (*method)(PVOID, PVOID, PVOID) = (int32_t __thiscall (*)(PVOID, PVOID, PVOID)) ((uint8_t *)mapPolygons);
    return method(self, _a, _b);
  }
}

void rebuildDataStructures() {
  onInitFlag = 0;
  eeTa_RebuildExtraDataStructure();
}

void setMapStart() {
  size_t *methodPointer = (size_t *)((size_t)(lib_BaseAddress()) + 0x437924);
  builder_AllowRules(methodPointer, sizeof(size_t) * 2);
  mapPolygons = *methodPointer;
  *methodPointer = (size_t)onGameStart;
}

void addBotMethodsHooks() {
  builder_Definition((PVOID)0x1540CD, (PVOID)onUnitIteration);
  builder_Definition((PVOID)0x15321F, (PVOID)onFrame);
  builder_Definition((PVOID)0x13B8CF, (PVOID)onUnitDelete);
  builder_Definition((PVOID)0x1F5E09, (PVOID)onPlanePark);
  builder_Definition((PVOID)0x16B3C3, (PVOID)onUnitBuy);
  builder_Definition((PVOID)0x16F275, (PVOID)onResourceInit);
  builder_Definition((PVOID)0x16F33E, (PVOID)onResourceRelease);
  builder_ReplaceMMUMethods();
  setMapStart();
}

extern "C"  __declspec(dllexport) void __cdecl someDllMain() {
  if(!lib_IsLoaded()) {
    exit(0xC0000135); // libs DLL not found
  }

  addBotMethodsHooks();
}