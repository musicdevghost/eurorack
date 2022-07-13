#include "daisy_patch_sm.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;
using namespace patch_sm; 

#include "clouds/dsp/frame.h"
#include "clouds/dsp/fx/reverb.h"

using namespace clouds; 

Reverb clouds_reverb; 
uint16_t reverb_buffer[32768];
DaisyPatchSM patch;
FloatFrame in_out[kMaxBlockSize];
 
void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    patch.ProcessAnalogControls();

    float cvOneValue = patch.GetAdcValue(CV_1);
    float cvTwoValue = patch.GetAdcValue(CV_2);
    float cvThreeValue = patch.GetAdcValue(CV_3);
    float cvFourValue = patch.GetAdcValue(CV_4);

    float reverb_amount = cvOneValue * 0.95f;
    reverb_amount += cvTwoValue * (2.0f - cvTwoValue);
    CONSTRAIN(reverb_amount, 0.0f, 1.0f);

    for(size_t i = 0; i < size; i++)
    {
        // Read Inputs
        in_out->l = IN_L[i];
        in_out->r = IN_R[i];
    
        clouds_reverb.set_amount(reverb_amount * 0.54f);
        clouds_reverb.set_diffusion(0.625f * cvThreeValue);
        clouds_reverb.set_time(0.35f + 0.63f * reverb_amount);
        clouds_reverb.set_input_gain(0.2f);
        clouds_reverb.set_lp(0.6f + 0.37f * cvFourValue);

        clouds_reverb.Process(in_out, 1); 
        
        OUT_L[i] = in_out->l;
        OUT_R[i] = in_out->r;
    }
}

int main(void)
{
    patch.Init();
    patch.StartAudio(AudioCallback);
    clouds_reverb.Init(reverb_buffer);
    while(1) {}
}
