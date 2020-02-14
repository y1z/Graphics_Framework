#pragma once
#include "enBaseBuffer.h"
class enVertexBuffer final :
  public enBaseBuffer
{
public:
  enVertexBuffer() ;
  ~enVertexBuffer() = default;

public:

  void
  init(uint32 singleElementSize,
       uint32 totalElements,
       uint32 index,
       void* ptr_toData,
       uint32 cpuAccess = 0,
       uint32 miscFlags = 0,
       uint32 structured = 0) override;

#if DIRECTX

  D3D11_BUFFER_DESC
  getDirectXDesc()override;

  D3D11_SUBRESOURCE_DATA
  getSubresource()const;
#endif // DIRECTX

};

