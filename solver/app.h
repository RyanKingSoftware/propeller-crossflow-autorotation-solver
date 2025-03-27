#ifndef _APP_H_
#define _APP_H_

#include <atomic>
#include <future>

#include "configuration.h"
#include "implot.h"
#include "plot_configuration.h"
#include "window.h"

class App : public Window
{
    public:
        App();
        ~App(); 

    private:
        void update() final;
        void renderPlots();
    
    private:
        Configuration m_configuration = Configuration();
        std::vector<Solution> m_solutions;
        std::future<Solution> m_future;
        std::atomic<float> m_progress = 0;

        int m_selectedSolution = -1;

        const std::array<PlotConfiguration, 3> m_PlotConfigsColumn1 = {{
            {"Angular Velocity", "Time (s)", "Angular Velocity (rad/s)", &Solution::getAngularVelocity},
            {"Lift", "Time (s)", "Lift (N)", &Solution::getLift},
            {"Torque", "Time (s)", "Torque (Nm)", &Solution::getTorque}
        }};

        const std::array<PlotConfiguration, 3> m_PlotConfigsColumn2 = {{
            {"Angular Acceleration", "Time (s)", "Angular Acceleration (rad/s^2)", &Solution::getAngularAcceleration},    
            {"Drag", "Time (s)", "Drag (N)", &Solution::getDrag},
            {"Side Force", "Time (s)", "Side Force (N)", &Solution::getSideForce}
        }};

        template<size_t N>
        void renderPlots(const std::array<PlotConfiguration, N>& plotConfigs)
        {
            for (const auto& plotConfig : plotConfigs)
            {
                if (ImPlot::BeginPlot(plotConfig.title.c_str()))
                {
                    ImPlot::SetupAxes(plotConfig.xLabel.c_str(), plotConfig.yLabel.c_str(), ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                    if(m_selectedSolution != -1)
                    {
                        const Solution& solution = m_solutions[m_selectedSolution];
                        ImPlot::SetNextLineStyle(solution.color, 2.0f);
                        ImPlot::PlotLine(solution.name.c_str(), solution.time.data(), plotConfig.field(solution).data(), solution.time.size() - 1); 
                    }
                    ImPlot::EndPlot();
                }
            }
        }
};

#endif // _APP_H_