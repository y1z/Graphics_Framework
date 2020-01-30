#pragma once
#include "BasePerspectiveCamera.h"
#include "en_defs.h"
/**
* @brief : will be used to describe which directions  the camera can move
*/
struct sFirstPersonCameraDesc
{
/**
* @brief : determines where the camera well move when going 'right' .
*/
  enVector3 rightMovemientDir = enVector3(1.0f,0.0f,0.0f);

/**
* @brief :  determines where the camera well move when going 'right' .
*/
  enVector3 frontMovemientDir = enVector3(0.0f,0.0f,1.0f);

  /**
  * @brief : determines where the camera well move when going 'up' by 
  * default it's basically disabled
  */
  enVector3 upMovemientDir = enVector3(0.0f,.0f,.0f);

  //! approximately 80 degrees in radians
  float MaxPitchAngle =  1.3962f;

  //! approximately -20 degrees in radians
  float MinPitchAngle = -0.3490f; 

  //! approximately 25 degrees in radians
  float MaxRollAngle = 0.4363f;

  //! approximately -25 degrees in radians
  float MinRollAngle = -0.4363f;

  float width = 600.0f;

  float height = 600.0f;
};

  /**
  * @brief : represents character from a fps game (has limitation when it comes to rotating)
  * @bug : no known bugs
  */
class enFirstPersonCamera final :
  public BasePerspectiveCamera
{
public:
  enFirstPersonCamera();
  ~enFirstPersonCamera() = default;
public:
  /**
  * @brief : initialized the camera with the values given by the descriptor
  * @param[in] descriptor ,use for the camera to have default values
  */
 enErrorCode
 init(const sFirstPersonCameraDesc &descriptor);

  /**
  * @brief :this function updates both the perspective and view matrix
  * @bug : no know bugs
  */
  void
  updateMatrixes();
  /**
  * @brief : makes the camera move
  */
  void 
  TranslateRelative(float x, float y,float z);

  void
  rotateInYaw(float angleInDegs);

  void
  rotateInPitch(float angleInDegs);

  void
  rotateInRoll(float angleInDegs);

  void
  rotateVector(const enVector2& rotationDir,
               float deltaTime = 0.01666f);

public:
  /**
  * @brief : 
  * @bug :
  */
  sFirstPersonCameraDesc m_desc;

};

