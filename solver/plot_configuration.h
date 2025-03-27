#ifndef _PLOT_CONFIGURATION_H_
#define _PLOT_CONFIGURATION_H_

#include "solution.h"

#include <functional>
#include <string>

struct PlotConfiguration
{
    std::string title;
    std::string xLabel;
    std::string yLabel;
    std::function<const std::vector<float>&(const Solution&)> field;
};

#endif // _PLOT_CONFIGURATION_H_