
#include <sstream>
#include "Squinky.hpp"
#include "WidgetComposite.h"

#ifdef _DTMODULE
#include "DrumTrigger.h"
#include "ctrl/SqHelper.h"
#include "ctrl/SqMenuItem.h"

using Comp = DrumTrigger<WidgetComposite>;

/**
 */
struct DrumTriggerModule : Module
{
public:
    DrumTriggerModule();
    /**
     *
     * Overrides of Module functions
     */
    void step() override;
    void onSampleRateChange() override;

    std::shared_ptr<Comp> blank;
private:

};

void DrumTriggerModule::onSampleRateChange()
{
}

DrumTriggerModule::DrumTriggerModule()
{
    config(Comp::NUM_PARAMS, Comp::NUM_INPUTS, Comp::NUM_OUTPUTS, Comp::NUM_LIGHTS);
    blank = std::make_shared<Comp>(this);
    std::shared_ptr<IComposite> icomp = Comp::getDescription();
    SqHelper::setupParams(icomp, this); 

    onSampleRateChange();
    blank->init();
}

void DrumTriggerModule::step()
{
    blank->step();
}

////////////////////
// module widget
////////////////////

struct DrumTriggerWidget : ModuleWidget
{
    DrumTriggerWidget(DrumTriggerModule *);
    DECLARE_MANUAL("Polygate manual", "https://github.com/squinkylabs/SquinkyVCV/blob/sq11/docs/dt.md");

    Label* addLabel(const Vec& v, const char* str, const NVGcolor& color = SqHelper::COLOR_BLACK)
    {
        Label* label = new Label();
        label->box.pos = v;
        label->text = str;
        label->color = color;
        addChild(label);
        return label;
    }

    void makeInputs(DrumTriggerModule* module);
    void makeOutputs(DrumTriggerModule* module);
    void makeOutput(DrumTriggerModule* module, int i);
};

const float xLed = 10;
const float xJack = 40;
const float yJack = 330;
const float dy = 30;
const float yInput = 40;
const float xOff = -10;

void DrumTriggerWidget::makeInputs(DrumTriggerModule* module)
{
    addInput(createInputCentered<PJ301MPort>(
        Vec(40 + xOff, yInput),
        module,
        Comp::CV_INPUT));
    addLabel(Vec(25 + xOff, yInput+20), "CV");
    addInput(createInputCentered<PJ301MPort>(
        Vec(70 + xOff, yInput),
        module,
        Comp::GATE_INPUT));
    addLabel(Vec(55 + xOff, yInput+20), "G");
}

void DrumTriggerWidget::makeOutput(DrumTriggerModule* module, int index)
{
    const float y = yJack -dy * index;
    addOutput(createOutputCentered<PJ301MPort>(
        Vec(xJack, y),
        module,
        index + Comp::GATE0_OUTPUT));

    addChild(createLight<MediumLight<GreenLight>>(
            Vec(xLed, y),
            module,
            Comp::LIGHT0 + index));
}

void DrumTriggerWidget::makeOutputs(DrumTriggerModule* module)
{
    for (int i=0; i< Comp::numChannels; ++i) {
        makeOutput(module, i);
    }
}

/**
 * Widget constructor will describe my implementation structure and
 * provide meta-data.
 * This is not shared by all modules in the DLL, just one
 */

DrumTriggerWidget::DrumTriggerWidget(DrumTriggerModule *module)
{
    setModule(module);
    box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
    SqHelper::setPanel(this, "res/dt_panel.svg");

    makeInputs(module);
    makeOutputs(module);

    // screws
    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild( createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
}
 
Model *modelDrumTriggerModule = createModel<DrumTriggerModule, DrumTriggerWidget>("squinkylabs-dt");
#endif
