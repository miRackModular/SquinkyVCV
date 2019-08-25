#include "MixStereo.h"

#include "TestComposite.h"
#include "asserts.h"

#include <memory>

using MixerS = MixStereo<TestComposite>;

static float gOutputBuffer[8];

template <typename T>
static std::shared_ptr<T> getMixerBase()
{
    auto ret = std::make_shared<T>();
    ret->init();
    auto icomp = ret->getDescription();
    for (int i = 0; i < icomp->getNumParams(); ++i) {
        auto param = icomp->getParam(i);
        ret->params[i].value = param.def;
    }

    return ret;
}

// factory for test mixers
template <typename T>
static std::shared_ptr<T> getMixer();

template <>
std::shared_ptr<MixerS> getMixer<MixerS>()
{
    std::shared_ptr<MixerS> ret = getMixerBase<MixerS>();
    ret->setExpansionOutputs(gOutputBuffer);
    return ret;
}

template <typename T>
static void dumpUb(std::shared_ptr<T> mixer)
{
    const int numUb = sizeof(mixer->unbufferedCV) / sizeof(mixer->unbufferedCV[0]);
    printf("ubcv: ");

    int x = 0;
    for (int i = 0; i < numUb; ++i) {
        printf("%.2f ", mixer->unbufferedCV[i]);
        if (++x >= 4) {
            printf("\n      ");
            x = 0;
        }
    }
    printf("\n");
}


template <typename T>
static void dumpOut(std::shared_ptr<T> mixer)
{
    printf("channel outs:\n");

    int x = 0;
    for (int i = 0; i < T::numChannels; ++i) {
        printf("%.2f ", mixer->unbufferedCV[i]);
    }
    printf("\n");
}
//***********************************************************************************


template <typename T>
void testChannel(int channel, bool useParam)
{
    printf("\n** running test channel %d useParam %d\n", channel, useParam);
    const int group = channel / 2;      //stereo
    auto mixer = getMixer<T>();

    // param > 1 is illegal. Fix this test!
    const float activeParamValue = useParam ? 1.f : .25f;
    const float activeCVValue = useParam ? 5.f : 10.f;

    // zero all inputs, put all channel gains to 1
    for (int i = 0; i < T::numChannels; ++i) {
        mixer->inputs[T::AUDIO0_INPUT + i].value = 0;
        mixer->params[T::GAIN0_PARAM + i].value = 1;
    }

   // auto xx = mixer->inputs[T::PAN0_INPUT].value;
  //  auto yy = mixer->params[T::PAN0_PARAM].value;

    mixer->inputs[T::AUDIO0_INPUT + channel].value = 5.5f;
    mixer->params[T::GAIN0_PARAM + group].value = activeParamValue;
    mixer->inputs[T::LEVEL0_INPUT + group].value = activeCVValue;
    mixer->inputs[T::LEVEL0_INPUT + group].active = true;

    for (int i = 0; i < 1000; ++i) {
        mixer->step();           // let mutes settle
    }

    float atten18Db = 1.0f / (2.0f * 2.0f * 2.0f);

    const float exectedInActiveChannel = (useParam) ?
        (5.5f * .5f) :       // param at 1, cv at 5, gain = .5
        atten18Db * 5.5f;       // param at .25 is 18db down cv at 10 is units

    dumpUb(mixer);
    dumpOut(mixer);
    for (int i = 0; i < T::numChannels; ++i) {
        const int gp = i / 2;
       // auto debugMuteState = mixer->params[T::MUTE0_STATE_PARAM + i];
        float expected = (gp == group) ? exectedInActiveChannel : 0;
        printf("mixer output for channel %d gp %d, useparam %d = %.2f\n", i, gp, useParam, mixer->outputs[T::CHANNEL0_OUTPUT + i].value);
        assertClose(mixer->outputs[T::CHANNEL0_OUTPUT + i].value, expected, .01f);
        
    }
}

template <typename T>
static void testChannel()
{
#if 1
    for (int i = 0; i < T::numChannels; ++i) {
        testChannel<T>(i, true);
        testChannel<T>(i, false);
    }
#else
    testChannel<T>(0, true);
    testChannel<T>(0, false);
#endif
}

void testStereoMix()
{
    testChannel<MixerS>();
}