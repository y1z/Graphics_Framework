#pragma once
#include "util/Header.h"
#include "util/soModule.h"

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
  * @bug :
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

  
  bool
  ReciveBuckBuffer(enRenderTargetView& renderTargetView);

  void 
  Present(int options = 0);


#endif // DIRECTX


public:
#if DIRECTX
  IDXGISwapChain* m_interface = nullptr;
#elif OPENGL
#endif // DIRECTX
  sSwapDesc m_descriptor;
};

