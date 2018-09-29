#ifndef __SOUND_SOURCE_H__
#define __SOUND_SOURCE_H__

#include <Audio.h>
#include "beats.h"
#include "instrument.h"

class SoundSource {
  protected:
    char *bank_name_;
    AudioMixer4         *p_mix_l_;
    AudioMixer4         *p_mix_r_;
    unsigned int         ch_l_;
    unsigned int         ch_r_;
    unsigned long        play_start_;
    unsigned long        duration_;

    SoundSource(AudioMixer4 *p_mix_l,
                AudioMixer4 *p_mix_r,
                unsigned int ch_l,
                unsigned int ch_r
    ) : p_mix_l_(p_mix_l), p_mix_r_(p_mix_r), ch_l_(ch_l), ch_r_(ch_r) {
      play_start_ = 0;
    };

  public:
    boolean virtual isPlaying(void);
    unsigned long getPlayStart(void) { return play_start_; };
};

class WaveformSoundSource : public SoundSource {
  private:
    AudioSynthWaveform  *p_wf_;
    AudioEffectEnvelope *p_env_;

  public:
    WaveformSoundSource( AudioSynthWaveform     *p_wf,
                         AudioEffectEnvelope    *p_env,
                         AudioMixer4            *p_mix_l,
                         AudioMixer4            *p_mix_r,
                         unsigned int           ch_l,
                         unsigned int           ch_r
    ) : SoundSource(p_mix_l, p_mix_r, ch_l, ch_r), p_wf_(p_wf), p_env_(p_env) {
      p_env_->hold(0);
      p_env_->attack(10);
      p_env_->hold(0);
      p_env_->decay(1000);
      p_env_->sustain(0.0f);
      p_env_->release(1);
      p_env_->releaseNoteOn(0);  
    };
    
    boolean isPlaying(void);
    void playNote(uint8_t note, uint8_t octave, uint8_t vol, WaveformInstrument *p_inst);
};

class PCMSoundSource : public SoundSource {
  public:
    PCMSoundSource( AudioPlaySdWav         *p_pcm,
                    AudioMixer4            *p_mix_l,
                    AudioMixer4            *p_mix_r,
                    unsigned int           ch_l,
                    unsigned int           ch_r
    ) : SoundSource(p_mix_l, p_mix_r, ch_l, ch_r), p_pcm_(p_pcm) {};

    boolean isPlaying(void);
    void playNote(uint8_t note, uint8_t octave, uint8_t vol, PCMInstrument *p_inst);
    
  private:
    AudioPlaySdWav  *p_pcm_;
};

WaveformSoundSource *waveform_source_get_free(boolean priority = true);
PCMSoundSource *pcm_source_get_free(boolean priority = true);

#endif // __SOUND_SOURCE_H__

