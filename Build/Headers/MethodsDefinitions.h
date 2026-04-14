#pragma once
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "InjectUtilities.h"

void builder_Definition(PVOID remoteAddress, PVOID localAddress);
void builder_PrintMemoryLayout(PVOID ref, size_t sz);
void builder_Store(PVOID ref, size_t sz);
void builder_CheckChanges(PVOID ref);
void builder_FillValue(PVOID ref, size_t offset, size_t value);
void builder_ReplaceMMUMethods();