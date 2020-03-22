#pragma once
#include "util/Header.h"
#include "util/soModule.h"

class enWindow;
class enViewport;

  /**
  * @brief : takes care of switching the front-buffers and back-buffer each frame.
  * @bug : no known bugs.
  */
class enSwapChain 
{
public:
  enSwapChain() = default;
  enSwapChain(const enSwapChain& other) = delete;
  enSwapChain(enSwapChain&& other) noexcept;
  ~enSwapChain();

  enSwapChain& operator=(const enSwapChain& other) = delete;
  enSwapChain& operator=(enSwapChain&& other)noexcept;
public:
  /**
  * @brief : sets up the descriptor for the buffer.
  * @bug : no known bugs.
  */
  bool
  init(const sSwapDesc& descriptor);

  /**
  * @returns my descriptor for the swap-chain.
  * @bug : no known bugs.
  */
  sSwapDesc 
  getSwapChainDesc()const;

#if DIRECTX
  /**
  * @return the descriptor for directX.
  * @bug : no known bugs.
  */
  DXGI_SWAP_CHAIN_DESC
  getdxSawpChainDesc()const;

#endif // DIRECTX

  /**
  * @brief : changes the size of the swap chain.
  * @bug : requires that enDevice and enDeviceContext already
  * be initialized
  */
  bool
  ResizeSwapChain(enWindow &currentWindow,
                  enRenderTargetView& renderTargetView,
                  enDepthStencilView& DepthStencilView,
                  enVector2 const& newSize,
                  enViewport& viewport);


  /**
  * @brief : give the swap-chain a back buffer.
  * @bug :no known bugs
  */
  bool
  ReciveBuckBuffer(enRenderTargetView& renderTargetView);

  /**
  * @brief : presents what the back-buffer currently has.
  * @bug : no known bugs
  */
  void 
  Present(int options = 0);

public:
#if DIRECTX
  IDXGISwapChain* m_interface = nullptr;
#elif OPENGL
#endif // DIRECTX
  sSwapDesc m_descriptor;
};

