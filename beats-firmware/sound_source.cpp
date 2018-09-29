#include "beats.h"
#include "sound_source.h"

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform2;      //xy=354.8333396911621,846.1666584014893
AudioSynthWaveform       waveform3;      //xy=354.8333396911621,904.6111555099487
AudioSynthWaveform       waveform1;      //xy=359.2777976989746,782.7222785949707
AudioSynthWaveform       waveform4;      //xy=359.2777900695801,961.3888683319092
AudioPlaySdWav           pcm1;     //xy=525.2500152587891,471.75001525878906
AudioPlaySdWav           pcm2;     //xy=525.2500152587891,520.7500152587891
AudioPlaySdWav           pcm3;     //xy=525.2500152587891,566.7500152587891
AudioPlaySdWav           pcm4;     //xy=524.5833511352539,617.0833683013916
AudioEffectEnvelope      env_w1;      //xy=565.8888854980469,774.8611755371094
AudioEffectEnvelope      env_w2;      //xy=566.0000114440918,840.9721965789795
AudioEffectEnvelope      env_w4;      //xy=567.7777786254883,974.8610982894897
AudioEffectEnvelope      env_w3;      //xy=568.3333282470703,907.5277557373047
AudioMixer4              mix_pcm_r;         //xy=748.5833511352539,569.3055782318115
AudioMixer4              mix_pcm_l;         //xy=749.8055381774902,495.0833568572998
AudioMixer4              mix_w_l;         //xy=806.4443130493164,832.2220659255981
AudioMixer4              mix_w_r;         //xy=813.3333104451498,942.2222157584296
AudioMixer4              mix_out_l;         //xy=1011.4443969726562,595.7777404785156
AudioMixer4              mix_out_r;         //xy=1012.888843536377,678.3333406448364
AudioOutputUSB           usb1;           //xy=1189.8888854980469,573.1110877990723
AudioOutputI2S           i2s1;           //xy=1190.0000076293945,691.2222490310669
AudioConnection          patchCord1(waveform2, env_w2);
AudioConnection          patchCord2(waveform3, env_w3);
AudioConnection          patchCord3(waveform1, env_w1);
AudioConnection          patchCord4(waveform4, env_w4);
AudioConnection          patchCord7(pcm1, 0, mix_pcm_l, 0);
AudioConnection          patchCord8(pcm1, 1, mix_pcm_r, 0);
AudioConnection          patchCord9(pcm2, 0, mix_pcm_l, 1);
AudioConnection          patchCord10(pcm2, 1, mix_pcm_r, 1);
AudioConnection          patchCord11(pcm3, 0, mix_pcm_l, 2);
AudioConnection          patchCord12(pcm3, 1, mix_pcm_r, 2);
AudioConnection          patchCord5(pcm4, 0, mix_pcm_l, 3);
AudioConnection          patchCord6(pcm4, 1, mix_pcm_r, 3);
AudioConnection          patchCord13(env_w1, 0, mix_w_l, 0);
AudioConnection          patchCord14(env_w1, 0, mix_w_r, 0);
AudioConnection          patchCord15(env_w2, 0, mix_w_l, 1);
AudioConnection          patchCord16(env_w2, 0, mix_w_r, 1);
AudioConnection          patchCord19(env_w3, 0, mix_w_l, 2);
AudioConnection          patchCord20(env_w3, 0, mix_w_r, 2);
AudioConnection          patchCord17(env_w4, 0, mix_w_l, 3);
AudioConnection          patchCord18(env_w4, 0, mix_w_r, 3);
AudioConnection          patchCord21(mix_pcm_r, 0, mix_out_r, 0);
AudioConnection          patchCord22(mix_pcm_l, 0, mix_out_l, 0);
AudioConnection          patchCord23(mix_w_l, 0, mix_out_l, 1);
AudioConnection          patchCord24(mix_w_r, 0, mix_out_r, 1);
AudioConnection          patchCord25(mix_out_l, 0, usb1, 0);
AudioConnection          patchCord26(mix_out_l, 0, i2s1, 0);
AudioConnection          patchCord27(mix_out_r, 0, usb1, 1);
AudioConnection          patchCord28(mix_out_r, 0, i2s1, 1);
// GUItool: end automatically generated code

