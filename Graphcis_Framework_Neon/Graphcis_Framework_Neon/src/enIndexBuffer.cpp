#include "../include/enIndexBuffer.h"

void
enIndexBuffer::init(uint32 singleElementSize,
                    uint32 totalElements,
                    uint32 index,
                    void* ptr_toData,
                    uint32 cpuAccess,
                    uint32 miscFlags,
                    uint32 structured)
{
  m_Desc.stride = singleElementSize;
  m_Desc.elementCount = totalElements;
  m_Desc.sizeOfBuffer = m_Desc.stride * m_Desc.elementCount;

  m_Desc.index = index;
  m_Desc.ptr_data = ptr_toData;
  m_Desc.cpuAccess = cpuAccess;
  m_Desc.miscFlags = miscFlags;
  m_Desc.structured = structured;
  m_Desc.usage = 0;
  m_Desc.bindFlags = enBufferBind::Index;
#if DIRECTX
  m_Desc.usage = static_cast<int32>(D3D11_USAGE_DEFAULT);

#elif OPENGL

  glGenBuffers(1, &m_interface);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_interface);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#endif // DIRECTX
}

void
enIndexBuffer::init(const sBufferDesc& descriptor)
{
  m_Desc = descriptor;

  m_Desc.sizeOfBuffer = m_Desc.elementCount * m_Desc.stride;

#if DIRECTX
  m_Desc.usage = static_cast<int32>(D3D11_USAGE_DEFAULT);
#endif // DIRECTX
}


#if DIRECTX

D3D11_BUFFER_DESC
enIndexBuffer::getDirectXDesc()
{
  D3D11_BUFFER_DESC Result;
  Result.BindFlags = m_Desc.bindFlags;
  Result.ByteWidth = m_Desc.sizeOfBuffer;
  Result.CPUAccessFlags = m_Desc.cpuAccess;
  Result.Usage = static_cast<D3D11_USAGE>(m_Desc.usage);
  Result.MiscFlags = m_Desc.miscFlags;
  Result.StructureByteStride = m_Desc.structured;
  return  Result;
}

D3D11_SUBRESOURCE_DATA
enIndexBuffer::getSubresource() const
{
  D3D11_SUBRESOURCE_DATA result;
  std::memset(&result, 0, sizeof(D3D11_SUBRESOURCE_DATA));
  result.pSysMem = m_Desc.ptr_data;
  return result;
}


#endif // DIRECTX

void*
enIndexBuffer::getData() const
{
  return m_Desc.ptr_data;
}
