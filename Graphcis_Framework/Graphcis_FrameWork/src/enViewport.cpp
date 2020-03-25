#include "enViewport.h"

void
enViewport::init(const sViewportDesc& viewportDescriptor)
{
  this->m_descriptor = viewportDescriptor;
}

#if DIRECTX
D3D11_VIEWPORT
enViewport::getDxViewport() const
{
  D3D11_VIEWPORT result;
  std::memset(&result, 0, sizeof(D3D11_VIEWPORT));
  result.TopLeftX = m_descriptor.TopLeftX;
  result.TopLeftY = m_descriptor.TopLeftY;
  result.Width = m_descriptor.width;
  result.Height = m_descriptor.height;
  result.MaxDepth = m_descriptor.maxDepth;
  result.MinDepth = m_descriptor.minDepth;

  return result;
}


#endif // DIRECTX

uint32 
enViewport::getWidth() const
{
  return m_descriptor.width;
}

uint32 
enViewport::getHeight() const
{
  return m_descriptor.height;
}
