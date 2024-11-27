# URepair

## Description

URepair is a comprehensive desktop application designed to facilitate the management of freelance repair jobs. Built using C++ with OpenGL for graphical rendering, ImGui for the graphical user interface, and SQLite for database management, URepair provides a streamlined platform for clients, contractors, and administrators to manage profiles, bookings, and job-related tasks.

This project was authored by **Isaac Latta**, **Matia Landry**, and **Michael Baudin**. The provided instructions are tailored for building and running the project on **Windows**, as per the expected platform for evaluation.

---

## Notes for our Instructor
- The project has been designed and tested on Windows with Visual Studio.
- All dependencies are included in the project, except for OpenGL libraries with is assumed to be found on nearly all systems.
- To evaluate functionality, navigate through the views as a client, contractor, or admin after logging in.
- Comprehensive comments in the source code and documentation facilitate understanding of the project structure.
- We have included an executable of the project to save you from building.
- If you prefer building yourself and assuming you are running the project on Windows, we have included the .sln, .vcxproj, and .vcxproj.filters files. If you would prefer to build completely from scratch we have included a small build guide below.


## Prerequisites

The project includes all necessary headers aside from OpenGL-related dependencies. Follow the instructions below to ensure your system is ready for building and running URepair.

### **Windows**

1. **Visual Studio 2019 or 2022**:
   - Ensure the **Desktop Development with C++** workload is installed.
   - Install Visual Studio via the [Visual Studio Installer](https://visualstudio.microsoft.com/).

2. **CMake**:
   - Download and install from [here](https://cmake.org/download/).
   - Be sure to select the option to add CMake to the **PATH** variable during installation.
   - If you missed this step, follow these instructions:
     1. Open the Start Menu and search for **"Environment variables"**.
     2. Click **"Edit system environment variables"**.
     3. In the **System Properties** window, select **"Environment Variables"**.
     4. Find **"PATH"** under **System Variables** and click **"Edit"**, then click **"New"**.
     5. Add the path to the CMake installation's `bin` folder (e.g., `C:\Program Files\CMake\bin`).
     6. Verify installation by running the following in PowerShell or Command Prompt:
        ```bash
        cmake --version
        ```

3. **OpenGL**:
   - Ensure OpenGL support is available on your system. Most modern systems include OpenGL drivers by default.

---

## Building the Project

### **Windows**

1. Open PowerShell or Command Prompt and navigate to the **URepair** directory, for example:
   ```bash
   cd C:\Users\<your username>\path\to\URepair
   ```
2. Create the **build** directory if it doesnt already exist:
	```bash
	mkdir build
	cd build
	```
3. Generate the Visual Studio project files using CMake:
	- For Visual Studio 2019
	```bash
	cmake .. -G "Visual Studio 16 2019"
	```
	- For Visual Studio 2022
	```bash
	cmake .. -G "Visual Studio 17 2022"
	```
4. Open the .sln file found in the build directory
5. Build and run the project:
	- Press **F5** or click the green **Start** button (arrow) in Visual Studio.

### **Linux**

1. Open the the terminal and navigate to the **URepair** directory, for example:
	```bash
	cd ~/URepair
	```
2. Create the **build** directory if it doesnt already exist:
	```bash
	mkdir build
	cd build
	```
3. Generate the build files:
	```bash
	cmake ..
	```
4. Build the project:
	```bash
	make
	```
5. Run the project:
	```bash
	./URepair
	```

## Features 
- Client View: Search and book talents, manage profiles, and view job statuses.
- Contractor View: Accept or decline job requests, manage profiles, and view active jobs.
- Admin View: Run custom queries and manage the database.
- Profile Management: Securely update usernames, passwords, and other profile details.
- SQLite Integration: Persistent storage of user data, jobs, and transactions.

