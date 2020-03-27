#pragma once
#include "util/Header.h"
#include "enTexture2D.h"

  /**
  * @brief : defines a depth-Stencil-View and all related operations.
  */
class enDepthStencilView
{
public:
  enDepthStencilView();
  enDepthStencilView(const enDepthStencilView& other) = delete;
  enDepthStencilView(enDepthStencilView&& other) noexcept;

  ~enDepthStencilView();

  bool
  ReleaseStencil();
  

  bool
  ReleaseAllInterfaces();
  
public:

  /**
  * @brief : describes the property's of the depthStencilView.
  */
  sDepthStencilDescriptor m_desc;

  /**
  * @brief : this texture represent a Depth-stencil.
  */
  enTexture2D m_texture;
#if DIRECTX

  /**
  * @brief : this is the object used to interface with the directX api.
  */
  ID3D11DepthStencilView* m_interface = nullptr;
#elif OPENGL

  /**
  * @brief : this is the object used to interface with the open-gl api.
  */
  uint32 m_interface = std::numeric_limits<uint32>::max();
#else 
  void* m_interface = nullptr;
#endif // DIRECTX

};

