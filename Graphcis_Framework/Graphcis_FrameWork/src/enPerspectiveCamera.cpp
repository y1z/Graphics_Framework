#include "../include/enPerspectiveCamera.h"

enPerspectiveCamera::enPerspectiveCamera()
  :BasePerspectiveCamera()
{
}

enErrorCode 
enPerspectiveCamera::init(const PerspectiveCameraDesc& descriptor)
{
  m_position = descriptor.position;
  m_lookAt = descriptor.lookAtPosition;
  m_front = descriptor.frontDir;
  m_right = descriptor.rightDir;
  m_up = descriptor.upDir;

  m_fov = descriptor.fov;
  m_height = descriptor.height;
  m_width = descriptor.width;

  //glm::perspectiveFovLH(m_fov,m_width,m_height,);

  return enErrorCode::Sucessful;
}
