#pragma once
#include "util/Header.h"

  /**
  * @brief : defines a 2D texture and all operations associated with it.
  * @bug : no known bugs.
  */
class enTexture2D
{
public: 
  enTexture2D();

  enTexture2D(const enTexture2D& other) = delete;
  enTexture2D(enTexture2D&& other) noexcept;

  ~enTexture2D();

public: 
  /**
  * @brief releases the memory used by the texture.
  * @returns 'false' when the memory could not be released or there
  * was non being used.
  * @bug no known bugs.
  */
  bool
  Release();

#if DIRECTX

  /**
  * @returns : the object or value needed to interact with 
  * directX.
  */
  ID3D11Texture2D*
  getInterface();

  /**
  * @returns : a reference to the object or value needed to interact with 
  * directX.
  */
  ID3D11Texture2D**
  getInterfaceRef();
#elif OPENGL

  /**
  * @returns : the object or value needed to interact with 
  * open-gl.
  */
  uint32
  getInterface();

  /**
  * @returns :  a reference to the object or value needed to interact with 
  * open-gl.
  */
  uint32&
  getInterfaceRef();

  /**
  * @returns :  a pointer to the object or value needed to interact with 
  * open-gl.
  */
  uint32*
  getInterfacePtr();

#endif // DIRECTX

private:

  /**
  * @brief : frees the memory required by the api for a 2D texture.
  * @bug : no known bugs.
  */
  void 
  deleteTexture();
  
public:
  /**
  * @brief : keeps track of the quality's of the texture
  */
  sTextureDescriptor m_desc;
#if DIRECTX

  /**
  * @brief : used to interface with directX api.
  */
  ID3D11Texture2D* m_interface = nullptr;
#elif OPENGL

  /**
  * @brief : used to interface with open-gl api.
  */
  uint32 m_interface = std::numeric_limits<uint32>::max();
#endif // DIRECTX

};

