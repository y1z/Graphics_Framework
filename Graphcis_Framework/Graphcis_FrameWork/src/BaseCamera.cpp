#include "BaseCamera.h"

BaseCamera::BaseCamera()
  : m_view(1.0f),
  m_projection(1.0f),
  m_position(0.0f, 3.0f, -6.0f),//eye
  m_viewPosition(0.0f, 0.0f, -1.0f),// at
  m_up(0.0f, 1.0f, 0.0f)
{
  m_front = glm::normalize(m_viewPosition - m_position);
}
