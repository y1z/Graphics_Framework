#pragma once
#include "BasePerspectiveCamera.h"
#include "glm/gtc/matrix_transform.hpp"

#include "en_defs.h"

/**
* @brief : the descriptor for this camera, should only require that
*          you fill out the following 'upDir' , 'lookAtPosition' ,'frontDir','height' and 'width'
*
*/
struct sPerspectiveCameraDesc
{
  /** describes where the camera is in world space*/
  enVector3 position = enVector3(0.0f, 3.0f, -6.0f); //(0.0f, 0.0f, 0.0f);

  /** describes where the camera is looking At (aka focal point)*/
  enVector3 lookAtPosition;

  /** describes the 'Up' direction relative to the camera*/
  enVector3 upDir;

  /** describes how tall the projection is*/
  float height = -1337.0f;

  /** describes how wide the projection is*/
  float width = -1337.0f;

  float horizantalAngle = 0.00f;

  //vertical
  float verticalAngle = 0.00f;

  float zAngle = 0.00f;

  /** describes the field of view*/
  float fov = 1.04719f;
};

/**
* @brief : this is a perspective camera
*/
class enPerspectiveFreeCamera final :
  public BasePerspectiveCamera
{
public:
  enPerspectiveFreeCamera();
  ~enPerspectiveFreeCamera() = default;
public:// functions
  /**
  * @brief : used to initialize the camera with user defined values
  * @param : descriptor[in] used to give the camera user defined values
  */
  enErrorCode
  init(const sPerspectiveCameraDesc &descriptor);

  /**
  * @brief : move the camera relative to it's own directions(aka 'up' , 'front' ,'right')
  * @param : x [in]  how much in 'x' to translate
  * @param : y [in]  how much in 'y' to translate
  * @param : z [in]  how much in 'z' to translate
  */
  void
  TranslateRelative(float x, float y, float z);

  void
  rotateInYaw(float angleInDegs);

  void
  rotateInPitch(float angleInDegs);

  void
  rotateInRoll(float angleInDegs);

  void
  rotateVector(const enVector2 &rotationDir,
               float deltaTime = 0.01666f);



public:// virtual functions

  virtual  bool
  initDefault()  override;

  void
  updateDimensions(float newWidth,
                   float newHeight) override;
private:// 

  /**
  * @brief :this function updates both the perspective and view matrix
  * @bug : no know bugs
  */
  void
  updateMatrixes();
private: //variable

};

