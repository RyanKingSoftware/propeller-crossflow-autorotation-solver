cmake_minimum_required(VERSION 3.25)
project(solver VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Release)

find_package(OpenGL REQUIRED)

set(SOLVER_INCLUDE_DIR "solver")
file(GLOB_RECURSE SOLVER_SOURCES "${SOLVER_INCLUDE_DIR}/*.cpp" "${SOLVER_INCLUDE_DIR}/*.h") 

set(IMGUI_INCLUDE_DIR "imgui")
file(GLOB IMGUI_SOURCES "${IMGUI_INCLUDE_DIR}/*.h" "${IMGUI_INCLUDE_DIR}/*.cpp")

set(IMGUI_BACKENDS_INCLUDE_DIR "imgui/backends")
set(IMGUI_BACKENDS_SOURCES 
    "${IMGUI_BACKENDS_INCLUDE_DIR}/imgui_impl_glfw.cpp" 
    "${IMGUI_BACKENDS_INCLUDE_DIR}/imgui_impl_opengl3.cpp" 
    "${IMGUI_BACKENDS_INCLUDE_DIR}/imgui_impl_opengl3_loader.h"
)

set(IMPLOT_INCLUDE_DIR "implot")
file(GLOB IMPLOT_SOURCES "${IMPLOT_INCLUDE_DIR}/*.h" "${IMPLOT_INCLUDE_DIR}/*.cpp")


set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF)
add_subdirectory(glfw)

add_executable(${PROJECT_NAME} 
    ${SOLVER_SOURCES}
    ${IMGUI_SOURCES}
    ${IMGUI_BACKENDS_SOURCES}
    ${IMPLOT_SOURCES}
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MT)
    set_target_properties(glfw PROPERTIES COMPILE_OPTIONS "/MT")
endif()


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${SOLVER_INCLUDE_DIR}
    ${OpenGL_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${IMGUI_BACKENDS_INCLUDE_DIR}
    ${IMPLOT_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OPENGL_LIBRARIES}
    glfw
)
