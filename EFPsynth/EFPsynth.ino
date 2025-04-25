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
#include <tables/sin2048_int8.h>
#include <tables/saw4096_int8.h>

#define CONTROL_RATE 64

// oscillators
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> square_osc(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> sin_osc(SIN2048_DATA);
Oscil<SAW4096_NUM_CELLS, AUDIO_RATE> osc(SAW4096_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> lfo(SIN2048_DATA);

void setup() {
  osc.setFreq(440);
  lfo.setFreq(1);
  startMozzi(CONTROL_RATE);
}

void updateControl() {
  int pitch = analogRead(A0);
  int wobble = analogRead(A1);
  float osc_freq = map(pitch, 0, 1023, 100, 1000);
  float lfo_freq = fmap(wobble, 0, 1023, 0.1, 15.0);
  lfo.setFreq(lfo_freq);
  osc.setFreq(osc_freq);
}

int updateAudio() {
  int wave = osc.next();
  int lfo_val = lfo.next();

  float depth = 1.0; // modulation intensity
  // maps lfo value to a range 0.0 - 1.0 so it basically "shakes" the amplitude by this rate
  // note: lfo.next is in range of -127 - 128
  //       divided by 128 makes it -1 - 1 (ish)
  //       *.50 = -.50 - .50 (ish)
  //       + 0.5 = 0.0 - 1.0 (*depth, which can change based on preference)
  float amplitude_modulation = (lfo_val / 128.0) * 0.5 * depth + 0.5;

  return wave * amplitude_modulation;
}

void loop() {
  audioHook();
}

/*
 * function for LFO frequency mapping -- LFOs oscillate between 0.1 Hz - 20 Hz
 * built-in map function only maps integers
 * 0    -> 0.1 Hz
 * 1023 -> 10 Hz
 * and so on.
 */
float fmap(float read, float ilow, float ihigh, float olow, float ohigh) {
  return read * (ohigh - olow) / (ihigh - ilow) + olow;
}