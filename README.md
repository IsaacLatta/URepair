# URepair

## Description

URepair is a comprehensive desktop application designed to facilitate the management of freelance repair jobs. Built using C++ with OpenGL for graphical rendering, ImGui for the graphical user interface, and SQLite for database management, URepair provides a streamlined platform for clients, contractors, and administrators to manage profiles, bookings, and job-related tasks.

This project was authored by **Isaac Latta**, **Matia Landry**, and **Michael Baudin**. The provided instructions are tailored for building and running the project on **Windows**, as per the expected platform for evaluation.

---

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

1. Open PowerShell or Command Prompt and navigate to the **URepair** directory:
   ```bash
   cd C:\Users\username\path\to\URepair
