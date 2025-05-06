# Frank Ziegler, Embedded Systems
## Final Project: Digital Synth with Mozzi library

### Group Members and Responsibilities
Well it's just me. I wrote all of the code, meaning the config and the audio logic, and built the circuit on my own.

### Components and Interfaces They Used
- PAM8403 Amplifier: GPIO through resistor/capacitor
- 3 watt 8 ohm speaker: powered through amplifier
- 5 potentiometers: analog GPIO

### Libraries That Were Used

Primarily just the Mozzi library and the oscillator class, and a range of pre-computed wavetable oscillators

### Context Diagram
![stateDiagram](https://github.com/user-attachments/assets/d4629f41-a450-4740-8a83-6061907c0e73)

### Wiring diagram
<img width="1128" alt="EFP_SCHEMATIC_bb" src="https://github.com/user-attachments/assets/19098146-862a-43dc-911e-98557bbb4812" />

### Future Work
- MIDI interfacing to use it in digital audio workstations to create actual instrumentals.
- More oscillators
- Custom oscillators
- Envelope shaping (attack, decay, sustain, release)
- Reverb/delay controls
- Arpeggiation controls
- Filtering/bitcrushing
MIDI keyboard implementation to switch between drone and non-drone modes.

### Sources used:
- https://github.com/sensorium/Mozzi
- https://sensorium.github.io/Mozzi/learn/under-the-hood/
- https://github.com/sensorium/Mozzi/tree/master/examples
