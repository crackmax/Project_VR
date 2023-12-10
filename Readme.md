# Planet project for info h-502

The cmake files used are inspired from the exercise sessions hence, from Glitter https://github.com/Polytonic/Glitter/blob/master/CMakeLists.txt

## Getting Started

Make sure that git is installed on your computer and clone or fork this repo.

## Video and report
You will find my report and a short video of the animation in /project/report.

Feel free to look at the video and read my report

## Data and models
The models are stored in /project/objects.

The textures are in /project/images.

The shaders are in /project/shaders

Feel free to look at them.
### Setup an IDE that support CMake build (recommanded option)
the following instructions come from the exercise session readme. Since I am using the same make file, the steps are the same. I used visual studio 2022 on windows with the desktop development kid with C++
Cmake tools for windows. I recommend this approach as it is very simple. When cloned or forked, right click on the CMakeList.txt and click configure info502-project. This should configure the project with the correct libraries. Then choose Project.exe and execute!

The project uses the following libraries:
- STB for images
- glm (maths)
- glfw (context manager)
- glad (pointer manager)
There are also some files such as shader.h that has been inspired by https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
Camera.h has been re-used from the exercise sessions
Object.h has been partly taken from the exercise sessions but it has been majorly modified to fit the needs of the project.

#### Visual Studio (Windows only)  
- Install Visual Studio (Community) if not already done : https://visualstudio.microsoft.com/downloads/ 
- Install the component "Desktop development with C++" during the installation process and be sure that "C++ Cmake tools for windows" is installed.
- Open Visual Studio and select "Open a local folder" then select the folder that you just clone.

#### Visual studio code (Cross-platform)
- Install Visual Studio Code : https://code.visualstudio.com/download
- Install CMake : https://cmake.org/download/
- Install the VSC extension for C++ and CMake tools.
- In VSC, select your C++ compiler, mode (Debug, Release, ...) then run ```Cmake:Configure```
- Build your code then run it.
- You can check this tutorial : https://code.visualstudio.com/docs/cpp/cmake-linux
It's fairly similar for MacOs.

### Those next one are possibilities, but please read futher to use them as I am not familiar with them.
#### Qt creator

- Install cmake if not already done: https://cmake.org/download/ or `sudo apt-get install cmake` (Linux)
- Install Qt creator (open-source): https://www.qt.io/download-open-source 
- Open Qt Creator and select `Open project`
- Chose the default configuration and press configure project

#### CLion

- Install CLion.
- Check out the tutorial for cmake with CLion : https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html
