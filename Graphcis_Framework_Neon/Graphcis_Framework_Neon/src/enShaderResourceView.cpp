#include "enShaderResourceView.h"
#if DIRECTX
#include "DirectXTK/include/DDSTextureLoader.h"
#include "DirectXTK/include/WICTextureLoader.h"
namespace dx = DirectX;
#elif OPENGL
#include "SOIL2/include/SOIL2.h"
#endif // DIRECTX
#include "enDevice.h"


enShaderResourceView::enShaderResourceView(enShaderResourceView&& other) noexcept
  :m_interface(other.m_interface)
{
#if DIRECTX
  other.m_interface = nullptr;
#elif OPENGL
  other.m_interface = std::numeric_limits<uint32>::max();
#else
  other.m_interface = nullptr;
#endif // DIRECTX
}

enShaderResourceView::~enShaderResourceView()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface);
#elif OPENGL
  if( m_interface != std::numeric_limits<uint32>::max() )
  {
    glDeleteTextures(1, &m_interface);
  }
#endif // DIRECTX
}

bool
enShaderResourceView::init()
{

  return true;
}

int32_t 
enShaderResourceView::getIndex() const
{
  return this->m_descriptor.m_index;
}

