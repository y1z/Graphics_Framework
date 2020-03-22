#pragma once

#if OPENGL
   #ifndef  __glew_h__ 
#include "GL/glew.h"
#include "GLFW/glfw3.h"
   #endif //__glew_h__

#elif DIRECTX
#include <d3d11.h>
#include <d3dcompiler.h>
#endif // OPENGL

#include "MacroslessWindow.h"
#include "en_defs.h"
#include "glm/glm.hpp"
#include "enTypes.h"



#include <cassert>
