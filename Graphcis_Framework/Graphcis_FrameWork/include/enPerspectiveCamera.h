#pragma once
#include "BasePerspectiveCamera.h"
#include "glm/gtc/matrix_transform.hpp"

#include "en_defs.h"

/**
* @brief : the descriptor for this camera, should only require that
*          you fill out the following 'upDir' , 'lookAtPosition' ,'rightDir', 'frontDir'
*
*/
struct PerspectiveCameraDesc
{
  /** describes where the camera is in world space*/
  enVector3 position = enVector3(0.0f, 0.0f, 0.0f);

  /** describes where the camera is looking At (aka focal point)*/
  enVector3 lookAtPosition;

  /** describes the 'Up' direction relative to the camera*/
  enVector3 upDir;

  /** describes the 'right' direction relative to the camera*/
  enVector3 rightDir;

  /** describes the 'front' direction relative to the camera*/
  enVector3 frontDir;

  /** describes how tall the projection is*/
  float height = -1337.0f;

  /** describes how wide the projection is*/
  float width = -1337.0f;

  /** describes the field of view*/
  float fov = 1.04719f;
};

/**
* @brief : this is a perspective camera
*/
class enPerspectiveCamera final :
  public BasePerspectiveCamera
{
public:
  enPerspectiveCamera();
  ~enPerspectiveCamera() = default;
public:
  /**
  * @brief : used to initialize the camera with user defined values
  * @param : descriptor[in] used to give the camera user defined values
  */
  enErrorCode
    init(const PerspectiveCameraDesc &descriptor);

  void
    Translate(float x, float y, float z);

};

