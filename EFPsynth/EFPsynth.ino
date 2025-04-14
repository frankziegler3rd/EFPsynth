/*
 *  Frank Ziegler, Embedded Systems Final Project
 *
 * zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
 * zzzzzzzzzzzzzzzzz EFPsynth zzzzzzzzzzzzzzzzzzz
 * zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
 */

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/square_no_alias_2048_int8.h>

#define CONTROL_RATE 64

Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> osc(SQUARE_NO_ALIAS_2048_DATA);

void setup() {
  osc.setFreq(440);
  startMozzi(CONTROL_RATE);
}

void updateControl() {
  int sensor = analogRead(A0);
  float freq = map(sensor, 0, 1023, 100, 1000);
  osc.setFreq(freq);
}

int updateAudio() {
  return osc.next();
}

void loop() {
  audioHook();
}
