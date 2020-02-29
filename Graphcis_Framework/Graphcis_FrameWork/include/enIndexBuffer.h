#pragma once
#include "enBaseBuffer.h"

class enIndexBuffer final: public enBaseBuffer
{
public:
	enIndexBuffer() = default;

	~enIndexBuffer() = default;

  virtual void
  init(uint32 singleElementSize,
       uint32 totalElements,
       uint32 index,
       void* ptr_toData,
       uint32 cpuAccess = 0,
       uint32 miscFlags = 0,
       uint32 structured = 0)override;

  void
  init(const sBufferDesc& descriptor);

#if DIRECTX

  D3D11_BUFFER_DESC
    getDirectXDesc()override;

  D3D11_SUBRESOURCE_DATA
    getSubresource()const;
#endif // DIRECTX

  void*
  getData() const;

private:

};

