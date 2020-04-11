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

#else
  other.m_interface = nullptr;
#endif // DIRECTX
}

enShaderResourceView::~enShaderResourceView()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface);
#elif OPENGL

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
  return this->m_index;
}

