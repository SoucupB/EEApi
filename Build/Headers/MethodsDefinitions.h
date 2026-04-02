#pragma once
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "InjectUtilities.h"

void builder_Definition(PVOID remoteAddress, PVOID localAddress);
void builder_PrintMemoryLayout(PVOID ref, size_t sz);