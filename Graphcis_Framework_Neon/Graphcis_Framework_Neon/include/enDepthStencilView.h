#pragma once
#include "util/Header.h"
#include "enTexture2D.h"


class enDepthStencilView
{
public:
  enDepthStencilView() = default;
  enDepthStencilView(const enDepthStencilView& other) = delete;
  enDepthStencilView(enDepthStencilView&& other) noexcept;

  ~enDepthStencilView();

  bool
  ReleaseStencil();
  

  bool
  ReleaseAllInterfaces();
  

  sDepthStencilDescriptor m_desc;
  enTexture2D m_texture;
#if DIRECTX
  ID3D11DepthStencilView* m_interface = nullptr;
#elif OPENGL
  int32 m_interface = 0;
  #else 
  void* m_interface = nullptr;
#endif // DIRECTX

};

