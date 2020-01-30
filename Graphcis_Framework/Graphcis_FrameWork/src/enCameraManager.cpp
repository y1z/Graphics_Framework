#include "enCameraManager.h"

enCameraManager::enCameraManager()
{
  m_cameras.reserve(4);
}

enCameraManager::~enCameraManager()
{

}

enFirstPersonCamera*
enCameraManager::getFirstPersonCamera()
{
  enFirstPersonCamera* result = nullptr;

  for( auto& ptrToCamera : m_cameras )
  {
    result = dynamic_cast<enFirstPersonCamera*> (ptrToCamera);
    if( result != nullptr )
    {
      break;
    }
  }


  return  result;
}

enPerspectiveFreeCamera*
enCameraManager::getFreeCamera()
{
  enPerspectiveFreeCamera* result = nullptr;

  for( auto& ptrToCamera : m_cameras )
  {
    result = dynamic_cast<enPerspectiveFreeCamera*> (ptrToCamera);
    if( result != nullptr )
    {
      break;
    }
  }


  return  result;
}

void
enCameraManager::addCamera(BasePerspectiveCamera* ptrToCamera)
{
  m_cameras.emplace_back(ptrToCamera);
}
