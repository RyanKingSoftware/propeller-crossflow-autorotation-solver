#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <GLFW/glfw3.h>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Window {
    public:
        Window(const std::string& windowTitle = "Market HUD", const int& width = 1280, const int& height = 720);
        ~Window();
        void run();
        void close() noexcept;
        ImGuiID getDockspaceId() noexcept;

    private:
        virtual void update() = 0;
        static void glfw_error_callback(int error, const char* description);
    
    private:
        GLFWwindow* m_window;
        const ImVec4 m_clearColor = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        ImGuiID m_dockspaceId = 0;
        bool m_shouldClose = false;
};

#endif // _WINDOW_H_