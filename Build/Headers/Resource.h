#pragma once
#include <windows.h>
#include "Game.h"

typedef struct Resource_t {
  PVOID _payload;
} Resource;

vector<Resource> res_All();
PVOID res_Reference(Resource resource);
void res_InitResource(PVOID resource);
char *res_Name(Resource self);
NeutralUnitType res_Type(Resource unit);
Resource res_Null();