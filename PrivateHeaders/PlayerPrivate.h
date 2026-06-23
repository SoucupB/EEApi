#pragma once
#include "EETypes.h"

void ply_Inits(PVOID self);
void ply_Print();
PVOID ply_TechTree_Ref(TechTree techTree);
void ply_SetResources(Player player, ResourceIndex resource, int32_t value);