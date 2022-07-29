#include <time.h>

#include "daisy_patch_sm.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;
using namespace patch_sm; 

#include "clouds/dsp/fx/fx_engine.h"
#include "clouds/dsp/frame.h"
#include "clouds/dsp/fx/voxverb.h"
#include "rings/dsp/follower.h"

using namespace clouds;
using namespace rings; 

VoxVerb vox_verb; 
uint16_t reverb_buffer[65536];
DaisyPatchSM patch;
FloatFrame in_out[kMaxBlockSize];
Follower follower_;
Switch toggle;

void AudioCallback(AudioHandle::InputBuffer  in, AudioHandle::OutputBuffer out, size_t size) {
    patch.ProcessAnalogControls();

    float cvOneValue = patch.GetAdcValue(CV_1);
    float cvTwoValue = patch.GetAdcValue(CV_2);
    float cvThreeValue = patch.GetAdcValue(CV_3);
    float cvFourValue = patch.GetAdcValue(CV_4);

    float amplitude_envelope, brightness_envelope;
    CONSTRAIN(amplitude_envelope, 0.0f, 5.0f);

    float reverb_amount = cvOneValue * 0.95f;
    reverb_amount += (cvTwoValue / 2.0f) * (2.0f - cvTwoValue);
    CONSTRAIN(reverb_amount, 0.0f, 1.0f);

    // Reverb 
    vox_verb.set_amount(reverb_amount * 0.54f);
    vox_verb.set_delay_time(fmap(cvTwoValue, 2680.0f, 4680.0f));
    vox_verb.set_time(0.35f + 0.63f * reverb_amount);
    vox_verb.set_diffusion(fmap(cvThreeValue, 0.3f, 0.9f));
    vox_verb.set_input_gain(0.2f);
    vox_verb.set_lp(0.6f + 0.37f * cvFourValue);

    for(size_t i = 0; i < size; i++) {
        // Read Inputs
        in_out->l = IN_L[i]; 
        in_out->r = IN_R[i];

        // CV Envelope follower of dry sound
        float max = IN_L[i] > IN_R[i] ? IN_L[i] : IN_R[i];
        follower_.Process(max, &amplitude_envelope, &brightness_envelope);
        patch.WriteCvOut(CV_OUT_1, amplitude_envelope);
        float amp = fmap(amplitude_envelope, 0.0f, 5.0f, daisysp::Mapping::LINEAR);
        patch.WriteCvOut(CV_OUT_2, amp);
        
        // Add Reverb
        vox_verb.Process(in_out, 1);

        // Output
        OUT_L[i] = in_out->l;
        OUT_R[i] = in_out->r;
    }
}

int main(void)
{
    patch.Init();
    toggle.Init(patch.B8);

    patch.StartAudio(AudioCallback);
    
    vox_verb.Init(reverb_buffer);

    float kSampleRate = patch.AudioSampleRate();
    follower_.Init(
      8.0f / kSampleRate,
      160.0f / kSampleRate,
      1600.0f / kSampleRate);

    while(1) {}
}