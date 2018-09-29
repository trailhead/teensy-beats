#include "beats.h"
#include "instrument.h"
#include "sound_source.h"

void WaveformInstrument::playNote(uint8_t note, uint8_t octave, uint8_t vol) {
  WaveformSoundSource *p_src = waveform_source_get_free(false);
  if (p_src) {
    p_src->playNote(note, octave, vol * vol_, this);
  }
}

void PCMInstrument::playNote(uint8_t note, uint8_t octave, uint8_t vol) {
  PCMSoundSource *p_src = pcm_source_get_free(false);
  if (p_src) {
    p_src->playNote(note, octave, vol * vol_, this);
  }
}

