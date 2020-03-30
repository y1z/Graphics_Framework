#pragma once
#include "util/Header.h"
#include <numeric>


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

  /**
  * @returns :the object/value that the open-gl api uses.
  * @bug :no known bugs.
  */
  uint32
  getInterface();

  /**
  * @returns : a pointer to the object/value that the open-gl api uses.
  * @bug :no known bugs.
  */
  uint32*
  getInterfacePtr();

  /**
  * @returns : a reference to the object/value that the open-gl api uses.
  * @bug :no known bugs.
  */
  uint32&
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

  /**
  * @brief : used to set pointer to the data the buffer contains
  * @bug : no known bugs.
  */
  void 
  setData(void *data);

protected:

#if DIRECTX
  //! directX 11 buffer implementation
  ID3D11Buffer* m_interface = nullptr;
#elif OPENGL
  uint32 m_interface = std::numeric_limits<uint32>::max();
  #else
  void* m_interface = nullptr;
#endif 
  //! to keep track of the stats of the buffer 
  sBufferDesc m_Desc;
};

