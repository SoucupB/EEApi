#include "TimerHelper.h"

uint64_t tmr_Milliseconds() {
  LARGE_INTEGER frequency, counter;
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&counter);
  return (counter.QuadPart * 1000) / frequency.QuadPart;
}

PTimerHelper tmr_Init() {
  PTimerHelper self = (PTimerHelper)malloc(sizeof(TimerHelper));
  self->fractures = new std::vector<TimeAtom>();
  return self;
}

void tmrs_Execute(TimeAtom *atom) {
  void (*method)(PVOID) = (void (*)(PVOID)) ((uint8_t *)atom->method);
  method(atom->arguments);
}

void tmrs_OnFrame(TimeAtom *atom) {
  uint64_t currentTime = tmr_Milliseconds();
  if(currentTime - atom->_lastCheckedTime >= atom->time) {
    tmrs_Execute(atom);
    atom->_lastCheckedTime = currentTime;
  }
}

void tmr_OnFrame(PTimerHelper self) {
  for(size_t i = 0, c = self->fractures->size(); i < c; i++) {
    tmrs_OnFrame(&((*self->fractures)[i]));
  }
}

void tmrs_AddMethod(PTimerHelper self, TimeAtom atom) {
  atom._lastCheckedTime = tmr_Milliseconds();
  self->fractures->push_back(atom);
}