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

  bool
  init(const sSwapDesc& descriptor);

  sSwapDesc 
  getSwapChainDesc()const;

#if DIRECTX

  DXGI_SWAP_CHAIN_DESC
  getdxSawpChainDesc()const;

#endif // DIRECTX


public:
#if DIRECTX
  IDXGISwapChain* m_interface = nullptr;
#elif OPENGL
#endif // DIRECTX
  sSwapDesc m_descriptor;
};

