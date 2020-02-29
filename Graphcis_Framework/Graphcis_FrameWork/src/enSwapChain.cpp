#include "enSwapChain.h"

enSwapChain::enSwapChain(enSwapChain&& other) noexcept
{
#if DIRECTX
  m_interface = other.m_interface;
  other.m_interface = nullptr;
  this->m_descriptor = std::move(other.m_descriptor);
#endif // DIRECTX
}

enSwapChain::~enSwapChain()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface);
#endif // DIRECTX
}

enSwapChain&
enSwapChain::operator=(enSwapChain&& other) noexcept
{
#if DIRECTX
  this->m_interface = other.m_interface;
  other.m_interface = nullptr;
  this->m_descriptor = std::move(other.m_descriptor);
#endif // DIRECTX

  return *this;
}

bool
enSwapChain::init(const sSwapDesc& descriptor)
{
  m_descriptor = descriptor;

  return true;;
}

sSwapDesc
enSwapChain::getSwapChainDesc() const
{
  return m_descriptor;
}

DXGI_SWAP_CHAIN_DESC
enSwapChain::getdxSawpChainDesc() const
{
  DXGI_SWAP_CHAIN_DESC result;
  SecureZeroMemory(&result, sizeof(result));
  result.BufferCount = m_descriptor.buffCount;
  result.BufferDesc.Width = m_descriptor.buffWidth;
  result.BufferDesc.Height = m_descriptor.buffHeight;
  result.BufferDesc.Format = static_cast<DXGI_FORMAT>(m_descriptor.buffFormat);
  result.BufferDesc.RefreshRate.Numerator = m_descriptor.buffRefershNumaretor;
  result.BufferDesc.RefreshRate.Denominator = m_descriptor.buffRefershDenominator;
  result.BufferUsage = m_descriptor.buffUsage;
  result.OutputWindow = m_descriptor.outputWindow;
  result.SampleDesc.Count = m_descriptor.sampCount;
  result.SampleDesc.Quality = m_descriptor.sampQuality;
  result.Windowed = m_descriptor.isWindowd;
  return  result;
}
