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
#if OPENGL
#endif // OPEN_GL
}


enTexture2D::enTexture2D(enTexture2D&& other)noexcept
{
  m_desc = other.m_desc;
#if DIRECTX
  m_interface = (other.m_interface);
  other.m_interface = nullptr;
#elif OPENGL
  m_interface = (other.m_interface);
  other.m_interface = std::numeric_limits<uint32>::max();
#endif // DIRECTX
}


enTexture2D::~enTexture2D()
{
  this->deleteTexture();
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
  this->deleteTexture();
#endif // DIRECTX
  return false;
}

#if DIRECTX
ID3D11Texture2D*
enTexture2D::getInterface()
{
  return m_interface;
}

ID3D11Texture2D**
enTexture2D::getInterfaceRef()
{
  return &m_interface;
}
#elif OPENGL

uint32
enTexture2D::getInterface()
{
  return m_interface;
}

uint32&
enTexture2D::getInterfaceRef()
{
  return m_interface;
}

uint32*
enTexture2D::getInterfacePtr()
{
  return &m_interface;
}

#endif // DIRECTX

void
enTexture2D::deleteTexture()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface);
#elif OPENGL
  if( m_interface != std::numeric_limits<uint32>::max() )
  {
    // this is done because a texture can be used for many things 
    if( glIsBuffer(m_interface) == GL_TRUE )
    {
      glDeleteBuffers(1, &m_interface);
    }

    if( glIsTexture(m_interface) == GL_TRUE )
    {
      glDeleteTextures(1, &m_interface);
    }

    if( glIsRenderbuffer(m_interface) == GL_TRUE )
    {
      glDeleteRenderbuffers(1, &m_interface);
    }

    if( glIsFramebuffer(m_interface) == GL_TRUE )
    {
      glDeleteFramebuffers(1, &m_interface);
    }

    m_interface = std::numeric_limits<uint32>::max();
  }
#endif // DIRECTX


}
