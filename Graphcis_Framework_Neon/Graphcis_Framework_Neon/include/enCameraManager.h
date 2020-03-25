#pragma once
#include "enPerspectiveFreeCamera.h"
#include "enFirstPersonCamera.h"

#include <deque>


/**
* @brief : contains and manages all 'camera' classes
*/
class enCameraManager
{
public:
  enCameraManager();
  ~enCameraManager();


public:


  /**
  * @brief finds the first instance of a 'enFirstPersonCamera' camera and returns a pointer to it.
  */
  enFirstPersonCamera*
  getFirstPersonCamera();

  /**
  * @brief finds the first instance of a 'enFirstPersonCamera' camera and returns a pointer to it.
  */
  enPerspectiveFreeCamera*
  getFreeCamera();

  /**
  * @brief :
  * @bug : no known bugs.
  */
  void 
  updateCameras(uint32c newWidth, uint32c newHeight);

  /**
  * @brief : add a camera to the manager.
  * @bug : no known bugs.
  */
  void
  addCamera(BasePerspectiveCamera *ptrToCamera);

private:
  /**
  * @brief : contains the cameras used by the manager
  */
  std::deque<BasePerspectiveCamera* > m_cameras;

  std::size_t m_currentCamIndex = 0u;
 };