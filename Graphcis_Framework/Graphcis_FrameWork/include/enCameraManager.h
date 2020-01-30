#pragma once
#include "enPerspectiveFreeCamera.h"
#include "enFirstPersonCamera.h"

#include <vector>


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
  * @param[in] nthInstance :it's used to know which of type 'enFirstPersonCamera' to return (in case 
  * there are more than one) by default it search the first(aka zero'eth instance).
  */
  enFirstPersonCamera*
  getFirstPersonCamera(size_t nthInstance = 0);

  /**
  * @brief finds the first instance of a 'enFirstPersonCamera' camera and returns a pointer to it.
  */
  enPerspectiveFreeCamera*
  getFreeCamera(size_t nthInstance = 0);


  void
  addCamera(BasePerspectiveCamera *ptrToCamera);

private:
  std::vector<BasePerspectiveCamera* > m_cameras;

 };
