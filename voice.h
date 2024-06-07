#pragma once

#include "daisy_seed.h"
#include "daisysp.h"


using namespace daisy;
using namespace daisysp;

class Voice
{
  public:
    Voice() {}
    ~Voice() {}

    enum Waveform
    {
        WAVE_SIN,
        WAVE_TRI,
        WAVE_SAW,
        WAVE_RAMP,
        WAVE_SQUARE,
        WAVE_POLYBLEP_TRI,
        WAVE_POLYBLEP_SAW,
        WAVE_POLYBLEP_SQUARE,
        WAVE_LAST
    };

    void Init();
    void SetWaveform();
    void SetEnvelope();
    void SetFilter();
    void SetLFO();
    void NoteOn(uint8_t midi_note, uint8_t midi_velocity = 127);
    void NoteOff(uint8_t midi_note);
    void Process(float *, float *);


    bool    gate;
    uint8_t midi_note;

    Oscillator osc[3];
    Oscillator lfo[3];
    Adsr       env[3];
    MoogLadder flt;

    enum ModSource
    {
        MOD_SRC_NONE,
        MOD_SRC_LFO0,
        MOD_SRC_LFO1,
        MOD_SRC_LFO2,
        MOD_SRC_ENV0,
        MOD_SRC_ENV1,
        MOD_SRC_ENV2,
        MOD_SRC_LAST,
    };

    enum ModTarget
    {
        MOD_TARGET_NONE,
        MOD_TARGET_OSC0_PITCH,
        MOD_TARGET_OSC1_PITCH,
        MOD_TARGET_OSC2_PITCH,
        MOD_TARGET_OSC0_AMP,
        MOD_TARGET_OSC1_AMP,
        MOD_TARGET_OSC2_AMP,
        MOD_TARGET_FILTER_CUTOFF,
        MOD_TARGET_FILTER_RES,
        MOD_TARGET_LAST

    };

    struct Mod
    {
        ModSource src    = MOD_SRC_NONE;
        ModTarget target = MOD_TARGET_NONE;
        float     amount = 0;
    };

    Mod mod[6];

    float    sample_rate;
    float    osc_pitch[3];
    float    osc_amp[3];
    Waveform osc_waveform[3];
    float    lfo_freq[3];
    float    env_attack[3];
    float    env_decay[3];
    float    env_sustain[3];
    float    env_release[3];
    float    flt_cutoff;
    float    flt_res;
    float    flt_drive;
};