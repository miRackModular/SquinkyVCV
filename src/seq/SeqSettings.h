#pragma once


#include "ISeqSettings.h"

#include "rack.hpp"

#include <stdio.h>

class SeqSettings : public ISeqSettings
{
public:
    friend class GridMenuItem;
    friend class ArticulationMenuItem;
    friend class GridItem;
    friend class ArticItem;
    friend class SequencerSerializer;

    SeqSettings(rack::engine::Module*);
    void invokeUI(rack::widget::Widget* parent) override;

    /**
     * Grid related settings
     */
    float getQuarterNotesInGrid() override;
    bool snapToGrid() override;
    bool snapDurationToGrid() override;
    float quantize(float, bool allowZero) override;
    float quantizeAlways(float, bool allowZero) override;

    float articulation() override;
private:
    rack::engine::Module* const module;

    enum class Grids
    {
        quarter,
        eighth,
        sixteenth
    };

    enum class Artics
    {
        tenPercent,
        twentyPercent,
        fiftyPercent,
        eightyFivePercent,
        oneHundredPercent,
        legato
    };

    std::string getGridString() const;
    std::string getArticString() const;

    static std::vector<std::string> getGridLabels();
    static Grids gridFromString(const std::string& s);
    static std::vector<std::string> getArticulationLabels();
    static Artics articFromString(const std::string& s);

    Grids curGrid = Grids::quarter;
    Artics curArtic = Artics::eightyFivePercent;

    bool snapEnabled = true;
    bool snapDurationEnabled = false;

    static float grid2Time(Grids);
    static float artic2Number(Artics);
    rack::ui::MenuItem* makeSnapItem();
    rack::ui::MenuItem* makeSnapDurationItem();
};