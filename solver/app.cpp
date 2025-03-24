#include "app.h"
#include "imgui.h"
#include "implot.h"

App::App() : Window("Propeller Autorotation Solver", 1200, 800) {}

App::~App() {}

void App::update()
{
    // Get the main viewport
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    // Set the window position and size to match the viewport
    ImGui::SetNextWindowPos(viewport->Pos); // Dock to the top-left of the viewport
    ImGui::SetNextWindowSize(viewport->Size); // Make the window match the size of the viewport

    // Create the ImGui window
    ImGui::Begin("Autorotation", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Add content to the window
    ImGui::Text("Autorotation");

    if (ImPlot::BeginPlot("Empty Plot"))
    {
        //
        ImPlot::EndPlot();
    }

    // End the ImGui window
    ImGui::End();
}