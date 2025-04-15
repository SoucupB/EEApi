#include <stdio.h>
#include <stdint.h>
#include "InjectUtilities.h"

int main(int argc, char *argv[]) {
  DWORD pid = util_FindProcID("EE-AOC.exe");
  if(!pid) {
    printf("Pid not found!\n");
    return 0;
  }
  if(!util_LoadDLL(pid, argv[1])) {
    printf("Bots.dll failed to load\n");
    return 0;
  }
  printf("Succesfull insert!\n");
}