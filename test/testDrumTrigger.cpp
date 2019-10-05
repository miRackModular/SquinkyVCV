
#include "DrumTrigger.h"
#include "TestComposite.h"

#include "asserts.h"

using DT = DrumTrigger<TestComposite>;

static void testInitialState()
{
    DT dt;
    dt.inputs[DT::GATE_INPUT].channels = 8;
    dt.step();
    for (int i = 0; i < numTriggerChannels; ++i) {
        assertLT(dt.outputs[DT::GATE0_OUTPUT + i].value, 1);
    }
}


static void test1Sub(float initCV)
{
    // set up with 8 channels of polyphonic input
    DT dt;
    dt.inputs[DT::GATE_INPUT].channels = 8;
    dt.inputs[DT::CV_INPUT].channels = 8;
   
    // set all the CV inputs to the init value
    for (int i = 0; i < numTriggerChannels; ++i) {
        dt.inputs[DT::CV_INPUT].voltages[i] = initCV;
    }

    // This is pretty sloppy. just happens we set polyphony to 8 so numTriggerChannels is
    // also the number of input channels.

    fprintf(stderr, "test, initCV = %f\n", initCV);
    for (int i = 0; i < numTriggerChannels; ++i) {

        fprintf(stderr, "test, i = %d\n", i);
        // get the pitch that maps to output channel 'i',
        // set the CV input to that pitch, set the gate active
        const float pitch = DT::base() + PitchUtils::semitone * i;
        dt.inputs[DT::CV_INPUT].voltages[i] = pitch;
        dt.inputs[DT::GATE_INPUT].voltages[i] = 10;
        dt.step();

        // should turn on the gate and light for this channel
        assertGT(dt.outputs[DT::GATE0_OUTPUT + i].value, 5);
        assertGT(dt.lights[DT::LIGHT0 + i].value, 5);

        // turn the gate off for this channel
        dt.inputs[DT::GATE_INPUT].voltages[i] = 0;
        dt.step();
        assertLT(dt.outputs[DT::GATE0_OUTPUT + i].value, 1);
        assertLT(dt.lights[DT::LIGHT0 + i].value, 1);
    }
}

static void test1()
{
    test1Sub(-5);
}

static void test2()
{
    test1Sub(0);
}

// feed an input that is out of current range.
static void testLess()
{
    DT dt;
    dt.inputs[DT::GATE_INPUT].channels = 1;

    for (int i = 0; i < numTriggerChannels; ++i) {
        dt.inputs[DT::CV_INPUT].voltages[i] = 0;
        dt.inputs[DT::GATE_INPUT].voltages[i] = 0;
    }

    const int i = 2;
    const float pitch = DT::base() + PitchUtils::semitone * i;
    dt.inputs[DT::CV_INPUT].voltages[i] = pitch;
    dt.inputs[DT::GATE_INPUT].voltages[i] = 10;
    dt.step();

    // should not respond to out of range port number
    assertLT(dt.outputs[DT::GATE0_OUTPUT + i].value, 1);
}

static void testGlide()
{
    DT dt;
    dt.inputs[DT::GATE_INPUT].channels = 1;
    dt.inputs[DT::CV_INPUT].channels = 1;


    // give it C, gate on
    dt.inputs[DT::CV_INPUT].voltages[0] = 0;
    dt.inputs[DT::GATE_INPUT].voltages[0] = 10;

    dt.step();
    assertGT(dt.outputs[DT::GATE0_OUTPUT].value, 1);

    // now switch pitch to C#
    dt.inputs[DT::CV_INPUT].voltages[0] = 1.f / 12.f;
    dt.step();

    // C# is gate1, gate0 should be off
    assertGT(dt.outputs[DT::GATE1_OUTPUT].value, 1);
    assertLT(dt.outputs[DT::GATE0_OUTPUT].value, 1);
}


void testDrumTrigger()
{
    testInitialState();
    test1();
    test2();
    testLess();
    testGlide();
}