#include "enSwapChain.h"
#include "enWindow.h"
#include "helperFucs.h"
#include "enViewport.h"
#include "enRenderTargetView.h"
#include "enDepthStencilView.h"
#include "enTexture2D.h"


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
  this->m_interface = other.m_interface;
  this->m_descriptor = std::move(other.m_descriptor);
#if DIRECTX
  other.m_interface = nullptr;
#elif OPENGL 
  other.m_interface = std::numeric_limits<uint32>::max();
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
#if DIRECTX

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


#endif // DIRECTX

bool 
enSwapChain::ResizeSwapChain(enWindow& currentWindow,
                             enRenderTargetView& renderTargetView,
                             enDepthStencilView& DepthStencilView,
                             enVector2 const &newSize,
                             enViewport& viewport)
{
  enVector2 const sizeOfWindow = helper::getWindowSize(currentWindow);
  bool isSuccessful = false;
  enDevice& device = enDevice::getInstance();
  enDeviceContext& deviceContext = enDeviceContext::getInstance();

  isSuccessful = renderTargetView.ReleaseAllInterfaces();

  assert(isSuccessful && " Error when Releasing all render-targets and renderTargetView");

  isSuccessful = DepthStencilView.ReleaseAllInterfaces();

  assert(isSuccessful && " Error when Releasing all DepthStencilView interfaces ");
  
  this->m_descriptor.buffWidth = newSize.x;
  this->m_descriptor.buffHeight = newSize.y;

  sTextureDescriptor const DepthDesc = enDepthStencilView::GenerateDesc(newSize.x,
                                                                        newSize.y);

  DepthStencilView.m_texture.m_desc = DepthDesc;

#if DIRECTX
  HRESULT hr = m_interface->ResizeBuffers(0, newSize.x, newSize.y, DXGI_FORMAT_UNKNOWN, 0);
  assert(!FAILED(hr) && " Error when Resizing the swap chain");
#elif OPENGL
  GlRemoveAllErrors();
  // TODO
  //glfwSetWindowSize(currentWindow.getHandle(),
  //                  m_descriptor.buffWidth,
  //                  m_descriptor.buffHeight);

  if( GlCheckForError() )
  {
    return false;
  }
                    
#endif // DIRECTX
  isSuccessful = this->ReciveBuckBuffer(renderTargetView);

  assert(isSuccessful && " Error with getting a backBuffer for SwapChain");

  isSuccessful = device.CreateRenderTargetView(renderTargetView, 0);

  assert(isSuccessful && " Error with creating a renderTarget");

  isSuccessful = device.CreateTexture2D(DepthStencilView.m_texture.m_desc, DepthStencilView.m_texture);

  assert(isSuccessful && " Error with creating a Stencil");

  isSuccessful = device.CreateDepthStencilView(DepthStencilView);

  assert(isSuccessful && " Error with creating a Depth-Stencil-View ");

  deviceContext.OMSetRenderTargets(&renderTargetView, &DepthStencilView);

  viewport.m_descriptor.width = static_cast< uint32 >(newSize.x);

  viewport.m_descriptor.height = static_cast< uint32 >(newSize.y);
  
  deviceContext.RSSetViewports(&viewport);

  return  isSuccessful;
}

bool
enSwapChain::ReciveBuckBuffer(enRenderTargetView& renderTargetView)
{
  
#if DIRECTX

  HRESULT hr = m_interface->GetBuffer(0,
                                      __uuidof(ID3D11Texture2D),
                                      (LPVOID*)&renderTargetView.m_targets[0].m_interface);

  if( FAILED(hr) )
    return false;

  else
  {
    renderTargetView.m_usedTargets[0] = true;
    renderTargetView.m_targetsCount++;
  }


  return true;
#elif OPENGL
  m_GlBackBuffer = &renderTargetView.m_targets[0];

  return true;
#endif // DIRECTX
  return false;
}

void 
enSwapChain::Present(int options)
{
#if DIRECTX
  m_interface->Present(0,options);
#elif OPENGL
  glfwSwapBuffers(m_descriptor.outputWindow);
#endif // DIRECTX
}

void 
enSwapChain::setWindow(enWindow& window)
{
  m_descriptor.outputWindow = window.getHandle();
}
