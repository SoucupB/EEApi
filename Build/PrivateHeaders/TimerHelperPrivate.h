#pragma once
#include "TimerHelper.h"

typedef struct TimerHelper_t {
  std::vector<TimeAtom> *fractures;
} TimerHelper;

typedef TimerHelper *PTimerHelper;

PTimerHelper tmr_Init();
void tmrs_Delete(PTimerHelper self);
void tmr_OnFrame(PTimerHelper self);