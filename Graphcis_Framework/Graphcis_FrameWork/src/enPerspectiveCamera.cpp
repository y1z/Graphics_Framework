#include "../include/enPerspectiveCamera.h"

enPerspectiveCamera::enPerspectiveCamera()
  :BasePerspectiveCamera()
{
}

enErrorCode
enPerspectiveCamera::init(const sPerspectiveCameraDesc& descriptor)
{
  m_position = descriptor.position;
  m_lookAt = descriptor.lookAtPosition;
  m_right = descriptor.rightDir;
  m_up = descriptor.upDir;

  m_fov = descriptor.fov;
  m_height = descriptor.height;
  m_width = descriptor.width;

  this->updateMatrixes();
  return enErrorCode::Sucessful;
}

void
enPerspectiveCamera::TranslateRelative(float x, float y, float z)
{
  m_position += m_right * x;
  m_position += m_up * y;
  m_position += m_front * z;

  updateMatrixes();
}

void
enPerspectiveCamera::updateMatrixes()
{
  m_projection = glm::perspectiveFovLH(m_fov, m_width, m_height, m_near, m_far);

  m_view = glm::lookAtLH(m_position, m_lookAt, m_up);

  m_front = (m_lookAt - m_position);
}
