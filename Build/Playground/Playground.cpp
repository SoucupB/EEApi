#include <math.h>
#include "AttackDecision.h"
#include "PlayerState.h"
#include "ResourceManager.h"

__declspec(dllexport) PVOID fakeStdin;

void cleanDataChecker() {
  if(GetAsyncKeyState('J') & 0x8000) {
    Beep (300, 250);
  }
}

void bt_OnInit() {
}

void bt_OnUnitDestroy(Unit unit) {

}