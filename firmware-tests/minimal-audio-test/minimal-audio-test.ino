#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define NUMROWS       4
#define NUMCOLS       4
#define NUMINST       4
#define NUMTICKS  (NUMROWS*NUMCOLS)
#define BUTTON_DEBOUNCE 250
#define DEFAULT_NOTE  1

// Time interval for serial status in ms
#define STATUS_INTERVAL 1000

uint8_t rows[NUMROWS] = { 28, 29, 30, 31 };
uint8_t cols[NUMCOLS] = { 35, 36, 37, 38 };

bool leds[NUMROWS][NUMCOLS];
long buttons[NUMROWS][NUMCOLS];
uint8_t notes[NUMINST][NUMCOLS][NUMROWS];

uint16_t tones[] = {
  440 / 2,
  466 / 2,
  494 / 2,
  523 / 2,
  554 / 2,
  587 / 2,
  622 / 2,
  659 / 2,
  698 / 2,
  740 / 2,
  784 / 2,
  831 / 2,
  880 / 2,
  440,
  466,
  494,
  523,
  554,
  587,
  622,
  659,
  698,
  740,
  784,
  831,
  880
};

// GUItool: begin automatically generated code
AudioSynthNoiseWhite     noise1;         //xy=376,500
AudioSynthSimpleDrum     drum1;          //xy=404,587
AudioSynthKarplusStrong  string1;        //xy=410,745
AudioSynthWaveform       triangle1;      //xy=415,655
AudioFilterStateVariable filter1;        //xy=536,537
AudioEffectEnvelope      env3;      //xy=613,720
AudioEffectEnvelope      env2;      //xy=634,651
AudioEffectEnvelope      env1;      //xy=696,551
AudioMixer4              mixer1;         //xy=847,588
AudioOutputI2S           i2s1;           //xy=1001,613
AudioConnection          patchCord1(noise1, 0, filter1, 0);
AudioConnection          patchCord2(drum1, 0, mixer1, 1);
AudioConnection          patchCord3(string1, env3);
AudioConnection          patchCord4(triangle1, env2);
AudioConnection          patchCord5(filter1, 2, env1, 0);
AudioConnection          patchCord6(env3, 0, mixer1, 3);
AudioConnection          patchCord7(env2, 0, mixer1, 2);
AudioConnection          patchCord8(env1, 0, mixer1, 0);
AudioConnection          patchCord9(mixer1, 0, i2s1, 1);
AudioConnection          patchCord10(mixer1, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=911,298
// GUItool: end automatically generated code

long enc1_pos    = 0;
long enc2_pos    = 0;

float bpm        = 200.0;
int tick         = 0;
int current_inst = 0;
long next_tick   = 0;
long next_tft    = 0;
bool playing     = true;

void setup() {

  AudioMemory(15);

  AudioNoInterrupts();

  noise1.amplitude(1.0f);

  env1.hold(0);
  env1.attack(0);
  env1.hold(0);
  env1.decay(100);
  env1.sustain(0.0f);
  env1.release(1); 
  env1.releaseNoteOn(0);
  filter1.frequency(8000);

  env2.hold(0);
  env2.attack(10);
  env2.hold(0);
  env2.decay(1000);
  env2.sustain(0.0f);
  env2.release(1); 
  env2.releaseNoteOn(0);
  
  env3.hold(0);
  env3.attack(20);
  env3.hold(0);
  env3.decay(1000);
  env3.sustain(0.0f);
  env3.release(1); 
  env3.releaseNoteOn(0);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  AudioInterrupts();

  next_tick = millis() + (long)(60000.0f / bpm);  
  next_tft = 0;

  notes[0][0][0] = 1;
  notes[0][0][1] = 1;
  notes[0][0][2] = 1;
  notes[0][0][3] = 1;
  notes[0][2][0] = 1;
  
  Serial.begin(115200);
}

void loop() {
  int x, y, inst;
  long ms = 0;
  static long cycles = 0;
  static long last_status = 0;

  ms = millis();
  if (playing && ms >= next_tick) {
    next_tick = ms + (long)(60000.0f / bpm);

    leds[tick / NUMCOLS][tick % NUMCOLS] = false;

    tick++;
    if (tick >= NUMTICKS) {
      tick = 0;
    }

    for (inst = 0; inst < NUMINST; inst++) {
      if (notes[inst][tick / NUMCOLS][tick % NUMCOLS]) {
        if (inst == 0) {
          env1.noteOn();
        }
        if (inst == 1) {
          drum1.frequency(50);
          drum1.noteOn();
        }
        if (inst == 2) {
          triangle1.begin(1.0f, tones[notes[inst][tick / NUMCOLS][tick % NUMCOLS]], WAVEFORM_TRIANGLE);
          env2.decay((60000.0f / bpm));
          env2.noteOn();
        }
        if (inst == 3) {
          string1.noteOn(261.6f, 1.0f);
          env3.noteOn();
        }
        if (inst == current_inst) {
          leds[tick / NUMCOLS][tick % NUMCOLS] = true;
        }
      }
    }
  }

  if (millis() >= last_status + STATUS_INTERVAL) {
    Serial.print((float)cycles / (millis() / 1000.0f));
    Serial.println(" cycles per sec");
    last_status = millis();
  }

  cycles++;
}

