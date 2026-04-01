#pragma once
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "EETwa.h"
#include "InjectUtilities.h"
#include "PlayerState.h"

void bt_OnFrame();
void bt_OnInit();
void bt_OnUnitIteration(Unit unit);
void bt_OnUnitDestroy(Unit unit);