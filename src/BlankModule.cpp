
#include <sstream>
#include "Squinky.hpp"
#include "WidgetComposite.h"

#ifdef _BLANKMODULE
#include "Blank.h"
#include "ctrl/SqHelper.h"
#include "ctrl/SqMenuItem.h"

using Comp = Blank<WidgetComposite>;

/**
 */
struct BlankModule : Module
{
public:
    BlankModule();
    /**
     *
     * Overrides of Module functions
     */
    void step() override;
    void onSampleRateChange() override;

    std::shared_ptr<Comp> blank;
private:

};

void BlankModule::onSampleRateChange()
{
}

BlankModule::BlankModule()
{
    config(Comp::NUM_PARAMS, Comp::NUM_INPUTS, Comp::NUM_OUTPUTS, Comp::NUM_LIGHTS);
    blank = std::make_shared<Comp>(this);
    std::shared_ptr<IComposite> icomp = Comp::getDescription();
    SqHelper::setupParams(icomp, this); 

    onSampleRateChange();
    blank->init();
}

void BlankModule::step()
{
    blank->step();
}

////////////////////
// module widget
////////////////////

struct BlankWidget : ModuleWidget
{
    BlankWidget(BlankModule *);
    DECLARE_MANUAL("Blank Manul", "https://github.com/squinkylabs/SquinkyVCV/blob/master/docs/booty-shifter.md");

    Label* addLabel(const Vec& v, const char* str, const NVGcolor& color = SqHelper::COLOR_BLACK)
    {
        Label* label = new Label();
        label->box.pos = v;
        label->text = str;
        label->color = color;
        addChild(label);
        return label;
    }
};


/**
 * Widget constructor will describe my implementation structure and
 * provide meta-data.
 * This is not shared by all modules in the DLL, just one
 */

#include "./seq/SqGfx.h"

class Button2 : public Button
{
public:
    // this isn't firing. don't know why
    void onAction(const ::rack::event::Action& e) override {
        DEBUG("onAction from button");
    }

    void onDragEnd(const ::rack::event::DragEnd& e) override {
        Button::onDragEnd(e);
        DEBUG("on DRAG END FOR ME");
    }
};

class ParentWindow : public OpaqueWidget
{
public:
    void draw(const DrawArgs &args) override
    {
        NVGcontext *vg = args.vg;
        NVGcolor color     = nvgRGB(0x80, 0x80, 0x80);
        SqGfx::filledRect(vg, color, 0, 0, box.size.x, box.size.y);
        Widget::draw(args);
    }

    void onButton(const event::Button &e) override
    {
        DEBUG("Parent window on button consumed=%d", e.isConsumed());
        OpaqueWidget::onButton(e);      // If I don't do this, child doesn't get buttons
        DEBUG("Parent: after call base, cons=%d", e.isConsumed());
    }
};

class ChildWindow : public OpaqueWidget
{
public:
    void draw(const DrawArgs &args) override
    {
        NVGcontext *vg = args.vg;
        NVGcolor color     = nvgRGB(0xf0, 0x80, 0x80);
        SqGfx::filledRect(vg, color, 0, 0, box.size.x, box.size.y);
        Widget::draw(args);
    }

    void onButton(const event::Button &e) override
    {
        DEBUG("Child window on button");
        OpaqueWidget::onButton(e);     
    }
};

BlankWidget::BlankWidget(BlankModule *module)
{
    setModule(module);
    box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
    SqHelper::setPanel(this, "res/blank_panel.svg");

    auto winp = new ParentWindow();
    winp->box.pos = Vec(40, 40);
    winp->box.size = Vec(100, 100);
    addChild(winp);

    auto win = new ChildWindow();
    win->box.pos = Vec(40, 40);
    win->box.size = Vec(50, 50);
    winp->addChild(win);
}

Model *modelBlankModule = createModel<BlankModule, BlankWidget>("squinkylabs-blank");
#endif

