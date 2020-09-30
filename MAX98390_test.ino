#include <Audio.h>
#include <Wire.h>
#include <Bounce.h>

#include "chords.h"

// Special thanks to Matthew Rahtz - http://amid.fish/karplus-strong/

AudioSynthKarplusStrong  string1;
AudioSynthKarplusStrong  string2;
AudioSynthKarplusStrong  string3;
AudioSynthKarplusStrong  string4;
AudioSynthKarplusStrong  string5;
AudioSynthKarplusStrong  string6;
AudioMixer4              mixer1;
AudioMixer4              mixer2;
AudioEffectFreeverb      reverb1;
AudioMixer4              mixer3;
//AudioOutputI2S         i2s1;
AudioOutputI2S2          i2s1;
AudioConnection          patchCord1(string1, 0, mixer1, 0);
AudioConnection          patchCord2(string2, 0, mixer1, 1);
AudioConnection          patchCord3(string3, 0, mixer1, 2);
AudioConnection          patchCord4(string4, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, mixer2, 0);
AudioConnection          patchCord6(string5, 0, mixer2, 1);
AudioConnection          patchCord7(string6, 0, mixer2, 2);
AudioConnection          patchCord8(mixer2, 0, reverb1, 0);
AudioConnection          patchCord9(reverb1, 0, mixer3, 0);
AudioConnection          patchCord10(mixer2, 0, mixer3, 1);
AudioConnection          patchCord11(mixer3, 0, i2s1, 0);
AudioConnection          patchCord12(mixer3, 0, i2s1, 1);

Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);  // 15 = 15 ms debounce time
//Bounce button2 = Bounce(2, 15);

const int finger_delay = 5;
const int hand_delay = 220;

int chordnum=0;

void setup() {
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  //pinMode(2, INPUT_PULLUP);
  while (!Serial && millis() < 1000) ; // wait
  Serial.println("MAX98390 Test");
  init_max98390();
  AudioMemory(15);
  mixer1.gain(0, 0.15);
  mixer1.gain(1, 0.15);
  mixer1.gain(2, 0.15);
  mixer1.gain(3, 0.15);
  mixer2.gain(1, 0.15);
  mixer2.gain(2, 0.15);
  mixer3.gain(0, 0.5);
  mixer3.gain(1, 0.5);
  delay(700);
  max98390_write(0x23E0, max98390_read(0x23E0) | 0x10);
  max98390_volume(0xA0);
}

void strum_up(const float *chord, float velocity);
void strum_dn(const float *chord, float velocity);

void loop() {
  const float *chord;

  // each time through the loop, play a different chord
  if (chordnum == 0) {
    chord = Cmajor;
    Serial.println("C major");
    chordnum = 1;
  } else if (chordnum == 1) {
    chord = Gmajor;
    Serial.println("G major");
    chordnum = 2;
  } else if (chordnum == 2) {
    chord = Aminor;
    Serial.println("A minor");
    chordnum = 3;
  } else {
    chord = Eminor;
    Serial.println("E minor");
    chordnum = 0;
  }

  // then strum the 6 string several times
  strum_up(chord, 1.0);
  mydelay(hand_delay * 2);
  strum_up(chord, 1.0);
  mydelay(hand_delay);
  strum_dn(chord, 0.8);
  mydelay(hand_delay * 2);
  strum_dn(chord, 0.8);
  mydelay(hand_delay);
  strum_up(chord, 1.0);
  mydelay(hand_delay);
  strum_dn(chord, 0.8);
  mydelay(hand_delay);
  strum_up(chord, 1.0);
  mydelay(hand_delay * 2);
  strum_up(chord, 1.0);
  mydelay(hand_delay);
  strum_dn(chord, 0.7);
  mydelay(hand_delay * 2);
  strum_dn(chord, 0.7);
  mydelay(hand_delay);
  strum_up(chord, 1.0);
  mydelay(hand_delay);
  strum_dn(chord, 0.7);
  mydelay(hand_delay);

  Serial.print("Max CPU Usage = ");
  Serial.print(AudioProcessorUsageMax(), 1);
  Serial.println("%");
}

