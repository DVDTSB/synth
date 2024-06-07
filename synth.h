#pragma once

#include "daisy_seed.h"
#include "daisysp.h"

#include "voice.h"

using namespace daisy;
using namespace daisysp;

class Synth
{
  public:
    Synth() {}
    ~Synth() {}

    void Init();
    void SetWaveform();
    void SetEnvelope();
    void SetFilter();
    void SetLFO();
    void NoteOn(uint8_t midi_note, uint8_t midi_velocity = 127);
    void NoteOff(uint8_t midi_note);
    void Process(float *, float *);

    Voice voice[7];


    Voice::Mod mod[6];

    float           sample_rate;
    float           osc_pitch[3];
    float           osc_amp[3];
    Voice::Waveform osc_waveform[3];
    float           lfo_freq[3];
    float           env_attack[3];
    float           env_decay[3];
    float           env_sustain[3];
    float           env_release[3];
    float           flt_cutoff;
    float           flt_res;
};
