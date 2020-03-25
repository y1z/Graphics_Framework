#include "enTexture2D.h"

enTexture2D::enTexture2D()
{
  sTextureDescriptor descriptoDepth;
  descriptoDepth.texWidth = 600;
  descriptoDepth.texHeight = 600;
  descriptoDepth.CpuAccess = 0;
  descriptoDepth.texFormat = static_cast<int>(enFormats::R8G8B8A8_uniform_norm);
  descriptoDepth.Usage = enBufferUse::Default;
  descriptoDepth.BindFlags = enBufferBind::RenderTarget;
  descriptoDepth.arraySize = 1;
  m_desc = descriptoDepth;
}


enTexture2D::enTexture2D(enTexture2D&& other)noexcept
{
  m_desc = other.m_desc;
#if DIRECTX
  m_interface = (other.m_interface);
  other.m_interface = nullptr;
#elif OPENGL
  m_interface = (other.m_interface);
#endif // DIRECTX
}


enTexture2D::~enTexture2D()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface);
#elif OPENGL
  m_interface = 0;
#endif // DIRECTX
}

bool
enTexture2D::Release()
{
#if DIRECTX
  if( m_interface != nullptr )
  {

    RELEASE_DX_PTR(m_interface);
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
