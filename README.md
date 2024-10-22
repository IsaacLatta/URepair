# URepair

## Description

## Prerequisites

### Windows
- **Visual Studio 2019 or 2022**: Ensure the **Desktop Development with C++** workload is installed.
	- This can be installed through the Visual Studio Installer. 
- **CMake**: cmake will need to be installed, the installer is found [here](https://cmake.org/download/).
- **NOTE**: Be sure to select the option to add cmake to the **PATH** variable. If you missed this option: 
	1. Open the start menu and search for **"Environment variables"**.
	2. Click on **"Edit system environment"**.
	3. In **System Properties** window, select **"Environment Variables"**.
	4. Find **"PATH"** under **System Variables** and click **"Edit"** then click **"New"**.
	5. Paste the path to the cmake installation bin folder (e.g., C:\Program Files\CMake\bin).
	6. To verify, open powershell or the command prompt and run:
	```bash
	cmake --version
	``` 

### Linux
- **CMake**: Install via your package manager, for example on ubuntu:
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
- Now open the .sln file found in the **build** directory with Visual Studio.
- To run, press **F5** or the little green start button(arrow).
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

