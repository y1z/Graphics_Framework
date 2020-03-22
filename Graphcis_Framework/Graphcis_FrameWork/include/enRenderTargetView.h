#pragma once
#include "util/Header.h"
#include <array>


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


#if DIRECTX
  ID3D11RenderTargetView* m_interface = nullptr;
#elif OPENGL
  int32 m_interface = -1;
#else
  void* m_interface = nullptr;
#endif // DIRECTX

  static constexpr uint32 s_renderTargetMax = 8;

  uint32 m_targetsCount = 0U;
  std::array<enTexture2D, s_renderTargetMax> m_targets{};
  std::array<bool, s_renderTargetMax > m_usedTargets{};
};



