# URepair

## Description

## Prerequisites

### Windows
- **Visual Studio 2019 or 2022**: Ensure the **Desktop Development with C++** workload is installed.
	- This can be installed through the Visual Studio Installer. 
- **CMake**: cmake will need to be installed, found [here](https://cmake.org/download/).

### Linux
- **Cmake**: Install via your package manager, for example on ubuntu:
```bash
sudo apt install cmake
```
- **g++/gcc**: Standard C++ compiler, can be installed from the [website](https://gcc.gnu.org/), or via the package manager:
```bash
sudo apt install g++ gcc
```
- **GLFW**, **GLAD**, **OpenGL**: Installed via the package manager:
```bash
sudo apt install libglfw3-dev libgl1-mesa-dev
```


## Building the Project

### Windows
- Open powershell or the command prompt and navigate to the **URepair** directory:
```
cd C:\Users\username\path\to\URepair
```
- Navigate to the **build** directory:
```
cd build
```
- If it is not found create via:
```bash 
mkdir build
cd build
```
- Now generate the the Visual Studio project files.
	- For Visual Studio 2019
	``` bash
	cmake .. -G "Visual Studio 16 2019"
	```
	- For Visual Studio 2022
	```bash
	cmake .. -G "Visual Studio 17 2022"
	```
- Now open the .sln file found in the **build** directory.
### Linux
- Navigate to the **URepair/build** directory, for example in the home directory:
```bash 
cd ~/URepair
```
- Navigate to the **build**.
```bash 
cd build
```
- Build the project:
```bash
cmake .. && make
```

