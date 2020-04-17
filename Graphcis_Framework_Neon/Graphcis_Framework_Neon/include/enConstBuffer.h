#pragma once
#include "enBaseBuffer.h"
#include <vector>

  /**
  * @brief : defines a buffer that contains the variables used in 
  * the shader.
  */
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
  /**
  * @brief : used for when the 'enConstBuffer' handles more that one element.
  */
  uint32 m_multiElementInterface = std::numeric_limits<uint32 >::max();

  std::vector<sUniformDetails> m_containedVariables;
#endif // DIRECTX
};

