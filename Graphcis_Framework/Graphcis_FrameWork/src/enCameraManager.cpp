#include "enCameraManager.h"
#include <algorithm>

enCameraManager::enCameraManager()
{
  m_cameras.reserve(4);
}

enCameraManager::~enCameraManager()
{}

enFirstPersonCamera*
enCameraManager::getFirstPersonCamera(size_t nthInstance)
{
  enFirstPersonCamera* result = nullptr;

  for( auto& ptrToCamera : m_cameras )
  {
    result = dynamic_cast< enFirstPersonCamera*> (ptrToCamera);

    if( result != nullptr && !(nthInstance == 0) )
    {
      nthInstance -= 1;
    }
    else if( result != nullptr && nthInstance == 0 )
    {
      return result;
    }
  }

  return  nullptr;
}

enPerspectiveFreeCamera*
enCameraManager::getFreeCamera(size_t nthInstance)
{
  enPerspectiveFreeCamera* result = nullptr;

  for( auto& ptrToCamera : m_cameras )
  {
    result = dynamic_cast<enPerspectiveFreeCamera*> (ptrToCamera);

    if( result != nullptr && !(nthInstance == 0) )
    {
      nthInstance -= 1;
    }
    else if( result != nullptr && nthInstance == 0 )
    {
      return result;
    }
  }

  return  nullptr;
}

void
enCameraManager::addCamera(BasePerspectiveCamera* ptrToCamera)
{
  m_cameras.emplace_back(ptrToCamera);
}
