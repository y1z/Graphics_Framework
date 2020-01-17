#include "../include/enPerspectiveCamera.h"
#include "glm/gtc/matrix_transform.hpp"

enPerspectiveCamera::enPerspectiveCamera()
  :BasePerspectiveCamera()
{
}

enErrorCode
enPerspectiveCamera::init(const sPerspectiveCameraDesc& descriptor)
{
  m_position = descriptor.position;
  m_lookAt = descriptor.lookAtPosition;
  m_up = descriptor.upDir;

  m_fov = descriptor.fov;
  m_height = descriptor.height;
  m_width = descriptor.width;

  m_horizantalAngle = descriptor.horizantalAngle;
  m_verticalAngle = descriptor.verticalAngle;

  m_direction = enVector3(std::cosf(m_verticalAngle) * std::sinf(m_horizantalAngle),
    std::sinf(m_verticalAngle),
    std::cosf(m_verticalAngle) * std::cosf(m_horizantalAngle));




  this->updateMatrixes();
  return enErrorCode::Sucessful;
}

void
enPerspectiveCamera::TranslateRelative(float x, float y, float z)
{
  m_position += m_right * x;
  m_lookAt += m_right * x;

  m_position += m_up * y;
  m_lookAt += m_up * y;

  m_position += m_front * z;
  m_lookAt += m_front * z;


  updateMatrixes();
}

void
enPerspectiveCamera::rotateVector(float x, float y, float z, float angleInDegrees)
{
  m_view = glm::rotate(m_view, glm::radians(angleInDegrees), enVector3(x, y, z));

  updateMatrixes();
}

void
enPerspectiveCamera::rotateFrontVectorDeg(float angleInDegrees)
{
  const float radians = glm::radians(angleInDegrees);

  float tempY = (m_front.y * std::cosf(radians)) - (m_front.z * std::sinf(radians));
  float tempZ = (m_front.y * std::sinf(radians)) + (m_front.z * std::cosf(radians));

  m_front.y = tempY;
  m_front.z = tempZ;

  enVector3 difference = (m_lookAt - m_position);



}

void
enPerspectiveCamera::updateMatrixes()
{
  m_projection = glm::perspectiveFovLH(m_fov, m_width, m_height, m_near, m_far);

  m_view = glm::lookAtLH(m_position, m_lookAt, m_up);


  m_front = glm::normalize(m_lookAt - m_position);

  m_right = glm::normalize(glm::cross(m_up, m_front));
}
