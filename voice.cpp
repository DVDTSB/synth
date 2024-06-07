#include "daisy_seed.h"
#include "daisysp.h"

#include "voice.h"


using namespace daisy;
using namespace daisysp;


void Voice::Init()
{
    gate      = false;
    midi_note = -1;
    //init oscillators

    for(int i = 0; i < 3; i++)
    {
        osc[i].Init(sample_rate);
        osc[i].SetWaveform(osc_waveform[i]);
        osc[i].SetAmp(0.5);  //def
        osc[i].SetFreq(440); //def
    }
    /*   for(int i = 0; i < 3; i++)
    {
        lfo[i].Init(sample_rate);
        lfo[i].SetWaveform(WAVE_SIN);
        lfo[i].SetAmp(0.5); //def
        lfo[i].SetFreq(1);  //def
    }
    for(int i = 0; i < 3; i++)
    {
        env[i].Init(sample_rate);
        env[i].SetTime(ADSR_SEG_ATTACK, env_attack[i]);
        env[i].SetTime(ADSR_SEG_DECAY, env_decay[i]);
        env[i].SetTime(ADSR_SEG_RELEASE, env_release[i]);
        env[i].SetSustainLevel(env_sustain[i]);
    }
*/
    flt.Init(sample_rate);
    flt.SetRes(flt_res);
    flt.SetFreq(flt_cutoff);
}

void Voice::SetWaveform()
{
    for(int i = 0; i < 3; i++)
    {
        osc[i].SetWaveform(osc_waveform[i]);
    }
}

void Voice::SetEnvelope()
{
    for(int i = 0; i < 3; i++)
    {
        env[i].SetTime(ADSR_SEG_ATTACK, env_attack[i]);
        env[i].SetTime(ADSR_SEG_DECAY, env_decay[i]);
        env[i].SetTime(ADSR_SEG_RELEASE, env_release[i]);
        env[i].SetSustainLevel(env_sustain[i]);
    }
}

void Voice::SetFilter()
{
    flt.SetRes(flt_res);
    flt.SetFreq(flt_cutoff);
}

void Voice::SetLFO()
{
    for(int i = 0; i < 3; i++)
    {
        lfo[i].SetFreq(lfo_freq[i]);
    }
}

void Voice::NoteOn(uint8_t midi_note, uint8_t midi_velocity)
{
    this->midi_note = midi_note;
    gate            = true;
    for(int i = 0; i < 3; i++)
    {
        float freq   = mtof(midi_note);
        osc_pitch[i] = freq;
    }
}

void Voice::NoteOff(uint8_t midi_note)
{
    this->midi_note = -1;
    gate            = false;
}

void Voice::Process(float *out_l, float *out_r)
{
    float _osc_pitch[3] = {osc_pitch[0], osc_pitch[1], osc_pitch[2]};
    float _osc_amp[3]   = {osc_amp[0], osc_amp[1], osc_amp[2]};
    float _flt_cutoff   = flt_cutoff;
    float _flt_res      = flt_res;
    /*


    float env_out[3];
    for(int i = 0; i < 3; i++)
    {
        env_out[i] = env[i].Process(gate);
    }
    float lfo_out[3];
    for(int i = 0; i < 3; i++)
    {
        lfo_out[i] = lfo[i].Process();
    }

    //loop trough mod[]W

    for(int i = 0; i < 6; i++)
    {
        float mod_out = 0;
        switch(mod[i].src)
        {
            case MOD_SRC_NONE: mod_out = 0; break;
            case MOD_SRC_LFO0: mod_out = lfo_out[0]; break;
            case MOD_SRC_LFO1: mod_out = lfo_out[1]; break;
            case MOD_SRC_LFO2: mod_out = lfo_out[2]; break;
            case MOD_SRC_ENV0: mod_out = env_out[0]; break;
            case MOD_SRC_ENV1: mod_out = env_out[1]; break;
            case MOD_SRC_ENV2: mod_out = env_out[2]; break;
            default: break;
        }
        switch(mod[i].target)
        {
            case MOD_TARGET_NONE: break;
            case MOD_TARGET_OSC0_PITCH:
                _osc_pitch[0] = (_osc_pitch[0] * mod_out) * mod[i].amount
                                + _osc_pitch[0] * (1 - mod[i].amount);
                break;
            case MOD_TARGET_OSC1_PITCH:
                _osc_pitch[1] = (_osc_pitch[1] * mod_out) * mod[i].amount
                                + _osc_pitch[1] * (1 - mod[i].amount);
                break;
            case MOD_TARGET_OSC2_PITCH:
                _osc_pitch[2] = (_osc_pitch[2] * mod_out) * mod[i].amount
                                + _osc_pitch[2] * (1 - mod[i].amount);
                break;
            case MOD_TARGET_OSC0_AMP:
                _osc_amp[0] = (_osc_amp[0] * mod_out) * mod[i].amount
                              + _osc_amp[0] * (1 - mod[i].amount);
                break;
            case MOD_TARGET_OSC1_AMP:
                _osc_amp[1] = (_osc_amp[1] * mod_out) * mod[i].amount
                              + _osc_amp[1] * (1 - mod[i].amount);
                break;
            case MOD_TARGET_OSC2_AMP:
                _osc_amp[2] = (_osc_amp[2] * mod_out) * mod[i].amount
                              + _osc_amp[2] * (1 - mod[i].amount);
                break;
            case MOD_TARGET_FILTER_CUTOFF:
                _flt_cutoff = (_flt_cutoff * mod_out) * mod[i].amount
                              + _flt_cutoff * (1 - mod[i].amount);
                break;
            case MOD_TARGET_FILTER_RES:
                _flt_res = (_flt_res * mod_out) * mod[i].amount
                           + _flt_res * (1 - mod[i].amount);
                break;
            default: break;
        }
    }*/

    //set the values to the actual values

    for(int i = 0; i < 3; i++)
    {
        osc[i].SetFreq(_osc_pitch[i]);
        osc[i].SetAmp(_osc_amp[i]);
    }
    //flt.SetFreq(_flt_cutoff);
    //flt.SetRes(_flt_res);

    //process the voice


    float output = 0;
    for(int i = 0; i < 3; i++)
    {
        output += osc[i].Process() * gate;
    }

    output = flt.Process(output);


    *out_l = output;
    *out_r = output;
}