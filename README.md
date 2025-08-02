# Propeller Crossflow Autorotation Solver

Abstract—The design of VTOL aircraft, particularly
eVTOL aircraft, often relies on the use of multiple electrically
powered thrust-vectoring propellers to meet the high thrust
requirements of vertical flight. Many configurations employ aft-
mounted propellers fixed in a vertical orientation, dedicated
exclusively to providing extra lift during vertical take-off and
landing. These propellers, left unpowered at cruise, experience
crossflow—air moving parallel to the plane of rotation—which
in turn induces autorotation. Modeling the aerodynamic
behavior resulting from this crossflow-induced autorotation
requires a modified mathematical framework—an adaptation of
blade element theory—formulated specifically for crossflow
conditions. This report presents the formulation of such a model
and introduces software developed to solve it numerically,
which, given propeller geometry and initial crossflow conditions,
computes both transient and steady-state solutions for the
propeller’s kinematic and aerodynamic states. These solutions
inform the design and optimization of propeller systems for
VTOL and eVTOL aircraft by providing insight into design
parameters that drive both desired and undesired aerodynamic
behavior. Notably, they reveal a strong positive correlation
between blade pitch and cruise drag, suggesting that variable-
pitch propellers, by leveling the blades at cruise, may offer
advantages in improving aerodynamic efficiency.

## Authors

- [@RyanKingSoftware](https://github.com/RyanKingSoftware)

## Supported Platforms

The following platforms are officially supported and have been tested. While the code may compile and run on other systems, only the platforms below are verified:

- Windows x64  
- Windows x32  
- Linux (Debian) x64  
- Linux (Debian) x32

## Windows Requirements

The following tools are required to clone, configure, build, and install the project on Windows:

- [**Git**](https://git-scm.com): Required to clone the repository.  
- [**CMake**](https://cmake.org): Used for configuring the build system, installation, and packaging.  
- [**MSVC**](https://visualstudio.microsoft.com/vs/features/cplusplus/): Recommended for compiling the project on Windows.

## Linux Requirements

Use the following command to install all dependencies required to clone, configure, build, install, and package the project on Debian-based Linux systems:

```bash
sudo apt install git cmake g++ gcc \
libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
libgl1-mesa-dev libglu1-mesa-dev \
libglfw3-dev libglew-dev \
libpng-dev libjpeg-dev \
build-essential
```
## Cloning with Github

This project uses Git submodules for dependency management. If you'd like to use the provided versions of all libraries, clone the repository recursively:

```bash
git clone --recurse-submodules https://github.com/RyanKingSoftware/propeller-crossflow-autorotation-solver.git
```
## How to Build

We recommend creating a build directory within the project folder and using CMake to configure and build the project. This will generate a bin folder containing the compiled executable.

```bash
  cd propeller-crossflow-autorotation-solver
  mkdir build
  cd build
  cmake -DCMAKE_BUILD_TYPE=Release ..
  cmake --build . --config Release
```
## Installation
To install the built application, use the following CMake command:

```bash
cmake --install . --config Release
```
