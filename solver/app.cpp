#include <iostream>

#include "app.h"
#include "imgui.h"
#include "solver.h"

App::App() : Window("Propeller Crossflow Autorotation Solver", 2200, 980) {}

App::~App() {}

void App::update()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGui::Begin("Solver", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGuiIO& io = ImGui::GetIO();
    ImGui::PushFont(io.Fonts->Fonts[0]);

    ImGui::Columns(3, nullptr, false);

    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::SeparatorText("Simulation Parameters");
    ImGui::PopFont();
    ImGui::InputFloat("Simulation Time (s)", &m_configuration.simTime);
    ImGui::InputFloat("Time Step (s)", &m_configuration.timeStep, 0.0F, 0.0F, "%.5f");
    ImGui::InputFloat("Radial Step (m)", &m_configuration.radialStep, 0.0F, 0.0F, "%.5f");

    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::SeparatorText("Flight Conditions");
    ImGui::PopFont();
    ImGui::InputFloat("Freestream Velocity (m/s)", &m_configuration.freestreamVelocity[0]);
    ImGui::InputFloat("Air Density (kg/m^3)", &m_configuration.airDensity);
    ImGui::InputFloat("Kinematic Viscosity (m^2/s)", &m_configuration.kinematicViscosity, 0.0F, 0.0F, "%.8f");
    
    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::SeparatorText("Initial Conditions");
    ImGui::PopFont();
    ImGui::InputFloat("Initial Angular Velocity (rad/s)", &m_configuration.initialAngularVelocity);

    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::SeparatorText("Motor Parameters");
    ImGui::PopFont();
    ImGui::InputFloat("Motor Resistance (ohms)", &m_configuration.motorResistance, 0.0F, 0.0F, "%.6f");
    ImGui::InputFloat("Motor Velocity Constant (rad/V)", &m_configuration.motorVelcoityConstant);
    ImGui::InputFloat("Motor Torque Constant (Nm/A)", &m_configuration.motorTorqueConstant, 0.0F, 0.0F, "%.6f");
    ImGui::InputFloat("Motor Rotor Moment of Inertia (kg*m^2)", &m_configuration.motorRotorMomentOfInertia, 0.0F, 0.0F, "%.6f");

    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::SeparatorText("Propeller and Hub Geometry");
    ImGui::PopFont();
    ImGui::InputFloat("Propeller Radius (m)", &m_configuration.propellerRadius);
    ImGui::InputInt("Number of Blades", &m_configuration.numBlades);
    if(m_configuration.numBlades < 2) m_configuration.numBlades = 2;
    ImGui::InputFloat("Hub Radius (m)", &m_configuration.hubRadius);

    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::SeparatorText("Propeller Geometry");
    ImGui::PopFont();
    ImGui::Text("TODO");

    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::SeparatorText("Solver and Solutions");
    ImGui::PopFont();
    if (ImGui::Button("Solve"))
    {
        if (!(m_future.valid() && m_future.wait_for(std::chrono::seconds(0)) != std::future_status::ready))
        {
            m_future = std::async(std::launch::async, Solver::solve, m_configuration, std::ref(m_progress));
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Save"))
    {
        if(m_selectedSolution != -1)
        {
            Util::writeSolutionToCsv(m_solutions[m_selectedSolution]);
        }
    }

    if (m_future.valid() && m_future.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
    {
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::ProgressBar(m_progress, ImVec2(0.0f, 0.0f));
    }
    else if (m_future.valid() && m_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
        m_solutions.push_back(m_future.get());

        // Switch the shown solution
        if(m_selectedSolution != -1)
        {
            m_solutions[m_selectedSolution].show = false;
        }
        m_selectedSolution = m_solutions.size() - 1;
        m_solutions[m_selectedSolution].show = true;
        m_configuration = m_solutions[m_selectedSolution].configuration;
    }

    int newSelection = m_selectedSolution;
    for(int i = 0; i < m_solutions.size(); ++i)
    {
        ImGui::RadioButton(m_solutions[i].name.c_str(), &newSelection, i);
    }
    if(newSelection != m_selectedSolution)
    {
        m_solutions[m_selectedSolution].show = false;
        m_solutions[newSelection].show = true;
        m_configuration = m_solutions[newSelection].configuration;
        m_selectedSolution = newSelection;
    }


    ImGui::NextColumn();
    renderPlots(m_PlotConfigsColumn1);
    ImGui::NextColumn();
    renderPlots(m_PlotConfigsColumn2);

    ImGui::PopFont();
    
    ImGui::End();
}