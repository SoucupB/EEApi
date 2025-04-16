#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "InjectUtilities.h"
#include "MethodsDefinitions.h"
#include "BotLogic.h"
#include "EETwa.h"
#include "Helpers.h"

static uint8_t onInitFlag = 0;

extern "C" {
  __declspec(dllexport) int32_t __thiscall onUnitIteration(PVOID self) {
    HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
    if(!hModule) {
      return 0;
    }
    int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)hModule + 0x1540DC);
    eeTa_OnUnitFrame((Unit) {
      ._payload = self
    });
    return method(self);
  }

  __declspec(dllexport) int32_t __thiscall onFrame(PVOID self) {
    HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
    if(!hModule) {
      return 0;
    }
    if(!onInitFlag) {
      eeTa_OnInit();
      onInitFlag = 1;
    }
    int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)hModule + 0x15401E);
    bt_OnFrame();
    eeTa_OnFrame();
    return method(self);
  }

  __declspec(dllexport) int32_t __thiscall onUnitDeath(PVOID self, PVOID _a, PVOID _b, PVOID _c) {
    HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
    if(!hModule) {
      return 0;
    }
    int32_t __thiscall (*method)(PVOID, PVOID, PVOID, PVOID) = (int32_t __thiscall (*)(PVOID, PVOID, PVOID, PVOID)) ((uint8_t *)hModule + 0x22975A);
    eeTa_OnUnitDeath(self);
    return method(self, _a, _b, _c);
  }

  __declspec(dllexport) int32_t __thiscall onPlanePark(PVOID self) {
    HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
    if(!hModule) {
      return 0;
    }
    int32_t __thiscall (*method)(PVOID) = (int32_t __thiscall (*)(PVOID)) ((uint8_t *)hModule + 0x1FDAF3);
    eeTa_OnUnitDeath(self);
    return method(self);
  }

  __declspec(dllexport) int32_t __fastcall onUnitDelete(PVOID self) {
    HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
    if(!hModule) {
      return 0;
    }
    int32_t __fastcall (*method)(PVOID) = (int32_t __fastcall (*)(PVOID)) ((uint8_t *)hModule + 0x1DB4);
    eeTa_OnUnitDeath(self);
    return method(self);
  }

  __declspec(dllexport) int32_t onUnitBuy(long double multiplier) {
    HMODULE hModule = GetModuleHandleA("EE-AOC.exe");
    if(!hModule) {
      return 0;
    }
    int32_t (*method)(long double) = (int32_t (*)(long double)) ((uint8_t *)hModule + 0x148291);
    return eeTa_OnUnitBuy(multiplier, method);
  }
}

void addBotMethodsHooks() {
  builder_Definition((PVOID)0x1540CD, (PVOID)onUnitIteration);
  builder_Definition((PVOID)0x15321F, (PVOID)onFrame);
  builder_Definition((PVOID)0x13B8CF, (PVOID)onUnitDelete);
  builder_Definition((PVOID)0x1F5E09, (PVOID)onPlanePark);
  builder_Definition((PVOID)0x16B3C3, (PVOID)onUnitBuy);
}

__declspec(dllexport) void __cdecl someDllMain() {
  addBotMethodsHooks();

  // Testing checker.
  // builder_Definition((PVOID)0xC2A18, (PVOID)help_New);
  // builder_Definition((PVOID)0xC2A6E, (PVOID)help_New);
  // builder_Definition((PVOID)0xC2A3C, (PVOID)help_FillSelectedUnits);
  // builder_Definition((PVOID)0xBF1B2, (PVOID)help_SearchUnits);


  // builder_Definition((PVOID)0xC2AD2, (PVOID)help_SearchUnits);
  // builder_Definition((PVOID)0xBF1B2, (PVOID)help_SearchUnits);
  // builder_Definition((PVOID)0xBF1B2, (PVOID)help_Checker_4C2A3C);
}