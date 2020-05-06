#include "enRenderTargetView.h"

enRenderTargetView::enRenderTargetView(enRenderTargetView&& other) noexcept
  :m_interface(other.m_interface)
{
#if DIRECTX
  other.m_interface = nullptr;
#elif OPENGL
#else
  other.m_interface = nullptr;
#endif // DIRECTX
}

enRenderTargetView::~enRenderTargetView()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface);
#elif OPENGL
  if( m_interface == std::numeric_limits<uint32>::max() )
  {
    glDeleteFramebuffers(1,&m_interface);
    m_interface = std::numeric_limits<uint32>::max();
  }
#endif // DIRECTX
}

enRenderTargetView&
enRenderTargetView::operator=(enRenderTargetView&& other) noexcept
{
  this->m_interface = other.m_interface;
#if DIRECTX
  other.m_interface = nullptr;
#elif OPENGL
#else
  other.m_interface = nullptr;

#endif // DIRECTX
  return *this;
}

bool
enRenderTargetView::ReleaseRenderTarget(size_t index)
{
#if DIRECTX
  if( index <= m_targets.size() - 1 )
  {
    this->m_targets[index].Release();
    this->m_usedTargets[index] = false;
    return true;
  }
  else
  {
    return false;
  }
#elif OPENGL
#endif // DIRECTX
  return false;
}

bool 
enRenderTargetView::ReleaseAllInterfaces()
{
  for( enTexture2D& target : m_targets )
  {
    target.Release();
  }

#if DIRECTX
  if( m_interface )
  {
    RELEASE_DX_PTR(m_interface);
    return true;
  }
  else { return false; }
#endif // DIRECTX
  return true;
}

