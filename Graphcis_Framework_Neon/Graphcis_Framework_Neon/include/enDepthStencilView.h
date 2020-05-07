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

public:

  /**
  * @brief Releases the stencil portion of the Depth-Stencil.
  * @bug no known bugs.
  */
  bool
  ReleaseStencil();

  /**
  * @brief Releases the depth-buffer and stencil portion of the Depth-Stencil.
  * @bug no known bugs.
  */
  bool
  ReleaseAllInterfaces();

  /**
  * @brief Creates a descriptor for the depth-stencil view.
  * @returns the descriptor that's used for a DepthStencil .
  * @bug no known bugs.
  */
  EN_NODISCARD static constexpr sTextureDescriptor
  GenerateDesc(float const Width, float const Height) 
  {
    sTextureDescriptor result;
    result.texWidth = Width;
    result.texHeight = Height;
    result.CpuAccess = 0;
    result.texFormat = static_cast< int >(enFormats::depthStencil_format);
    result.Usage = enBufferUse::Default;
    result.BindFlags = enBufferBind::DepthStencil;
    result.arraySize = 1;
    return result;
  };
  
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

