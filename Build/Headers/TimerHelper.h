#pragma once
#include <vector>
#include <Windows.h>
#include <stdint.h>

typedef struct TimeAtom_t {
  PVOID method;
  size_t time;
  PVOID arguments; // Arguments should be alocated on the heap and the calee should take care of freeing the object.
  size_t _lastCheckedTime;
} TimeAtom;


typedef struct TimerHelper_t {
  std::vector<TimeAtom> *fractures;
} TimerHelper;

typedef TimerHelper *PTimerHelper;

PTimerHelper tmr_Init();
uint64_t tmr_Milliseconds();
void tmrs_AddMethod(PTimerHelper self, TimeAtom atom);
void tmr_OnFrame(PTimerHelper self);