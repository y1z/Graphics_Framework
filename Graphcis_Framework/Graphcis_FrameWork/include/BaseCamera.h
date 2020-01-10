#pragma once
#include "cTransform.h"

class BaseCamera
{
public:
  BaseCamera();
  virtual ~BaseCamera() = default;

protected:
/**
*  @brief : represent the following , where the camera is, how it's rotated .
*/
  enMatrix4x4 m_view;
/**
*  @brief : represent 
*/
  enMatrix4x4 m_projection;
/**
*  @brief : represent where the camera is in world space(equivalent to eye).
*/
  enVector3 m_position;
/**
*  @brief : represent WHERE the camera is looking (equivalent to At).
*/
  enVector3 m_viewPosition;
/**
*  @brief : it's the direction that represents 'up' relative to the camera
*/
  enVector3 m_up;
/**
*  @brief :it's the direction  that the camera is pointing too.
*/
  enVector3 m_front;
/**
*  @brief : used to control how wide the 'lens' of the camera is.
*/
  float m_fov{ 1.04719f };// 60 degrees in radians
/**
*  @brief : used to control how fast the camera moves in world-space.
*/
  float m_speed{ 10.0f };

};

