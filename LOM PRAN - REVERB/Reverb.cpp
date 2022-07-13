#include "daisy_patch_sm.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;
using namespace patch_sm; 


#include "rings/dsp/fx/reverb.h"

using namespace rings; 

Reverb clouds_reverb; 
uint16_t reverb_buffer[65536];

DaisyPatchSM patch;
// ReverbSc     reverb;

float prevCtrlVal[4] = {0};
 
void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    patch.ProcessAnalogControls();

    float cvOneValue = patch.GetAdcValue(CV_1);
    float cvTwoValue = patch.GetAdcValue(CV_2);
    float cvThreeValue = patch.GetAdcValue(CV_3);
    float cvFourValue = patch.GetAdcValue(CV_4);

     prevCtrlVal[0] = cvOneValue;
     float delta = 0.01;

    // float send = 1.0;
    // float drylevel = 1.;
    // float dryL, dryR, sendL, sendR;
    float ins_left[48];
    float ins_right[48];

    for(size_t i = 0; i < size; i++)
    {
        // Read Inputs
        ins_left[i] = IN_L[i];
        ins_right[i]= IN_R[i];
    
        clouds_reverb.set_amount(cvTwoValue * 0.5f);
        clouds_reverb.set_diffusion(0.625f * cvFourValue);
        clouds_reverb.set_time(0.35f + 0.63f * cvThreeValue);// 0.5f + (0.49f * patch_position));
        clouds_reverb.set_input_gain(0.2f);
        clouds_reverb.set_lp(0.3f + cvOneValue * 0.6f);// : 0.6f);

        clouds_reverb.Process(&ins_left[i], &ins_right[i], 1);
        
        OUT_L[i] = ins_left[i];
        OUT_R[i] = ins_right[i];

        // float dryl  = IN_L[i] * in_level + 1;
        // float dryr  = IN_R[i] * in_level + 1;
        // float sendl = IN_L[i] * send_level;
        // float sendr = IN_R[i] * send_level;
        // float wetl, wetr;
        // // reverb.Process(sendl, sendr, &wetl, &wetr);
        // OUT_L[i] = dryl + wetl;
        // OUT_R[i] = dryr + wetr;
    }

    // /** Update Params with the four knobs */
    // float time_knob = patch.GetAdcValue(CV_1);
    // float time      = fmap(time_knob, 0.3f, 0.99f);

    // float damp_knob = patch.GetAdcValue(CV_2);
    // float damp      = fmap(damp_knob, 1000.f, 19000.f, Mapping::LOG);

    // float in_level = patch.GetAdcValue(CV_3); 

    // float send_level = patch.GetAdcValue(CV_4);

    // // reverb.SetFeedback(time);
    // // reverb.SetLpFreq(damp);

    // for(size_t i = 0; i < size; i++)
    // {
    //     float dryl  = IN_L[i] * in_level + 1;
    //     float dryr  = IN_R[i] * in_level + 1;
    //     float sendl = IN_L[i] * send_level;
    //     float sendr = IN_R[i] * send_level;
    //     float wetl, wetr;
    //     // reverb.Process(sendl, sendr, &wetl, &wetr);
    //     OUT_L[i] = dryl + wetl;
    //     OUT_R[i] = dryr + wetr;
    // }
}

int main(void)
{
    patch.Init();
    patch.StartAudio(AudioCallback);
    clouds_reverb.Init(reverb_buffer);
    while(1) {}
}
