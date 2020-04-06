#include "enCameraManager.h"
#include <algorithm>

enCameraManager::enCameraManager()
{}

enCameraManager::~enCameraManager()
{}

bool
enCameraManager::translateRelative(const enVector3& translationVector,
                                   const bool useFreeCam,
                                   const size_t whichInstanceToUse)
{



  return false;
}

enFirstPersonCamera*
enCameraManager::getFirstPersonCamera()
{
  enFirstPersonCamera* result = nullptr;
  m_currentCamIndex = 0u;

  for( BasePerspectiveCamera* ptrToCamera : m_cameras )
  {
    result = dynamic_cast<enFirstPersonCamera*> (ptrToCamera);

    if( result != nullptr )
    {
      break;
    }

    ++m_currentCamIndex;
  }

  return result;
}

enPerspectiveFreeCamera*
enCameraManager::getFreeCamera()
{
  enPerspectiveFreeCamera* result = nullptr;
  m_currentCamIndex = 0u;

  for( auto& ptrToCamera : m_cameras )
  {
    result = dynamic_cast<enPerspectiveFreeCamera*> (ptrToCamera);

    if(result != nullptr ) 
      break;
    ++m_currentCamIndex;
  }

  return result;
}

enFirstPersonCamera*
enCameraManager::getNthFirstPersonCamera(size_t instance)
{
  enFirstPersonCamera* result = nullptr;
  enFirstPersonCamera* currentCam = nullptr;
  m_currentCamIndex = 0u;
  for( auto& ptrToCameras : m_cameras )
  {
    currentCam = dynamic_cast< enFirstPersonCamera* >(ptrToCameras);

    if( currentCam != nullptr )
    {
      if( instance == 0 )
      {
        result = currentCam;
        break;
      }

      --instance;
    }

  }

  return result;
}

enPerspectiveFreeCamera*
enCameraManager::getNthFreeCamera(size_t instance)
{
  enPerspectiveFreeCamera* result = nullptr;
  enPerspectiveFreeCamera* currentCam = nullptr;
  m_currentCamIndex = 0u;
  for( auto& ptrToCameras : m_cameras )
  {
    currentCam = dynamic_cast< enPerspectiveFreeCamera* >(ptrToCameras);

    if( currentCam != nullptr )
    {
      if( instance == 0 )
      {
        result = currentCam;
        break;
      }

      --instance;
    }

  }

  return result;
}

void 
enCameraManager::updateCameras(uint32c newWidth, uint32c newHeight)
{

  for( BasePerspectiveCamera* ptrToCamera : m_cameras )
  {
    ptrToCamera->updateDimensions(static_cast<float>(newWidth), static_cast<float>(newHeight) );
  }

}

void
enCameraManager::addCamera(BasePerspectiveCamera* ptrToCamera)
{
  m_cameras.emplace_back(ptrToCamera);
}

BasePerspectiveCamera* 
enCameraManager::getLastSelectedCam()
{
  return m_cameras[m_currentCamIndex];
}
