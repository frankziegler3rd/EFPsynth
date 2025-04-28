/*
 *  Frank Ziegler, Embedded Systems Final Project
 *
 * zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
 * zzzzzzzzzzzzzzzzz EFPsynth zzzzzzzzzzzzzzzzzzz
 * zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
 */

#include <MozziGuts.h>
#include <Oscil.h>

#include <tables/smoothsquare8192_int8.h>
#include <tables/sin2048_int8.h>
#include <tables/saw4096_int8.h>
#include <tables/triangle2048_int8.h>
#include <tables/whitenoise8192_int8.h>

#define CONTROL_RATE 64

// oscillators
Oscil<SMOOTHSQUARE8192_NUM_CELLS, AUDIO_RATE> sqware(SMOOTHSQUARE8192_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> sine(SIN2048_DATA);
Oscil<SAW4096_NUM_CELLS, AUDIO_RATE> saw(SAW4096_DATA);
Oscil<TRIANGLE2048_NUM_CELLS, AUDIO_RATE> triangle(TRIANGLE2048_DATA);
Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> whitenoise(WHITENOISE8192_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> lfo(SIN2048_DATA);

void setup() {
  sqware.setFreq(440); 
  sine.setFreq(440); 
  saw.setFreq(440);
  triangle.setFreq(440); 
  whitenoise.setFreq(440); 
  lfo.setFreq(1);
  startMozzi(CONTROL_RATE);
}

void updateControl() {
  int pitch = analogRead(A0);
  int wobble = analogRead(A1);
  float osc_freq = map(pitch, 0, 1023, 100, 1000);
  float lfo_freq = fmap(wobble, 0, 1023, 0.1, 15.0);
  lfo.setFreq(lfo_freq);
  sqware.setFreq(osc_freq); 
  sine.setFreq(osc_freq); 
  saw.setFreq(osc_freq);
  triangle.setFreq(osc_freq); 
  whitenoise.setFreq(osc_freq); 
}

int updateAudio() {
  int wave;
  int lfo_val = lfo.next();
  int curr = map(analogRead(A2), 0, 1023, 0, 4); // current oscillator ()

  switch (curr) {
    case 0:
      wave = sqware.next();
      break;
    case 1:
      wave = sine.next();
      break;
    case 2:
      wave = saw.next();
      break;
    case 3:
      wave = triangle.next();
      break;
    case 4:
      wave = whitenoise.next();
      break;
  }
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
