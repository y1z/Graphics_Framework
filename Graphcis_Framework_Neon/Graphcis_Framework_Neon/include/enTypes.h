#pragma once
#include <iostream>
// TODO : Remove when you make your own math library
#define USE_GLM_MATH  1

#if USE_GLM_MATH == 1
#include "glm/glm.hpp"

using enMatrix4x4 = glm::mat4x4;
using enMatrix3x3 = glm::mat3x3;
using enVector4 = glm::vec4;
using enVector3 = glm::vec3;
using enVector2 = glm::vec2;


#elif USE_ENTROPY_MATH == 1
#endif //GLM_API 


static inline std::ostream&
operator <<(std::ostream& Stream, const enVector2& vec2) {
  return Stream << '<' << vec2.x << ", " << vec2.y << '>';
}

static inline std::ostream&
operator <<(std::ostream& Stream, const enVector3& vec3) {
  return Stream << '<' << vec3.x << ", " << vec3.y << ", " << vec3.z << '>';
}

static inline std::ostream&
operator <<(std::ostream& Stream, const enVector4& vec4) {
  return Stream << '<' << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w << '>';
}

/**
* Struct type defines
*/

#define ActiveVertex_t sFullVertex


struct SimpleVertex
{
  glm::vec4 Pos;
  glm::vec2 Tex;
};



struct /*alignas(sizeof(glm::vec4)) */sFullVertex
{
  glm::vec4 Pos;
  glm::vec2 Tex;
  glm::vec3 Normal;
};

struct viewMatrix
{
  glm::mat4x4 mView;
};

struct projectionMatrix
{
  glm::mat4x4 mProjection;
};

struct ConstBufferWorldColor
{
  glm::mat4x4 mWorld;
  glm::vec4 vMeshColor;
};

struct alignas(sizeof(float) * 4) sLightDirs
{
  enVector3 m_lambertDir;
};

struct /*alignas(sizeof(float) * 4)*/ sLightIntensity 
{
  float m_lambertIntensity = 0.5f;
};
