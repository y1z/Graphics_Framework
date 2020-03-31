# Graphics_Framework
This repository will house a graphics framework that can chose between 2 api's open_gl and directX

## External libs 
- DirectXTK :  https://github.com/Microsoft/DirectXTK
    - include in the project as a static lib.


- SOIL2 : https://github.com/SpartanJ/SOIL2
    - included in the project as a static lib.

- GLM : https://github.com/g-truc/glm
    - NOT include in the project 
    - *only requires* that you have the headers
    - path is __`$(DEVLIBS)glm`__ 

- GLFW : https://github.com/glfw/glfw
    - included in the project as a dynamic lib.
    - ALL the '.dll' files are already in the places where the '.exe' will be generated. 

- ImGui : https://github.com/ocornut/imgui
    - included in the project. 

- Assimp : https://github.com/assimp/assimp
    - included in the project.

- GLEW : http://glew.sourceforge.net/ 
	- NOT included in the project 
    - is used as a static lib 
    - lib path is __`$(DEVLIBS)glew\lib\$(PlatformTarget) `__


## Environment variable
 - DEVLIBS : `C:\c++libs\`
    - aka in the folder where **YOUR** libs.
 - PlatformTarget : in windows this should be x64 or x86 depending on the configuration.
