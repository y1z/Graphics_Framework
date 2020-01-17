#pragma once
// TODO : Remove when you make your own math library
#define USE_GLM_MATH  1

#if USE_GLM_MATH == 1
#include "glm/glm.hpp"

using enMatrix4x4 = glm::mat4x4;
using enVector4 = glm::vec4;
using enVector3 = glm::vec3;
using enVector2 = glm::vec2;

#elif USE_ENTROPY_MATH == 1
#endif //GLM_API 

