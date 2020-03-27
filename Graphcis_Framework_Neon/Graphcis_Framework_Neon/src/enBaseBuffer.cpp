#include"enBaseBuffer.h"

enBaseBuffer::enBaseBuffer(enBaseBuffer&& other)
  :m_interface(other.m_interface),
  m_Desc(std::move(other.m_Desc))
{
#if DIRECTX
  other.m_interface = nullptr;
#elif OPENGL
  other.m_interface = 0;
#else
  other.m_interface = nullptr;
#endif // DIRECTX

}

enBaseBuffer::~enBaseBuffer()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface)
  #elif OPENGL
  if(m_interface != 0)
  {
    glDeleteBuffers(1, &m_interface);
    m_interface = 0;
  }
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
    if( m_interface != std::numeric_limits<uint32>::max() )
    {
      glDeleteBuffers(1, &m_interface);
    }

    m_interface = other.m_interface;
    other.m_interface = std::numeric_limits<uint32>::max();
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

uint32
enBaseBuffer::getInterface()
{
  return m_interface;
}

uint32&
enBaseBuffer::getInterfaceRef()
{
  return m_interface;
}

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
  return m_Desc.stride;
}

uint32
enBaseBuffer::getBufferSize() const
{
  return m_Desc.sizeOfBuffer;
}

uint32
enBaseBuffer::getIndex() const
{
  return m_Desc.index;
}
