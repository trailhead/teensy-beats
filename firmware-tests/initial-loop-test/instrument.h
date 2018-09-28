#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__

#include "beats.h"
#include <Audio.h>

class Instrument {
  protected:
    Instrument(char *inst_name, AudioMixer4 *p_mix_l, AudioMixer4 *p_mix_r, unsigned int ch_l, unsigned int ch_r) : inst_name_(inst_name), p_mix_l_(p_mix_l), p_mix_r_(p_mix_r), ch_l_(ch_l), ch_r_(ch_r) {};
    char *inst_name_;
    AudioMixer4         *p_mix_l_;
    AudioMixer4         *p_mix_r_;
    unsigned int         ch_l_;
    unsigned int         ch_r_;
};

class WaveformInstrument : public Instrument {
  public:
    WaveformInstrument(char *inst_name,
                       AudioSynthWaveform     *p_wf,
                       AudioEffectEnvelope    *p_env,
                       AudioMixer4            *p_mix_l,
                       AudioMixer4            *p_mix_r,
                       unsigned int           ch_l,
                       unsigned int           ch_r
    ) : Instrument(inst_name, p_mix_l, p_mix_r, ch_l, ch_r), p_waveform_(p_wf), p_env_(p_env) {};
  private:
    AudioSynthWaveform  *p_waveform_;
    AudioEffectEnvelope *p_env_;
};

#endif // __INSTRUMENT_H__
