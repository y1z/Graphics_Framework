#include "enDepthStencilView.h"

enDepthStencilView::enDepthStencilView()
{
#if DIRECTX
  m_interface = nullptr;
#elif OPENGL
  m_interface = std::numeric_limits<uint32>::max();
#endif // DIRECTX
}

enDepthStencilView::enDepthStencilView(enDepthStencilView&& other)noexcept
  :m_interface(other.m_interface),
  m_desc(other.m_desc),
  m_texture(std::move(other.m_texture))
{

#if DIRECTX
  other.m_interface = nullptr;
  #elif OPENGL

#endif // DIRECTX
}

enDepthStencilView::~enDepthStencilView()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface);
#elif OPENGL
  if( m_interface == std::numeric_limits<uint32>::max() )
  {
    glDeleteRenderbuffers(1, &m_interface);
  }
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
  bool const ReleasedStencil = ReleaseStencil();
  bool ReleasedDepth = false;
#if DIRECTX

  if( m_interface != nullptr )
  {
    RELEASE_DX_PTR(m_interface);
    ReleasedDepth = true;
  }
  else { EN_LOG_DB("depth Stencil is not initialized created\n"); }
#elif OPENGL

  if( m_interface == std::numeric_limits<uint32>::max() )
  {
    glDeleteRenderbuffers(1, &m_interface);
    ReleasedDepth = true;
  }
  else { EN_LOG_DB("depth Stencil is not initialized created\n"); }

#endif // DIRECTX

  return (ReleasedStencil && ReleasedDepth);
}
