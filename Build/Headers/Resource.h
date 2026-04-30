#pragma once
#include <windows.h>
#include "Game.h"

typedef struct Resource_t {
  PVOID _payload;
} Resource;

void res_OnInit();
vector<Resource> res_All();