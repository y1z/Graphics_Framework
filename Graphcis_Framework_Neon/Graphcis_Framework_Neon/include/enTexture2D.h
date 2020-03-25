#pragma once
#include "util/Header.h"


class enTexture2D
{
public: 
  enTexture2D();

  enTexture2D(const enTexture2D& other) = delete;
  enTexture2D(enTexture2D&& other) noexcept;

  ~enTexture2D();

  bool
  Release();
  
  sTextureDescriptor m_desc;
#if DIRECTX

  ID3D11Texture2D* m_interface = nullptr;
#elif OPENGL

  int32 m_interface = 0;
#endif // DIRECTX

};