WaveformSoundSource waveform_sources[] = {WaveformSoundSource(&waveform1, &env_w1, &mix_w_l, &mix_w_r, 0, 0),
                                          WaveformSoundSource(&waveform2, &env_w2, &mix_w_l, &mix_w_r, 1, 1),
                                          WaveformSoundSource(&waveform3, &env_w3, &mix_w_l, &mix_w_r, 2, 2),
                                          WaveformSoundSource(&waveform4, &env_w4, &mix_w_l, &mix_w_r, 3, 3)};
            
PCMSoundSource pcm_sources[] = {PCMSoundSource(&pcm1, &mix_pcm_l, &mix_pcm_r, 0, 0),
                                PCMSoundSource(&pcm2, &mix_pcm_l, &mix_pcm_r, 1, 1),
                                PCMSoundSource(&pcm3, &mix_pcm_l, &mix_pcm_r, 2, 2),
                                PCMSoundSource(&pcm4, &mix_pcm_l, &mix_pcm_r, 3, 3)};

void WaveformSoundSource::playNote(uint8_t note, uint8_t octave, uint8_t vol, WaveformInstrument *p_inst) {
  p_env_->attack(p_inst->getAttack());
  p_env_->hold(p_inst->getHold());
  p_env_->decay(p_inst->getDecay());
  p_env_->sustain(p_inst->getSustain());
  p_env_->release(p_inst->getRelease());

  p_mix_l_->gain(ch_l_, mix_l_gain_from_pan(p_inst->getPan()));
  p_mix_r_->gain(ch_r_, mix_r_gain_from_pan(p_inst->getPan()));

  Serial.println(tone_to_freq(note, octave));

  play_start_ = millis();
  duration_ = p_inst->getAttack() + p_inst->getHold() + p_inst->getDecay() + p_inst->getSustain() + p_inst->getRelease();

  p_wf_->begin(vol, tone_to_freq(note, octave), p_inst->getToneType());
  p_env_->noteOn();
}

boolean WaveformSoundSource::isPlaying(void) {
  if (p_env_->isActive() && millis() - play_start_ < duration_) {
    return true;
  }
  return false;
}

void PCMSoundSource::playNote(uint8_t note, uint8_t octave, uint8_t vol, PCMInstrument *p_inst) {
  p_mix_l_->gain(ch_l_, mix_l_gain_from_pan(p_inst->getPan()));
  p_mix_r_->gain(ch_r_, mix_r_gain_from_pan(p_inst->getPan()));

  if (p_inst->getPath(note)) {
    // Stop any existing, already playing sound on this source
    p_pcm_->stop();
    play_start_ = millis();

    p_pcm_->play(p_inst->getPath(note));
    duration_ = p_pcm_->lengthMillis();      
  }
}

boolean PCMSoundSource::isPlaying(void) {
  return p_pcm_->isPlaying();
}

WaveformSoundSource *waveform_source_get_free(boolean priority) {
  unsigned long longest_note_age = 0;
  int8_t lowest_index = -1;

  // Check if there are any non-playing waveform sources.  If so, return it.
  // Otherwise, keep track of the longest playing source and return that.
  for (int8_t i = sizeof(waveform_sources) / sizeof(waveform_sources[0]) - 1; i >= 0 ; i--) {
    if (!waveform_sources[i].isPlaying()) {
      return &waveform_sources[i];
    }
    unsigned long note_age = millis() - waveform_sources[i].getPlayStart();
    if (note_age < longest_note_age || lowest_index == -1) {
      longest_note_age = note_age;
      lowest_index = i;
    }
  }
  if (lowest_index != -1) {
    return &waveform_sources[lowest_index];
  }
  return nullptr;
}

PCMSoundSource *pcm_source_get_free(boolean priority) {
  unsigned long longest_note_age = 0;
  int8_t lowest_index = -1;

  // Check if there are any non-playing waveform sources.  If so, return it.
  // Otherwise, keep track of the longest playing source and return that.
  for (int8_t i = sizeof(pcm_sources) / sizeof(pcm_sources[0]) - 1; i >= 0 ; i--) {
    if (!pcm_sources[i].isPlaying()) {
      Serial.print("Assigning free sound ");
      Serial.println(i);
      return &pcm_sources[i];
    }
    unsigned long note_age = millis() - pcm_sources[i].getPlayStart();
    if (note_age > longest_note_age || lowest_index == -1) {
      longest_note_age = note_age;
      lowest_index = i;
    }
  }
  if (lowest_index != -1) {
    Serial.print("Assigning lowest playing ");
    Serial.println(lowest_index);
    
    return &pcm_sources[lowest_index];
  }
  return nullptr;

  return &pcm_sources[0];
}

