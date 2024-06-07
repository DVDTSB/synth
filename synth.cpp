#include "daisy_seed.h"
#include "daisysp.h"

#include "synth.h"
#include "voice.h"

using namespace daisy;
using namespace daisysp;

void Synth::Init()
{
    for(int i = 0; i < 7; i++)
    {
        voice[i].sample_rate = sample_rate;

        //copy synth settings to voice

        for(int j = 0; j < 3; j++)
        {
            voice[i].osc_waveform[j] = osc_waveform[j];
            voice[i].osc_amp[j]      = osc_amp[j];
            voice[i].env_attack[j]   = env_attack[j];
            voice[i].env_decay[j]    = env_decay[j];
            voice[i].env_release[j]  = env_release[j];
            voice[i].env_sustain[j]  = env_sustain[j];
        }

        voice[i].flt_res    = flt_res;
        voice[i].flt_cutoff = flt_cutoff;

        //copy mod settings to voice

        for(int j = 0; j < 6; j++)
        {
            voice[i].mod[j].src    = mod[j].src;
            voice[i].mod[j].target = mod[j].target;
            voice[i].mod[j].amount = mod[j].amount;
        }


        voice[i].Init();
    }
}

void Synth::SetWaveform()
{
    for(int i = 0; i < 7; i++)
    {
        voice[i].SetWaveform();
    }
}

void Synth::SetEnvelope()
{
    for(int i = 0; i < 7; i++)
    {
        voice[i].SetEnvelope();
    }
}

void Synth::SetFilter()
{
    for(int i = 0; i < 7; i++)
    {
        voice[i].SetFilter();
    }
}

void Synth::SetLFO()
{
    for(int i = 0; i < 7; i++)
    {
        voice[i].SetLFO();
    }
}

void Synth::NoteOn(uint8_t midi_note, uint8_t midi_velocity)
{
    // Find a free voice

    for(int i = 0; i < 7; i++)
    {
        if(voice[i].gate == false)
        {
            voice[i].NoteOn(midi_note, midi_velocity);
            break;
        }
    }
}

void Synth::NoteOff(uint8_t midi_note)
{
    for(int i = 0; i < 7; i++)
    {
        if(voice[i].gate == true && voice[i].midi_note == midi_note)
        {
            voice[i].NoteOff(midi_note);
            break;
        }
    }
}

void Synth::Process(float *out_l, float *out_r)
{
    float _l = 0, _r = 0;
    for(int i = 0; i < 7; i++)
    {
        float l = 0, r = 0;
        voice[i].Process(&l, &r);
        _l += l;
        _r += r;
    }
    *out_l = _l;
    *out_r = _r;
}