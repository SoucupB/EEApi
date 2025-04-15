#pragma once
#include <stdint.h>
#include <Windows.h>
#include "EETwaTypes.h"

#define ACTION_BUFFER_SIZE 0xB8

typedef struct MoveAction_t {
  PVOID methodsBundle; // 0x0
  PVOID _const_1; // 0x4
  PVOID _known_Const; // 0x8
  PVOID _const_3; // 0xC
  PVOID _const_4; // 0x10
  PVOID _const_5; // 0x14
  PVOID _const_6; // 0x18
  PVOID _const_7; // 0x1C
  Point pos; // 0x20
  float angle; // 0x28
  PVOID actionType; // 0x2C
  PVOID _const_8; // 0x30
  PVOID _const_9; // 0x34
} MoveAction;

typedef struct MoveActionUnit_t {
  PVOID methodsBundle;
  PVOID _const_1;
  PVOID _known_Const;
  PVOID _const_3;
  PVOID _const_4;
  PVOID _const_5;
  PVOID _const_6;
  PVOID _const_7;
  Point pos1;
  float angle1;
  Point pos2;
  float angle2;
  PVOID target;
} MoveActionUnit;

typedef struct ActionBuffer_t {
  uint8_t *buffer;
  uint8_t validAddress;
} ActionBuffer;