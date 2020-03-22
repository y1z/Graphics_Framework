#include "enCameraManager.h"
#include <algorithm>

enCameraManager::enCameraManager()
{}

enCameraManager::~enCameraManager()
{}

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
