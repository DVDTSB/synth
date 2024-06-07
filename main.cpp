#include "daisy_seed.h"
#include "daisysp.h"

#include "synth.h"
#include "voice.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

Synth  synth;
Switch buttons[7];

uint8_t midinotes[7] = {60, 62, 64, 65, 67, 69, 71};


void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    for(int i = 0; i < 7; i++)
    {
        buttons[i].Debounce();
        if(buttons[i].RisingEdge())
        {
            synth.NoteOn(midinotes[i], 127);
            hw.PrintLine("Note On: %d", i);
        }
        if(buttons[i].FallingEdge())
        {
            synth.NoteOff(midinotes[i]);
        }
    }

    //synth.flt_cutoff = filterknob;
    // synth.SetFilter();

    float voice_left, voice_right;

    for(size_t i = 0; i < size; i += 2)
    {
        synth.Process(&voice_left, &voice_right);
        out[i]     = voice_left;
        out[i + 1] = voice_right;
    }
}


int main(void)
{
    hw.Init();

    hw.SetAudioBlockSize(4); // number of samples handled per callback
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    hw.StartAudio(AudioCallback);

    hw.StartLog(true);
    hw.PrintLine("Synth Example");

    synth.sample_rate     = hw.AudioSampleRate();
    synth.osc_amp[0]      = 0.5;
    synth.osc_amp[1]      = 0;
    synth.osc_amp[2]      = 0;
    synth.osc_waveform[0] = Voice::Waveform::WAVE_SAW;
    synth.osc_waveform[1] = Voice::Waveform::WAVE_TRI;
    synth.osc_waveform[2] = Voice::Waveform::WAVE_SQUARE;
    synth.lfo_freq[0]     = 1;
    synth.lfo_freq[1]     = 1;
    synth.lfo_freq[2]     = 1;
    synth.env_attack[0]   = 0.01;
    synth.env_attack[1]   = 0.01;
    synth.env_attack[2]   = 0.01;
    synth.env_decay[0]    = 0.1;
    synth.env_decay[1]    = 0.1;
    synth.env_decay[2]    = 0.1;
    synth.env_sustain[0]  = 0.5;
    synth.env_sustain[1]  = 0.5;
    synth.env_sustain[2]  = 0.5;
    synth.env_release[0]  = 0.1;
    synth.env_release[1]  = 0.1;
    synth.env_release[2]  = 0.1;
    synth.flt_cutoff      = 700.0;
    synth.flt_res         = 0.8;

    synth.Init();


    // synth.mod[0].src    = Voice::ModSource::MOD_SRC_LFO0;
    //   synth.mod[0].target = Voice::ModTarget::MOD_TARGET_OSC0_PITCH;
    //  synth.mod[0].amount = 0.3;


    for(int i = 0; i < 7; i++)
    {
        buttons[i].Init(hw.GetPin(i), 100);
    }


    while(1) {}
}
