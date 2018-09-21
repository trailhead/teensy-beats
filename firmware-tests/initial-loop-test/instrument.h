#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__

#include "beats.h"
#include <Audio.h>

class Instrument {
  protected:
    Instrument(char *inst_name, AudioMixer4 *p_mix, unsigned int ch) : inst_name_(inst_name), p_mixer_(p_mix), mixer_channel_(ch) {};
    char *inst_name_;
    AudioMixer4         *p_mixer_;
    unsigned int         mixer_channel_;
};

class WaveformInstrument : public Instrument {
  public:
    WaveformInstrument(char *inst_name,
                       AudioSynthWaveform     *p_wf,
                       AudioEffectEnvelope    *p_env,
                       AudioEffectReverb      *p_rev,
                       AudioMixer4            *p_mix,
                       unsigned int           ch
    ) : Instrument(inst_name, p_mix, ch), p_waveform_(p_wf), p_env_(p_env), p_reverb_(p_rev) {};
  private:
    AudioSynthWaveform  *p_waveform_;
    AudioEffectEnvelope *p_env_;
    AudioEffectReverb   *p_reverb_;
};

class NoiseInstrument : public Instrument {
  public:
    NoiseInstrument(char *inst_name,
                    AudioSynthNoiseWhite        *p_noise,
                    AudioFilterStateVariable    *p_filter,
                    AudioEffectEnvelope         *p_env,
                    AudioMixer4                 *p_mix,
                    unsigned int                ch

    ) : Instrument(inst_name, p_mix, ch), p_noise_(p_noise), p_filter_(p_filter), p_env_(p_env) {};
  private:
    AudioSynthNoiseWhite      *p_noise_;
    AudioFilterStateVariable  *p_filter_;
    AudioEffectEnvelope       *p_env_;
    AudioEffectReverb         *p_reverb_;
};

#endif // __INSTRUMENT_H__
