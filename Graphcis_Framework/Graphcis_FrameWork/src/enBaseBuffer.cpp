#include"enBaseBuffer.h"

enBaseBuffer::enBaseBuffer(enBaseBuffer&& other)
  :m_interface(other.m_interface),
  m_Desc(std::move(other.m_Desc))
{
#if DIRECTX
  other.m_interface = nullptr;
#elif OPENGL
#endif // DIRECTX

}

enBaseBuffer::~enBaseBuffer()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface)
  #elif OPENGL
#endif // DIRECTX
}

enBaseBuffer&
enBaseBuffer::operator=(enBaseBuffer&& other)
{
  if( &other != this )
  {
  #if DIRECTX
    RELEASE_DX_PTR(this->m_interface);
    this->m_interface = other.m_interface;
    other.m_interface = nullptr;
  #elif OPENGL
  #endif // DIRECTX
    this->m_Desc = std::move(other.m_Desc);
  }

  return *this;
}

#if DIRECTX
ID3D11Buffer*
enBaseBuffer::getInterface()
{
  return m_interface;
}


ID3D11Buffer**
enBaseBuffer::getInterfaceRef()
{
  return &m_interface;
}


#elif OPENGL
#endif // DIRECTX

confInt
enBaseBuffer::getElementCount() const
{
  return m_Desc.elementCount;
}

sBufferDesc
enBaseBuffer::getDescriptor() const
{
  return m_Desc;
}

confInt
enBaseBuffer::getStride() const
{
  return m_Desc.Stride;
}

uint32
enBaseBuffer::getBufferSize() const
{
  return m_Desc.sizeOfBuffer;
}
