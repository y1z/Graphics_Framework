#include "enShaderResourceView.h"
#include "DirectXTK/include/DDSTextureLoader.h"
#include "DirectXTK/include/WICTextureLoader.h"
#include "enDevice.h"
#include "helperFucs.h"


namespace dx = DirectX;

enShaderResourceView::enShaderResourceView(enShaderResourceView&& other) noexcept
{
  this->m_interface = other.m_interface;
#if DIRECTX
  other.m_interface = nullptr;
#endif // DIRECTX
}

enShaderResourceView::~enShaderResourceView()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface);
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

