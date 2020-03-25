#include "enDepthStencilView.h"

enDepthStencilView::enDepthStencilView(enDepthStencilView&& other)noexcept
  :m_interface(other.m_interface)
{
#if DIRECTX
  other.m_interface = nullptr;
#endif // DIRECTX
}

enDepthStencilView::~enDepthStencilView()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface);
#elif OPENGL
#endif // DIRECTX
}

bool
enDepthStencilView::ReleaseStencil()
{
  bool isSuccessful = m_texture.Release();
  return isSuccessful;
}

bool
enDepthStencilView::ReleaseAllInterfaces()
{
  bool ReleasedStencil = ReleaseStencil();
  bool ReleasedDepth = false;
#if DIRECTX

  if( m_interface != nullptr )
  {
    RELEASE_DX_PTR(m_interface);
    ReleasedDepth = true;
  }
  else { EN_LOG_DB("depth Stencil is not initialized/ created "); }

#endif // DIRECTX

  return (ReleasedStencil && ReleasedDepth);
}
