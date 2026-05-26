#include "CheckSum.h"
#include "LibManager.h"
#include "EETwa.h"
#include "Offset.h"

size_t cs_GetCheckSum(PVOID address, size_t size) {
  if(!builder_IsMemoryValid(address)) {
    return 0;
  }
  builder_AddRules(address, size);
  size_t response = 0;
  for(size_t i = 0; i < size; i++) {
    uint8_t *code = (uint8_t *)&response;
    code[(i & 3)] ^= ((uint8_t *)address)[i];
  }

  return response;
}

size_t cs_GetLibCheckSum(PVOID address, size_t size) {
  return cs_GetCheckSum((PVOID)((size_t)lib_BaseAddress() + (size_t)address), size);
}

void cs_PrintError(size_t offset, size_t checkSumExpected, size_t checkSumGot) {
  FILE *fd = fopen("BotsError.txt", "w+");
  fprintf(fd, "Game version mismatch, Expected checksum %p at %p, got %p instead\n", (size_t)lib_BaseAddress() + checkSumExpected, offset, checkSumGot);
  fclose(fd);
}

void cs_CheckSum() {
  size_t checkSize = 64;
  size_t checkSums[][2] = {
    {METHOD_HOOK_ON_UNIT_ITERATION, 0x7759E410},
    {METHOD_HOOK_ON_FRAME, 0xBFDEB07C},
    {METHOD_HOOK_ON_UNIT_DELETE, 0x2C13CAE7},
    {METHOD_HOOK_ON_PLANE_PARK, 0x5A1BA8AE},
    {METHOD_HOOK_ON_UNIT_BUY, 0x7C5A9709},
    {METHOD_HOOK_ON_RESOURCE_INIT, 0x2618EA2D},
    {METHOD_HOOK_ON_RESOURCE_RELEASE, 0x7E312C9A},
    {METHOD_HOOK_ON_MAP_INITIALIZATION, 0xDDA01796},
  };
  for(size_t i = 0; i < sizeof(checkSums) / sizeof(checkSums[0]); i++) {
    size_t checkSum = cs_GetLibCheckSum((PVOID)checkSums[i][0], checkSize);
    if(checkSum != checkSums[i][1]) {
      cs_PrintError(checkSums[i][0], checkSums[i][1], checkSum);
      exit(2002);
    }
  }
}