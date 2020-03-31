#include "enFirstPersonCamera.h"
#include "util/Header.h"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "..\include\enFirstPersonCamera.h"

#include<algorithm>

enFirstPersonCamera::enFirstPersonCamera()
  :BasePerspectiveCamera()
{}

enErrorCode
enFirstPersonCamera::init(const sFirstPersonCameraDesc& descriptor)
{
  m_desc = descriptor;

  m_horizantalAngle = m_desc.m_horizantalAngle;
  m_verticalAngle = m_desc.m_verticalAngle;

  m_position = enVector3(-25.0f, 0.0f, -10.0f);

  m_height = m_desc.height;
  m_width = m_desc.width;

  return enErrorCode::NoError;
}

void
enFirstPersonCamera::updateMatrixes()
{
  m_front = enVector3
  (
    std::cosf(m_verticalAngle) * std::sinf(m_horizantalAngle),
    std::sinf(m_verticalAngle),
    std::cosf(m_verticalAngle) * std::cosf(m_horizantalAngle)
  );

  m_desc.frontMovemientDir = enVector3(m_front.x, 0.0f, m_front.z);

  m_right = enVector3
  (
    std::sinf(m_horizantalAngle - (glm::pi<float>() / 2)),
    0.0f,
    std::cosf(m_horizantalAngle - (glm::pi<float>() / 2))
  );


  m_desc.rightMovemientDir = enVector3(m_right.x, 0.0f, m_right.z);

  m_up = glm::normalize(glm::cross(m_front, m_right));

  m_projection = glm::perspectiveFovLH(m_fov, m_width, m_height, m_near, m_far);

  m_view = glm::lookAtLH(m_position, m_position + m_front, m_up);

  glm::mat4x4 zRotation(1.0f);
  const enVector4 FirstRow(std::cosf(m_zAngle), -std::sinf(m_zAngle), 0.0f, 0.0f);
  const enVector4 secondRow(std::sinf(m_zAngle), std::cosf(m_zAngle), 0.0f, 0.0f);

  zRotation = glm::row(zRotation, 0, FirstRow);
  zRotation = glm::row(zRotation, 1, secondRow);

  m_projection *= zRotation;

}


void
enFirstPersonCamera::TranslateRelative(float x, float y, float z)
{
  m_position += -m_desc.rightMovemientDir * x;
  //m_lookAt += m_right * x;

//  m_position += m_desc.upMovemientDir * y;
  //m_lookAt += m_up * y;

  m_position += m_desc.frontMovemientDir * z;
  //m_lookAt += m_front * z;

  this->updateMatrixes();
}

void
enFirstPersonCamera::rotateInYaw(float angleInDegs)
{
  m_horizantalAngle += glm::radians(angleInDegs);
  this->updateMatrixes();
}


void
enFirstPersonCamera::rotateInPitch(float angleInDegs)
{
  this->m_verticalAngle += glm::radians(angleInDegs);

  m_verticalAngle = glm::clamp(m_verticalAngle,
                               m_desc.MinPitchAngle,
                               m_desc.MaxPitchAngle);

  this->updateMatrixes();
}

void
enFirstPersonCamera::rotateInRoll(float angleInDegs)
{
  m_zAngle += glm::radians(angleInDegs);

  m_zAngle = glm::clamp(m_zAngle,
                        m_desc.MinRollAngle,
                        m_desc.MaxRollAngle);

  this->updateMatrixes();
}

void
enFirstPersonCamera::rotateVector(const enVector2& rotationDir, float deltaTime)
{

  m_horizantalAngle += glm::radians(rotationDir.x * deltaTime);
  m_verticalAngle += glm::radians(rotationDir.y * deltaTime);
  m_verticalAngle = glm::clamp(m_verticalAngle,
                               m_desc.MinPitchAngle,
                               m_desc.MaxPitchAngle);


  this->updateMatrixes();
}

bool  
enFirstPersonCamera::initDefault()
{
  m_desc = sFirstPersonCameraDesc();

  this->updateMatrixes();
  return true;
}

void 
enFirstPersonCamera::updateDimensions(float newWidth, float newHeight)
{
  m_height = newHeight;
  m_width = newWidth;
  this->updateMatrixes();
}
