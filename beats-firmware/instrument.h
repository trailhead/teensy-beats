#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__

#include "beats.h"

class Instrument {
  protected:
    Instrument() {
      pan_ = 0;
      vol_ = 1.0f;
    };
    char name_[20];
    float pan_;
    float vol_;

  public:
    virtual void playNote(uint8_t note, uint8_t octave, uint8_t vol) = 0;
    float getPan(void) { return pan_; }
    void setPan(float p) { pan_ = p; }
    float getVol(void) { return vol_; }
    void setVol(float v) { vol_ = v; }
};

class WaveformInstrument : public Instrument {
  protected:
    float attack_;
    float hold_;
    float decay_;
    float sustain_;
    float release_;
    short tone_type_;
  
  public:
    WaveformInstrument() {};
    void playNote(uint8_t note, uint8_t octave, uint8_t vol);

    float getAttack(void) { return attack_; }
    void setAttack(float a) { attack_ = a >= 0.0f ? a : 0.0f; }
    float getHold(void) { return hold_; }    
    void setHold(float h) { hold_ = h >= 0.0f ? h : 0.0f; }
    float getDecay(void) { return decay_; }
    void setDecay(float d) { decay_ = d >= 0.0f ? d : 0.0f; }
    float getSustain(void) { return sustain_; }
    void setSustain(float s) { sustain_ = s >= 0.0f ? s : 0.0f; }
    float getRelease(void) { return release_; }
    void setRelease(float r) { release_ = r >= 0.0f ? r : 0.0f; }
    short getToneType(void) { return tone_type_; }
    void setToneType(short t) { tone_type_ = t; if (tone_type_ > WAVEFORM_TRIANGLE) tone_type_ = 0; if (tone_type_ < 0) tone_type_ = WAVEFORM_TRIANGLE; }
};

class PCMInstrument : public Instrument {
  protected:
    char *p_paths_[16];
  
  public:
    PCMInstrument() {
    };
    void playNote(uint8_t note, uint8_t octave, uint8_t vol);

    char * getPath(uint8_t index) { return p_paths_[index]; };
    void setPath(uint8_t index, char *path) { if (index >= 12) return; p_paths_[index] = path; };
};

#endif // __INSTRUMENT_H__
