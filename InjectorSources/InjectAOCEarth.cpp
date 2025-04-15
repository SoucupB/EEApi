#include <stdio.h>
#include <stdint.h>
#include "InjectUtilities.h"

int main() {
  DWORD pid = util_FindProcID("EE-AOC.exe");
  if(!pid) {
    printf("Pid not found!\n");
    return 0;
  }
  if(!util_LoadDLL(pid, "D:\\Programs\\Cheats\\EmpireEarth\\Bots.dll")) {
    printf("Bots.dll failed to load\n");
    return 0;
  }
  printf("Succesfull insert!\n");
}