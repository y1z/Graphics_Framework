#include "BasePerspectiveCamera.h"

BasePerspectiveCamera::BasePerspectiveCamera()
  :m_view(1.0f),
  m_position(0.0f, 3.0f, -6.0f),
  m_lookAt(0.0f,0.0f,-1.0f),
  m_up(0.0f,1.0f,0.0f),
  m_right(1.0f,0.0f,0.0f),
  m_projection(1.0f),
  m_fov(1.04719f),
  m_height(-1337.0f),
  m_width (-1337.0f),
  m_far(100.0f),
  m_near(0.01f)
{
  m_front = glm::normalize(m_lookAt - m_position);
}

enMatrix4x4
BasePerspectiveCamera::getView() const
{
  return m_view;
}

enMatrix4x4
BasePerspectiveCamera::getProjection() const
{
  return m_projection;
}

enVector3
BasePerspectiveCamera::getPosition() const
{
  return m_position;
}

enVector3 
BasePerspectiveCamera::getViewPosition() const
{
  return m_lookAt;
}

enVector3
BasePerspectiveCamera::getUpDir() const
{
  return m_up;
}

enVector3 
BasePerspectiveCamera::getFront() const
{
  return m_front;
}

enVector3
BasePerspectiveCamera::getRight() const
{
  return m_right;
}

enVector3 
BasePerspectiveCamera::getDirction() const
{
  return m_direction;
}

float 
BasePerspectiveCamera::getFov() const
{
  return  m_fov;
}

float
BasePerspectiveCamera::getHeight() const
{
  return m_height;
}

float
BasePerspectiveCamera::getWidth() const
{
  return m_width;
}

float 
BasePerspectiveCamera::getFar() const
{
  return  m_far;
}

float
BasePerspectiveCamera::getNear() const
{
  return m_near;
}

float 
BasePerspectiveCamera::getHorizontalAngle() const
{
  return m_horizantalAngle;
}

float 
BasePerspectiveCamera::getVerticalAngle() const
{
  return m_verticalAngle;
}

float 
BasePerspectiveCamera::getSpeed() const
{
  return m_speed;
}

void 
BasePerspectiveCamera::setViewMatrix(const enMatrix4x4& matrix)
{
  m_view = matrix;
}

void 
BasePerspectiveCamera::setProjectionMatrix(const enMatrix4x4& matrix)
{
  m_projection = matrix;
}

void 
BasePerspectiveCamera::setPosition(float x, float y, float z)
{
  m_position = enVector3(x, y, z);
}

void 
BasePerspectiveCamera::setPosition(const enVector4& newPosition)
{
  m_position = newPosition;
}

void
BasePerspectiveCamera::setViewPosition(float x, float y, float z)
{
  m_lookAt = enVector3(x, y, z);
}

void 
BasePerspectiveCamera::setViewPosition(const enVector3& newPosition)
{
  m_lookAt = newPosition;
}

void 
BasePerspectiveCamera::setUpDir(float x, float y, float z)
{
  m_up = enVector3(x, y, z);
}

void 
BasePerspectiveCamera::setUpDir(const enVector3& newUp)
{
  m_up = newUp;
}

void
BasePerspectiveCamera::setFront(float x, float y, float z)
{
  m_front = enVector3(x, y, z);
}

void 
BasePerspectiveCamera::setFront(const enVector3& newFront)
{
  m_front = newFront;
}

void 
BasePerspectiveCamera::setRight(float x, float y, float z)
{
  m_right = enVector3(x, y, z);
}

void 
BasePerspectiveCamera::setRight(const enVector3& newRight)
{
  m_right = newRight;
}

void 
BasePerspectiveCamera::setDirection(float x, float y, float z)
{
  m_direction = enVector3(x, y, z);
}

void
BasePerspectiveCamera::setDirection(const enVector3& newDirection)
{
  m_direction = newDirection;
}

void 
BasePerspectiveCamera::setHeight(float newHeight)
{
  m_height = newHeight;
}

void 
BasePerspectiveCamera::setWidth(float newWidth)
{
  m_width = newWidth;
}

void 
BasePerspectiveCamera::setFar(float newFar)
{
  m_far = newFar;
}

void
BasePerspectiveCamera::setNear(float newNear)
{
  m_near = newNear;
}

void 
BasePerspectiveCamera::setHorzantalAngleDeg(float newAngleInDegrees)
{
  m_horizantalAngle = glm::radians(newAngleInDegrees);
}

void 
BasePerspectiveCamera::setHorzantalAngleRad(float newAngleInRandians )
{
  m_horizantalAngle = newAngleInRandians;
}

void 
BasePerspectiveCamera::setVerticalAngleDeg(float newAngleInDegrees)
{
  m_verticalAngle = glm::radians(newAngleInDegrees);
}

void 
BasePerspectiveCamera::setVerticalAngleRad(float newAngleInRadians)
{
  m_verticalAngle = newAngleInRadians;
}

void 
BasePerspectiveCamera::setSpeed(float newSpeed)
{
  m_speed = newSpeed;
}
