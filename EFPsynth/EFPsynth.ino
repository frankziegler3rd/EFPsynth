/*
 *  Frank Ziegler, Embedded Systems Final Project
 *
 * zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
 * zzzzzzzzzzzzzzzzz EFPsynth zzzzzzzzzzzzzzzzzzz
 * zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
 */

#include <MozziGuts.h>
#include <Oscil.h>
#include <LowPassFilter.h>
#include <tables/smoothsquare8192_int8.h>
#include <tables/sin2048_int8.h>
#include <tables/saw4096_int8.h>
#include <tables/triangle2048_int8.h>
#include <tables/whitenoise8192_int8.h>

#define CONTROL_RATE 64 // how often updateControl is called per second

// oscillators to switch between
Oscil<SMOOTHSQUARE8192_NUM_CELLS, AUDIO_RATE> sqware(SMOOTHSQUARE8192_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> sine(SIN2048_DATA);
Oscil<SAW4096_NUM_CELLS, AUDIO_RATE> saw(SAW4096_DATA);
Oscil<TRIANGLE2048_NUM_CELLS, AUDIO_RATE> triangle(TRIANGLE2048_DATA);
Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> whitenoise(WHITENOISE8192_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> lfo(SIN2048_DATA); // for amplitude modulation

int osc1 = 0, osc2 = 1; // the two currently running
float blend = 0.5; // total overall blend of the two oscillators
                   // i.e. 0.5 for sin/square is half and half.
                   // i.e. 0.75 for sin/square is 3/4 sin, 1/4 square.                   

void setup() {
  sqware.setFreq(440); 
  sine.setFreq(440); 
  saw.setFreq(440);
  triangle.setFreq(440); 
  whitenoise.setFreq(440); 
  lfo.setFreq(1);
  startMozzi(CONTROL_RATE);
}

/*
 * Mozzi function to handle user input and calculations that don't need to be done at sample rate.
 */
void updateControl() {
  // read pot-inputted pitch, blend, and wobble
  int pitch = analogRead(A0);
  int tremolo = analogRead(A1);
  blend = analogRead(A4) / 1023.0; // 0.0 to 1.0 blend between osc1 and osc2
  // map analog reads for oscillator pots over range of 0-5
  osc1 = map(analogRead(A2), 0, 1023, 0, 4); // current oscillator 1
  osc2 = map(analogRead(A3), 0, 1023, 0, 4); // current oscillator 2
  float osc_freq = map(pitch, 0, 1023, 100, 1000);
  float lfo_freq = fmap(tremolo, 0, 1023, 0, 15.0);
  // lfo gets its own because its in its own range
  lfo.setFreq(lfo_freq);
  // set the pitch-based oscillators to all the same frequency so switching doesnt sound off
  sqware.setFreq(osc_freq); 
  sine.setFreq(osc_freq); 
  saw.setFreq(osc_freq);
  triangle.setFreq(osc_freq);
  whitenoise.setFreq(osc_freq);
}

/*
 * audio logic ran at sample rate
 */
int updateAudio() {
  int wave;
  int lfo_val = lfo.next();
  int wave1 = getNextWave(osc1);
  int wave2 = getNextWave(osc2);
  float depth = 1.0; // modulation intensity
  // maps lfo value to a range 0.0 - 1.0 so it basically "wobbles" the frequency at this rate
  // note: lfo.next is in range of -127 - 128
  //       divided by 128 makes it -1 - 1 (ish)
  //       *.50 = -.50 - .50 (ish)
  //       + 0.5 = 0.0 - 1.0 (*depth, which can change based on preference)
  float amplitude_modulation = ((lfo_val / 128.0) * 0.5 + 0.5) * depth;
  // manages the balance of the oscillators
  int mixed = ((1.0 - blend) * wave1 + blend * wave2) * amplitude_modulation;
  return mixed;
}

/*
 * returns the next sample in the wave table based upon which oscillator is currently being used
 */
int getNextWave(int osc) {
  switch (osc) {
    case 0: return sqware.next();
    case 1: return sine.next();
    case 2: return saw.next();
    case 3: return triangle.next();
    case 4: return whitenoise.next();
    default: return 0;
  }
}

void loop() {
  // function that calls updateAudio at sample rate
  // Mozzi by default uses 16384 Hz (16.384 kHz)
  // so 16384 times per second.
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
