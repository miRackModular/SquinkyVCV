
#pragma once

#include <set>
#include <vector>

class SimpleQuantizer
{
public:
    enum class Scales {
        _12Even,
        _8Even,
        _12Just,
        _8Just
    };
    SimpleQuantizer(std::vector<Scales>& scales, Scales scale);

    float quantize(float);
private:
    std::set<float> pitches_12even;
    std::set<float> pitches_8even;
    std::set<float> pitches_12just;
    std::set<float> pitches_8juse;
    std::set<float>& cur_set = pitches_12even;
    using iterator = std::set<float>::iterator; 
};