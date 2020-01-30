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

  enFirstPersonCamera*
  getFirstPersonCamera();

  enPerspectiveFreeCamera*
  getFreeCamera();

  void
  addCamera(BasePerspectiveCamera *ptrToCamera);

public:
  std::vector<BasePerspectiveCamera* > m_cameras;

 };
