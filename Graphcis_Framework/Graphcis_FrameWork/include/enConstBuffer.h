#pragma once
#include "enBaseBuffer.h"


class enConstBuffer :
  public enBaseBuffer
{
public:
  enConstBuffer();
  ~enConstBuffer() = default;

public:
  //! for setting the values in the buffer 
  void
  init(uint32 singleElementSize,
       uint32 totalElements,
       uint32 index,
       void* ptr_toData,
       uint32 cpuAccess = 0,
       uint32 miscFlags = 0,
       uint32 structured = 0) override;

  void
  init(const sBufferDesc& descriptor);

#if DIRECTX

  D3D11_BUFFER_DESC
    getDirectXDesc() override;
#elif OPENGL

#endif // DIRECTX
};

