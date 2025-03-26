#include "app.h"
#include "imgui.h"
#include "implot.h"
#include "solver.h"

#include <iostream>

App::App() : Window("Propeller Crossflow Autorotation Solver", 2000, 1000) {}

App::~App() {}

void App::update()
{
    // Get the main viewport
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    // Set the window position and size to match the viewport
    ImGui::SetNextWindowPos(viewport->Pos); // Dock to the top-left of the viewport
    ImGui::SetNextWindowSize(viewport->Size); // Make the window match the size of the viewport

    // Create the ImGui window
    ImGui::Begin("Solver", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::Columns(3, nullptr, false);

    ImGui::SeparatorText("Simulation Parameters");
    ImGui::InputFloat("Simulation Time", &m_configuration.simTime);
    ImGui::InputFloat("Time Step", &m_configuration.timeStep, 0.0F, 0.0F, "%.5f");
    ImGui::InputFloat("Radial Step", &m_configuration.radialStep, 0.0F, 0.0F, "%.5f");

    ImGui::SeparatorText("Flight Conditions");
    ImGui::InputFloat("Freestream Velocity", &m_configuration.freestreamVelocity[0]);
    ImGui::InputFloat("Air Density", &m_configuration.airDensity);
    ImGui::InputFloat("Kinematic Viscosity", &m_configuration.kinematicViscosity, 0.0F, 0.0F, "%.8f");
    
    ImGui::SeparatorText("Initial Conditions");
    ImGui::InputFloat("Initial Angular Velocity", &m_configuration.initialAngularVelocity);

    ImGui::SeparatorText("Motor Parameters");
    ImGui::InputFloat("Motor Resistance", &m_configuration.motorResistance, 0.0F, 0.0F, "%.6f");
    ImGui::InputFloat("Motor Velocity Constant", &m_configuration.motorVelcoityConstant);
    ImGui::InputFloat("Motor Torque Constant", &m_configuration.motorTorqueConstant, 0.0F, 0.0F, "%.6f");
    ImGui::InputFloat("Motor Rotor Moment of Inertia", &m_configuration.motorRotorMomentOfInertia, 0.0F, 0.0F, "%.6f");

    ImGui::SeparatorText("Propeller and Hub Geometry");
    ImGui::InputFloat("Propeller Radius", &m_configuration.propellerRadius);
    ImGui::InputInt("Number of Blades", &m_configuration.numBlades);
    if(m_configuration.numBlades < 2) m_configuration.numBlades = 2;
    ImGui::InputFloat("hub Radius", &m_configuration.hubRadius);

    ImGui::SeparatorText("Propeller Geometry");
    ImGui::Text("TODO");

    ImGui::SeparatorText("Solver and Solutions");
    if (ImGui::Button("Solve"))
    {
        if (!(m_future.valid() && m_future.wait_for(std::chrono::seconds(0)) != std::future_status::ready))
        {
            m_future = std::async(std::launch::async, Solver::solve, m_configuration, std::ref(m_solution), std::ref(m_progress));
        }
    }

    if (m_future.valid() && m_future.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
    {
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::ProgressBar(m_progress, ImVec2(0.0f, 0.0f), "Solving...");
    }
    else if (m_future.valid() && m_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
       m_future.get();
    }

    ImGui::NextColumn();

    renderPlots();

    // End the ImGui window
    ImGui::End();
}

void App::renderPlots()
{

    if(m_solution.mutex.try_lock())
    {
        if(ImPlot::BeginPlot("Angular Position"))
        {
            ImPlot::EndPlot();
        }

        if(ImPlot::BeginPlot("Angular Acceleration"))
        {
            ImPlot::EndPlot();
        }

        if(ImPlot::BeginPlot("Lift"))
        {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_AutoFit);
            ImPlot::SetNextLineStyle(ImVec4(0.0,1.0,0.0,1.0), 2.0f);
            ImPlot::PlotLine("Fz", m_solution.time.data(), m_solution.lift.data(), m_solution.time.size()-1); // PlotLine expects x and y data pointers and size
            ImPlot::EndPlot();
        }

        ImGui::NextColumn();

        if(ImPlot::BeginPlot("Angular Velocity"))
        {
            ImPlot::EndPlot();
        }
        
        if(ImPlot::BeginPlot("Torque"))
        {
            ImPlot::EndPlot();
        }

        if(ImPlot::BeginPlot("Drag"))
        {
            ImPlot::EndPlot();
        }
    }
    m_solution.mutex.unlock(); 
}