void strum_up(const float *chord, float velocity)
{
  if (chord[0] > 20.0) string1.noteOn(chord[0], velocity);
  mydelay(finger_delay);
  if (chord[1] > 20.0) string2.noteOn(chord[1], velocity);
  mydelay(finger_delay);
  if (chord[2] > 20.0) string3.noteOn(chord[2], velocity);
  mydelay(finger_delay);
  if (chord[3] > 20.0) string4.noteOn(chord[3], velocity);
  mydelay(finger_delay);
  if (chord[4] > 20.0) string5.noteOn(chord[4], velocity);
  mydelay(finger_delay);
  if (chord[5] > 20.0) string6.noteOn(chord[5], velocity);
  mydelay(finger_delay);
}

void strum_dn(const float *chord, float velocity)
{
  if (chord[5] > 20.0) string1.noteOn(chord[5], velocity);
  mydelay(finger_delay);
  if (chord[4] > 20.0) string2.noteOn(chord[4], velocity);
  mydelay(finger_delay);
  if (chord[3] > 20.0) string3.noteOn(chord[3], velocity);
  mydelay(finger_delay);
  if (chord[2] > 20.0) string4.noteOn(chord[2], velocity);
  mydelay(finger_delay);
  if (chord[1] > 20.0) string5.noteOn(chord[1], velocity);
  mydelay(finger_delay);
  if (chord[0] > 20.0) string6.noteOn(chord[0], velocity);
  mydelay(finger_delay);
}

void mydelay(unsigned int milliseconds)
{
	static bool bass_ext = true;
	elapsedMillis msec;

	float knob1 = (float)analogRead(A1) / 1023.0;
	float knob2 = (float)analogRead(A2) / 1023.0;
	float knob3 = (float)analogRead(A3) / 1023.0;
  knob1 = 0.99;
  knob2 = 0.2;
  knob3 = 0.95;
	//max98390_volume(knob1 * 60.0 + 100.0);
  //max98390_volume(0xA0);
	milliseconds = milliseconds * ((1.0 / 3.0 - knob2 / 3.0) + 0.75);
	mixer3.gain(0, 1.0 - knob3); // wet
	mixer3.gain(1, knob3); // dry

	while (msec < milliseconds) {
		button0.update();
		button1.update();
		//button2.update();
		if (button0.fallingEdge()) {
			Serial.println("Button0 press");
			if (bass_ext) {
				Serial.println("turn off bass extension");
				//max98390_write(0x23E0, max98390_read(0x23E0) & ~0x10);
				bass_ext = false;
			} else {
				Serial.println("turn on bass extension");
				max98390_write(0x23E0, max98390_read(0x23E0) | 0x10);
				bass_ext = true;
			}
		}
		if (button0.risingEdge()) {
			Serial.println("Button0 release");
		}
		if (button1.fallingEdge()) {
			Serial.println("Button1 press");
		}
		if (button1.risingEdge()) {
			Serial.println("Button1 release");
		}
		//if (button2.fallingEdge()) {
			//Serial.println("Button2 press");
		//}
		//if (button2.risingEdge()) {
			//Serial.println("Button2 release");
		//}

	}
}


extern "C" const uint16_t MAX98390_Registers_Addr38[];

void max98390_volume(int vol)
{
	if (vol < 0 || vol > 0xA0) return;
	max98390_write(0x23BA, vol);
}


void max98390_write(uint16_t addr, uint8_t data)
{
	Wire.beginTransmission(0x38);
	Wire.write(addr >> 8);
	Wire.write(addr);
	Wire.write(data);
	int r = Wire.endTransmission();
	//Serial.printf(" r = %d, addr = %04X, data = %02X\n", r, addr, data);
}

uint8_t max98390_read(uint16_t addr)
{
	Wire.beginTransmission(0x38);
	Wire.write(addr >> 8);
	Wire.write(addr);
	Wire.endTransmission(false);
	if (Wire.requestFrom(0x38, 1) == 1) {
		return Wire.read();
	}
	return 0;
}


void init_max98390()
{
	const uint16_t *table = MAX98390_Registers_Addr38;
	Wire.begin();
	max98390_write(0x2000, 1); // reset
	while (1) {
		uint16_t addr = *table++;
		uint16_t data = *table++;
		if (addr < 0x2000 || addr > 0x23FF) break;
		max98390_write(addr, data);
		if (addr == 0x23FF) break;
	}
}
