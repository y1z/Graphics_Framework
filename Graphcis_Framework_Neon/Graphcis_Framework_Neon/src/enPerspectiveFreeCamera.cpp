#include "enPerspectiveFreeCamera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"

enPerspectiveFreeCamera::enPerspectiveFreeCamera()
  :BasePerspectiveCamera()
{}

enErrorCode
enPerspectiveFreeCamera::init(const sPerspectiveCameraDesc& descriptor)
{
  m_position = descriptor.position;
  m_lookAt = descriptor.lookAtPosition;
  m_up = descriptor.upDir;

  m_fov = descriptor.fov;
  m_height = descriptor.height;
  m_width = descriptor.width;

  m_horizantalAngle = descriptor.horizantalAngle;
  m_verticalAngle = descriptor.verticalAngle;
  m_zAngle = descriptor.zAngle;

  this->updateMatrixes();
  return enErrorCode::NoError;
}

void
enPerspectiveFreeCamera::TranslateRelative(float x, float y, float z)
{
  m_position += -m_right * x;
  //m_lookAt += m_right * x;

  m_position += m_up * y;
  //m_lookAt += m_up * y;

  m_position += m_front * z;
  //m_lookAt += m_front * z;


  updateMatrixes();
}

void
enPerspectiveFreeCamera::rotateInYaw(float angleInDegs)
{
  m_horizantalAngle += glm::radians(angleInDegs);

  this->updateMatrixes();
}

void
enPerspectiveFreeCamera::rotateInPitch(float angleInDegs)
{
  m_verticalAngle += glm::radians(angleInDegs);
  this->updateMatrixes();
}

void
enPerspectiveFreeCamera::rotateInRoll(float angleInDegs)
{
  m_zAngle += glm::radians(angleInDegs);

  this->updateMatrixes();
}

void
enPerspectiveFreeCamera::rotateVector(const enVector2& rotationDir,
                                      float deltaTime)
{
  m_horizantalAngle += glm::radians(rotationDir.x * deltaTime);
  m_verticalAngle += glm::radians(rotationDir.y * deltaTime);

  updateMatrixes();
}

void 
enPerspectiveFreeCamera::updateDimensions(float newWidth, float newHeight)
{
  m_width = newWidth;
  m_height = newHeight;
  this->updateMatrixes();
}

bool
enPerspectiveFreeCamera::initDefault()
{
  sPerspectiveCameraDesc defaultValues;

  m_position = defaultValues.position;
  m_lookAt = defaultValues.lookAtPosition;
  m_up = defaultValues.upDir;

  m_fov = defaultValues.fov;
  m_height = defaultValues.height;
  m_width = defaultValues.width;

  m_horizantalAngle = defaultValues.horizantalAngle;
  m_verticalAngle = defaultValues.verticalAngle;

  this->updateMatrixes();

  return true;
}


void
enPerspectiveFreeCamera::updateMatrixes()
{
  m_front = enVector3
  (
    std::cosf(m_verticalAngle) * std::sinf(m_horizantalAngle),
    std::sinf(m_verticalAngle),
    std::cosf(m_verticalAngle) * std::cosf(m_horizantalAngle)
  );

  m_right = enVector3
  (
    std::sinf(m_horizantalAngle - (glm::pi<float>() / 2)),
    0.0f,
    std::cosf(m_horizantalAngle - (glm::pi<float>() / 2))
  );

  m_up = glm::normalize(glm::cross(m_front, m_right));

  m_projection = glm::perspectiveFovLH(m_fov, m_width, m_height, m_near, m_far);

  m_view = glm::lookAtLH(m_position, m_position + m_front, m_up);

  glm::mat4x4 zRotation(1.0f);
  const enVector4 FirstRow(std::cosf(m_zAngle), -std::sinf(m_zAngle), 0.0f, 0.0f);
  const enVector4 secondRow(std::sinf(m_zAngle), std::cosf(m_zAngle), 0.0f, 0.0f);

  zRotation = glm::row(zRotation, 0, FirstRow);
  zRotation = glm::row(zRotation, 1, secondRow);

  m_view *= zRotation;

}
