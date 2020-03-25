#pragma once
#include "util/Header.h"


class enBaseBuffer
{
public:
  enBaseBuffer() = default;
  enBaseBuffer(const enBaseBuffer& ) = delete;
  enBaseBuffer(enBaseBuffer&& other );

  virtual ~enBaseBuffer();

public://operator
  enBaseBuffer & operator =(const enBaseBuffer & other) = delete;
  enBaseBuffer & operator =(enBaseBuffer && other);



public:
#if DIRECTX
  //! for functions that require a single pointer 
  ID3D11Buffer*
  getInterface();

  //! for functions that require a 2 pointers 
  ID3D11Buffer **
  getInterfaceRef();
#elif OPENGL
  int32
  getInterface();

  int32&
  getInterfaceRef();
#endif // DIRECTX

public: // functions 

  /**
  * @brief : set's the value for the buffer.
  */
  virtual void
  init(uint32 singleElementSize,
       uint32 totalElements,
       uint32 index,
       void* ptr_toData,
       uint32 cpuAccess = 0,
       uint32 miscFlags = 0,
       uint32 structured = 0) = 0;

  /**
  * @retuns :the number of elements the buffer contain.
  */
  confInt
  getElementCount() const;

#if DIRECTX
  //! this is for interfacing with the directX api.
  virtual D3D11_BUFFER_DESC
  getDirectXDesc() = 0;

#elif OPEN_GL
#endif // DIRECTX

  //!return the buffer descriptor 
  sBufferDesc
  getDescriptor() const;

  //! return the stride of the buffer 
  confInt
  getStride() const;

  uint32
  getBufferSize() const;

  uint32
  getIndex() const;
  

protected:

#if DIRECTX
  //! directX 11 buffer implementation
  ID3D11Buffer* m_interface = nullptr;
#elif OPENGL
  int32 m_interface = 0;
  #else
  void* m_interface = nullptr;
#endif 
  //! to keep track of the stats of the buffer 
  sBufferDesc m_Desc;
};

