#include "enDevice.h"
#include "enVertexBuffer.h"
#include "enIndexBuffer.h"
#include "enConstBuffer.h"
#include "enSampler.h"

#include <vector>

bool enDevice::is_Initalized = false;


int
enDevice::OnStartUp(void* _Descriptor)
{
#if DIRECTX
  m_interface = nullptr;
#elif
#endif // DIRECTX
  is_Initalized = true;

  return 0;
}

void
enDevice::OnShutDown()
{
#if DIRECTX

  RELEASE_DX_PTR(m_interface)
#endif // DIRECTX
}

bool
enDevice::init()
{
#if DIRECTX

#elif OPENGL

#endif // DIRECTX
  return true;
}

bool
enDevice::CreateRenderTargetView(enTexture2D& texture,
                                 enRenderTargetView& renderTraget)
{
#if DIRECTX
  HRESULT hr = S_FALSE;


  hr = m_interface->CreateRenderTargetView(texture.m_interface,
                                           NULL,
                                           &renderTraget.m_interface);
  if( SUCCEEDED(hr) )
  {
    return true;
  }
#elif OPENGL

#endif // DIRECTX
  return false;
}

bool
enDevice::CreateTexture2D(sTextureDescriptor& Description,
                          enTexture2D& Texture)
{
#if DIRECTX

  HRESULT hr = S_FALSE;
  D3D11_TEXTURE2D_DESC descDepth;
  std::memset(&descDepth, 0, sizeof(descDepth));
  descDepth.Width = Description.texWidth;
  descDepth.Height = Description.texHeight;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = static_cast<DXGI_FORMAT> (Description.texFormat);//DXGI_FORMAT_D24_UNORM_S8_UINT;//DXGI_FORMAT_D24_UNORM_S8_UINT
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = static_cast<D3D11_USAGE> (Description.Usage);
  descDepth.BindFlags = Description.BindFlags;
  descDepth.CPUAccessFlags = Description.CpuAccess;
  descDepth.MiscFlags = 0;

  hr = m_interface->CreateTexture2D(&descDepth, NULL, &Texture.m_interface);
  if( SUCCEEDED(hr) )
  {
    return true;
  }
#elif OPENGL
#endif // DIRECTX
return false;
  return false;
}

bool
enDevice::CreateDepthStencilView(enDepthStencilView& DepthView)
{
#if DIRECTX
  HRESULT hr;
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  SecureZeroMemory(&descDSV, sizeof(descDSV));
  descDSV.Format = static_cast<DXGI_FORMAT>(DepthView.m_desc.Format);
  descDSV.ViewDimension = static_cast<D3D11_DSV_DIMENSION>(DepthView.m_desc.Dimension);
  descDSV.Texture2D.MipSlice = DepthView.m_desc.Mip;

  hr = m_interface->CreateDepthStencilView(DepthView.m_texture.m_interface, &descDSV, &DepthView.m_interface);

  if( SUCCEEDED(hr) )
  {
    return true;
  }

#elif OPENGL
#endif // DIRECTX
  return false;
}

bool
enDevice::CreateVertexShader(enVertexShader& vertexShader)
{
#if DIRECTX
  HRESULT hr;
  hr = m_interface->CreateVertexShader(vertexShader.m_desc.m_infoOfShader->GetBufferPointer(),
                                       vertexShader.m_desc.m_infoOfShader->GetBufferSize(),
                                       NULL,
                                       &vertexShader.m_interface);

  if( SUCCEEDED(hr) )
  {
    return true;
  }

#elif
#endif // DIRECTX
  return false;
}

bool
enDevice::CreatePixelShader(enPixelShader& pixelShader)
{
#if DIRECTX
  HRESULT hr = S_FALSE;
  hr = m_interface->CreatePixelShader(pixelShader.m_desc.m_infoOfShader->GetBufferPointer(),
                                      pixelShader.m_desc.m_infoOfShader->GetBufferSize(),
                                      NULL,
                                      &pixelShader.m_interface);

  if( SUCCEEDED(hr) )
  {
    return true;
  }
#elif OPENGL
#endif // DIRECTX
  return false;
}

