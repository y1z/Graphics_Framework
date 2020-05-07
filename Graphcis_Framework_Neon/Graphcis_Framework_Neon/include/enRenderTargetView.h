#pragma once
#include "util/Header.h"
#include "enTexture2D.h"
#include <array>

  /**
  * @brief : controls render-target. 
  */
class enRenderTargetView
{
public:

  enRenderTargetView() = default;
  enRenderTargetView(const enRenderTargetView & other) = delete;
  enRenderTargetView(enRenderTargetView && other) noexcept;
  ~enRenderTargetView();
public:
  enRenderTargetView& operator =(const enRenderTargetView& other) = delete;

  enRenderTargetView& operator =(enRenderTargetView&& other)noexcept;
public:

  /**
  * @brief : releases a specific render-target.
  * @bug :no known bugs
  */
  bool
  ReleaseRenderTarget(size_t index);

  /**
  * @brief : release every single renderTarget and the renderTargetView it self.
  * @bug : no known bugs
  */
  bool
  ReleaseAllInterfaces();

  static constexpr sTextureDescriptor
  GenerateDesc(float const Width, float const Height)
  {
    sTextureDescriptor result;
    result.texWidth = Width;
    result.texHeight = Height;
    result.CpuAccess = 0;
    result.BindFlags = enBufferBind::RenderTarget;
    result.texFormat = static_cast< int >(enFormats::renderTarget_format);
    result.Usage = enBufferUse::Default;
    result.arraySize = 1;
    return result;
  }


#if DIRECTX
  ID3D11RenderTargetView* m_interface = nullptr;
#elif OPENGL
  uint32 m_interface = std::numeric_limits<uint32>::max();
#else
  void* m_interface = nullptr;
#endif // DIRECTX

  /**
  * @brief : optional descriptor ( unnecessary when used by swap chain )
  */
  sRenderTargetDesc2D m_desc;

  static constexpr const uint32 s_renderTargetMax = 8;

  uint32 m_targetsCount = 0U;
  std::array<enTexture2D, s_renderTargetMax> m_targets{};
  std::array<bool, s_renderTargetMax > m_usedTargets{};
};



