#pragma once
#include <windows.h>
#include "EETypes.h"

void res_InitResource(PVOID resource);
void res_OnRelease(Resource resource);
void res_SetResource(Player ply, ResourceIndex res, int32_t value);