bool 
enDevice::CreateInputLayout(enInputLayout& inputLayout,
                            enVertexShader& vertexShaderPath)
{
#if DIRECTX
          //directX native input layout struct 
  std::vector<D3D11_INPUT_ELEMENT_DESC> directxInputLayout;
  // my input layout struct 
  std::vector<sInputDescriptor> intermidateLayout = {inputLayout.m_desc};
  /*convert my intermediate input layout format to
  directX input layout format */
  for( const sInputDescriptor& intermidate : intermidateLayout )
  {
    D3D11_INPUT_ELEMENT_DESC directxDesc;
    directxDesc.Format = static_cast<DXGI_FORMAT>(intermidate.Format);
    directxDesc.AlignedByteOffset = static_cast<UINT>(intermidate.Alignment);
    directxDesc.SemanticIndex = intermidate.Index;
    directxDesc.SemanticName = intermidate.Name.c_str();
    directxDesc.InputSlot = intermidate.Slot;
    directxDesc.InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION> (intermidate.SlotClass);
    directxDesc.InstanceDataStepRate = intermidate.InstanceData;
    directxInputLayout.emplace_back(directxDesc);
  }

  HRESULT  hr = m_interface->CreateInputLayout(&directxInputLayout[0],
                                               intermidateLayout.size(),
                                               vertexShaderPath.m_desc.m_infoOfShader->GetBufferPointer(),
                                               vertexShaderPath.m_desc.m_infoOfShader->GetBufferSize(),
                                               &inputLayout.m_interface);
  if( SUCCEEDED(hr) )
  {
    return true;
  }

#elif OPENGL
#endif // DIRECTX
  return false;
}

bool
enDevice::CreateIndexBuffer(enIndexBuffer& indexBuffer)
{
#if DIRECTX
  HRESULT hr;
  D3D11_BUFFER_DESC directxDesc = indexBuffer.getDirectXDesc();
  D3D11_SUBRESOURCE_DATA subresource = indexBuffer.getSubresource();

  hr = m_interface->CreateBuffer(&directxDesc,
                                 &subresource,
                                 indexBuffer.getInterfaceRef());

  if( SUCCEEDED(hr) )
  {
    return true;
  }
  return false;
#elif OPENGL 
#endif// DIRECTX

}

bool
enDevice::CreateVertexBuffer(enVertexBuffer& vertexBuffer)
{
#if DIRECTX
  HRESULT hr;
  D3D11_BUFFER_DESC directxDesc = vertexBuffer.getDirectXDesc();
  D3D11_SUBRESOURCE_DATA subresource = vertexBuffer.getSubresource();

  hr = m_interface->CreateBuffer(&directxDesc,
                                 &subresource,
                                 vertexBuffer.getInterfaceRef());

  if( SUCCEEDED(hr) )
  {
    return true;
  }
  return false;
#elif OPENGL
#endif // DIRECTX


  return false;
}

bool 
enDevice::CreateConstBuffer(enConstBuffer& constBuffer)
{
#if DIRECTX
  HRESULT hr = S_FALSE;
  D3D11_BUFFER_DESC directxDesc = constBuffer.getDirectXDesc();

  hr = m_interface->CreateBuffer(&directxDesc,
                                 nullptr,
                                 constBuffer.getInterfaceRef());

  if( SUCCEEDED(hr) )
  {
    return true;
  }
  return false;
#elif OPENGL
#endif // DIRECTX
  return false;
}

bool
enDevice::CreateSamplerState(enSampler& sampler)
{
#if DIRECTX
  HRESULT hr = 0;
  D3D11_SAMPLER_DESC directxSampler = sampler.getDirectxSamplerDesc();
  hr = m_interface->CreateSamplerState(&directxSampler,
                                       &sampler.m_interface);

  if( SUCCEEDED(hr) )
  {
    return true;
  }
  return false;
#elif OPENGL

#endif // DIRECTX
  return false;
}

#if DIRECTX

ID3D11Device*
enDevice::getInterface()
{
  return m_interface;
}

ID3D11Device**
enDevice::getInterfaceRef()
{
  return &m_interface;
}


#endif // DIRECTX
