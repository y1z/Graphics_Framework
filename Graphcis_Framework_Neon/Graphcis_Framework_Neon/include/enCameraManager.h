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
  * @brief : translates a camera relative to it self.
  * @returns true if the operation succeeded.
  * @param[in] translationVector : controls how the camera is going to translate.
  * @param[in] useFreeCam determines which type of camera to use.
  * @param[in] whichInstanceToUse : controls which instance of the cameras to use.
  * @bug : no known bugs.
  */
  bool
  translateRelative(const enVector3 & translationVector,
                    const bool useFreeCam = true,
                    const size_t whichInstanceToUse = 0);


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
  * @brief find the nth instance of a camera if it exist.
  */
  enFirstPersonCamera*
  getNthFirstPersonCamera(size_t instance);

  /**
  * @brief find the nth instance of a camera if it exist.
  */
  enPerspectiveFreeCamera*
  getNthFreeCamera(size_t instance);


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

  /**
  * @returns : a pointer to the last selected camera.
  * @bug : no known bugs.
  */
  BasePerspectiveCamera*
  getLastSelectedCam();

private:

  BasePerspectiveCamera*
  getCameraInstance( size_t const selectedInstance,
                    bool const useFreeCam);

  /**
  * @brief : contains the cameras used by the manager
  */
  std::deque<BasePerspectiveCamera* > m_cameras;

  std::size_t m_currentCamIndex = 0u;
 };
