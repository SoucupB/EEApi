#include <math.h>
#include "AttackDecision.h"
#include "PlayerState.h"
#include "ResourceManager.h"

__declspec(dllexport) PVOID fakeStdin;

void execDataPengus() {
  if(GetAsyncKeyState('J') & 0x8000) {
    eeTa_FilePrintf("PULAS NIGA\n");
    Beep (300, 250);
  }
}

void bt_OnInit() {
}

void bt_OnFrame() {
  execDataPengus();
}

void bt_OnUnitDestroy(Unit unit) {

}