#include "cTransform.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"

cTransform::cTransform()
{
}

void
cTransform::moveTransform(float x, float y, float z, float w, float deltaTime)
{
  glm::vec3 moveVector(x, y, z);
  moveVector *= deltaTime;
  m_transformMatrix = glm::translate(m_transformMatrix, moveVector);
}

bool
cTransform::Init() noexcept
{
  m_transformMatrix = glm::identity<glm::mat4>();
  return true;
}

void
cTransform::rotateInXAxis(float AngleInDegrees)
{
  m_transformMatrix = glm::rotate(m_transformMatrix, glm::radians(AngleInDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
}

void
cTransform::rotateInYAxis(float AngleInDegrees)
{
  m_transformMatrix = glm::rotate(m_transformMatrix, glm::radians(AngleInDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
}

void
cTransform::rotateInZAxis(float AngleInDegrees)
{
  m_transformMatrix = glm::rotate(m_transformMatrix, glm::radians(AngleInDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
}

void
cTransform::resetToIdentity()
{
  m_transformMatrix = glm::mat4(1.0f);
}

void
cTransform::shearTransformInXAxis(float shearingAmount)
{
  glm::mat4 shearMatrix(1.0f);
  glm::vec4 ShearingVec(1.0f, shearingAmount, 0.0f, 0.0f);
  shearMatrix = glm::column(shearMatrix, 0, ShearingVec);
  m_transformMatrix *= shearMatrix;
}

void
cTransform::shearTransformInYAxis(float shearingAmount)
{
  glm::mat4 shearMatrix(1.0f);
  glm::vec4 ShearingVec(0.0f, 1.0f, shearingAmount, 0.0f);
  shearMatrix = glm::column(shearMatrix, 1, ShearingVec);
  m_transformMatrix *= shearMatrix;
}

void
cTransform::shearTransformInZAxis(float shearingAmount)
{
  glm::mat4 shearMatrix(1.0f);
  glm::vec4 ShearingVec(0.0f, shearingAmount, 1.0f, 0.0f);
  shearMatrix = glm::column(shearMatrix, 2, ShearingVec);
  m_transformMatrix *= shearMatrix;
}

void
cTransform::reflectTransfromInXAxis(float Amount)
{
  glm::vec4 reflectionColum(-Amount, 0.0f, 0.0f, 0.0f);
  m_transformMatrix = glm::column(m_transformMatrix, 0, reflectionColum);
}

void
cTransform::reflectTransfromInYAxis(float Amount)
{
  glm::vec4 reflectionColum(0.0f, -Amount, 0.0f, 0.0f);
  m_transformMatrix = glm::column(m_transformMatrix, 1, reflectionColum);
}

void
cTransform::reflectTransfromInZAxis(float Amount)
{
  glm::vec4 reflectionColum(0.0f, 0.0f, -Amount, 0.0f);
  m_transformMatrix = glm::column(m_transformMatrix, 2, reflectionColum);
}

enMatrix4x4
cTransform::getMatrix() const
{
  return  m_transformMatrix;
}


