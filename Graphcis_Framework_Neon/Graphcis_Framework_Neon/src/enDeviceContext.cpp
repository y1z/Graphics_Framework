#include "..\include\enDeviceContext.h"
#include "enShaderResourceView.h"
#include "enRenderTargetView.h"
#include "enDepthStencilView.h"
#include "enTexture2D.h"

// limits for setting objects 
static constexpr const uint8_t c_MaxRenderTargets = 8;
static constexpr const uint8_t c_MaxViewPorts = 8;
//TODO : make open-gl have the ability to use multiple vertex buffers  
static constexpr const uint8_t c_MaxVertexBuffers = 24;

void
enDeviceContext::OnShutDown()
{
#if DIRECTX

  RELEASE_DX_PTR(m_interface);

#endif // DIRECTX
}

int
enDeviceContext::OnStartUp(void* _Desc)
{
#if DIRECTX

  m_interface = nullptr;

#endif // DIRECTX
  return 0;
}

#if DIRECTX

ID3D11DeviceContext*
enDeviceContext::getInterface()
{
  return m_interface;
}

ID3D11DeviceContext**
enDeviceContext::getInterfaceRef()
{
  return &m_interface;
}
#endif // DIRECTX


void
enDeviceContext::ClearState()
{
#if DIRECTX
  m_interface->ClearState();
#elif OPENGL

#endif // DIRECTX
}


void
enDeviceContext::OMSetRenderTargets(enRenderTargetView renderTargetsViews[],
                                    enDepthStencilView*ptr_depthStencilView,
                                    uint32_t numRenderTargets)
{
#if DIRECTX
  ID3D11RenderTargetView* RenderTempPtrArr[c_MaxRenderTargets];
  ID3D11DepthStencilView* ptr_DirectXDepth = nullptr;

  if(ptr_depthStencilView ){
    ptr_DirectXDepth = ptr_depthStencilView->m_interface;
  }

  if( numRenderTargets <= c_MaxRenderTargets )
  {
    for( uint8_t i = 0; i < numRenderTargets; ++i )
    {
      RenderTempPtrArr[i] = renderTargetsViews[i].m_interface;
    }

    m_interface->OMSetRenderTargets(numRenderTargets,
                                    RenderTempPtrArr,
                                    ptr_DirectXDepth);
  }
  else
  {
    assert(("Error two many render targets", numRenderTargets <= c_MaxRenderTargets));
  }
#elif OPENGL
#endif // DIRECTX

}

void 
enDeviceContext::setDepthStencilView(enDepthStencilView& depthStencilView,
                                     uint32_t numRenderTragets)
{

#if DIRECTX
  m_interface->OMSetRenderTargets(numRenderTragets,
                                  nullptr,
                                  depthStencilView.m_interface);

#elif OPENGL
#endif // DIRECTX
}


void
enDeviceContext::OMSetRenderTargets(std::vector<enRenderTargetView>& renderTragetsViews,
                                    enDepthStencilView& depthStencilView)
{
#if DIRECTX
  ID3D11RenderTargetView* RenderTempPtrArr[c_MaxRenderTargets];
  size_t const totalElements = renderTragetsViews.size();
  if( totalElements <= c_MaxRenderTargets )
  {
    for( uint8_t i = 0; i < totalElements; ++i )
    {
      RenderTempPtrArr[i] = renderTragetsViews[i].m_interface;
    }

    m_interface->OMSetRenderTargets(totalElements,
                                    RenderTempPtrArr,
                                    depthStencilView.m_interface);
  }
  else
  {
    assert(("Error two many render targets", totalElements <= c_MaxRenderTargets));
  }
#elif OPENGL
#endif // DIRECTX
}

void
enDeviceContext::RSSetViewports(enViewport viewports[], uint8_t numViewports)
{
#if DIRECTX
  if( numViewports <= c_MaxViewPorts )
  {
    D3D11_VIEWPORT viewportsArr[c_MaxViewPorts];
    for( uint8_t i = 0; i < numViewports; ++i )
    {
      viewportsArr[i] = viewports[i].getDxViewport();
    }
    m_interface->RSSetViewports(numViewports, viewportsArr);
  }
  else
  {
    assert(("Error too many view-ports" && numViewports <= c_MaxViewPorts));
  }
#elif OPENGL 
  //TODO : make this have the ability to address multiple view-ports
  glViewport(0, 0, viewports[0].getWidth(), viewports[0].getHeight());

#endif // DIRECTX

}

void
enDeviceContext::IASetInputLayout(enInputLayout& inputLayout)
{
#if DIRECTX
  m_interface->IASetInputLayout(inputLayout.getInterface());
#elif OPENGL
#endif // DIRECTX
}

void
enDeviceContext::IASetVertexBuffers(enVertexBuffer vertexBuffer[], uint32_t numBuffers, uint32_t Slot)
{
#if DIRECTX

  if( numBuffers <= c_MaxVertexBuffers )
  {
    ID3D11Buffer* BufferTempPtrArr[c_MaxVertexBuffers];

    uint32_t strides[c_MaxVertexBuffers];
    uint32_t offSets[c_MaxVertexBuffers]{ 0 }; // use later 

    for( uint32_t i = 0; i < numBuffers; ++i )
    {
      BufferTempPtrArr[i] = vertexBuffer[i].getInterface();
      strides[i] = vertexBuffer->getStride();
    }

    m_interface->IASetVertexBuffers(Slot,
                                    numBuffers,
                                    BufferTempPtrArr,
                                    strides,
                                    offSets);
  }
  else
  {
    assert(("Error too many vertex buffer ", numBuffers <= c_MaxVertexBuffers));
  }

#elif OPENGL
  m_drawingData.currentVertexBuffer = vertexBuffer[0].getInterface();
#endif // DIRECTX
}

void
enDeviceContext::IASetIndexBuffer(enIndexBuffer& indexBuffer, int Format, int offSet)
{
#if DIRECTX
  m_interface->IASetIndexBuffer(indexBuffer.getInterface(), static_cast<DXGI_FORMAT>(Format), offSet);
#elif OPENGL
  m_drawingData.currentIndexBuffer = indexBuffer.getInterface();
  m_drawingData.currentFormat = Format;
#endif // DIRECTX
}

void
enDeviceContext::IASetPrimitiveTopology(int enTopology)
{
#if DIRECTX
  m_interface->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(enTopology));
#elif OPENGL
#endif // DIRECTX
}


void
enDeviceContext::UpdateSubresource(enBaseBuffer* Buffer,
                                   const void* originOfData)
{
#if DIRECTX
  m_interface->UpdateSubresource(Buffer->getInterface(),
                                 0,
                                 nullptr,
                                 originOfData,
                                 0,
                                 0);
#elif OPENGL
#endif//DIRECTX
}

void
enDeviceContext::ClearRenderTargetView(enRenderTargetView& renderTargetView,
                                       sColorf* color)
{
#if DIRECTX
  static constexpr sColorf directXColor{ 0.30f,0.30f,1.0f,1.0f };

  if( color == nullptr )
  {
    m_interface->ClearRenderTargetView(renderTargetView.m_interface,
                                       directXColor.allColor);
  }
  else
  {
    m_interface->ClearRenderTargetView(renderTargetView.m_interface,
                                       color->allColor);
  }
#elif OPENGL
#endif // DIRECTX
}

void
enDeviceContext::ClearDepthStencilView(enDepthStencilView& depthStencilView,
                                       bool ClearStencil,
                                       bool ClearDepth,
                                       uint8_t StencilClearValues,
                                       float DepthClearValue)
{
#if DIRECTX
  int clearFlags = 0x00;
  // adding the flags for the directX api
  if( ClearStencil ) { clearFlags |= D3D11_CLEAR_STENCIL; }
  if( ClearDepth ) { clearFlags |= D3D11_CLEAR_DEPTH; }

  m_interface->ClearDepthStencilView(depthStencilView.m_interface,
                                     clearFlags,
                                     DepthClearValue,
                                     StencilClearValues);
#elif OPENGL
#endif // DIRECTX
}

void
enDeviceContext::VSSetShader(enVertexShader& vertexShaderPath)
{
#if DIRECTX
  m_interface->VSSetShader(vertexShaderPath.getInterface(),
                           nullptr,
                           0);

#elif OPENGL
#endif // DIRECTX
}

void
enDeviceContext::VSSetConstantBuffer(enConstBuffer& Buffer,
                                     uint32_t Index)
{
#if DIRECTX
  // make sure i don't use more slot than directX has 
  if( Index <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )
  {
    this->m_interface->VSSetConstantBuffers(Index, 1, Buffer.getInterfaceRef());
  }
  else
  {
    assert("Error used too many slot " && Index <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1);
  }
#elif OPENGL
#endif // DIRECTX
}

void
enDeviceContext::PSSetShader(enPixelShader& pixelShader)
{
#if DIRECTX
  this->m_interface->PSSetShader(pixelShader.getInterface(),
                                 nullptr,
                                 0);

#endif // DIRECTX
}

void
enDeviceContext::PSSetShaderResources(enShaderResourceView shaderResources[],
                                      uint32_t numResources,
                                      uint32_t Slots)
{
#if DIRECTX
  // make sure i don't use too many slots 
  if( Slots <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 &&
     numResources <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )
  {

    ID3D11ShaderResourceView* ShaderPtrArr[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
    for( uint16_t i = 0; i < numResources; ++i )
    {
      ShaderPtrArr[i] = shaderResources[i].m_interface;
    }

    m_interface->PSSetShaderResources(Slots,
                                      numResources,
                                      ShaderPtrArr);

  }
  else
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::UnClassified);
    assert(("Error asking for too many slots", Slots <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1));
  }

#elif OPENGL
#endif// DIRECTX
}

void
enDeviceContext::PSSetShaderResources(std::vector<enShaderResourceView>& shaderResources,
                                      uint32_t Slot)
{
#if DIRECTX
  const bool is_withinLimits = (Slot <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 &&
                                shaderResources.size() <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1);

  if( is_withinLimits )

  {

    ID3D11ShaderResourceView* ShaderPtrArr[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{ 0 };
    uint32_t i = 0;
    for( enShaderResourceView& element : shaderResources )
    {
      ShaderPtrArr[i] = element.m_interface;
      ++i;
    }

    m_interface->PSSetShaderResources(Slot,
                                      shaderResources.size() - 1,
                                      ShaderPtrArr);

  }
  else
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::UnClassified);
    assert(("Error asking for too many slots", Slot <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1));
  }

#endif // DIRECTX

}

void
enDeviceContext::PSSetConstantBuffers(enConstBuffer& Buffer,
                                      uint32_t Slots)
{
#if DIRECTX

  if( Slots <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )
  {
    m_interface->PSSetConstantBuffers(Slots,
                                      1,
                                      Buffer.getInterfaceRef());
  }
  else
  {
    assert(("Error setting PSSetConstantBuffer ", Slots <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1));
  }

#endif // DIRECTX
}

void
enDeviceContext::PSSetSamplers(enSampler samplers[],
                               uint32_t numSamplers,
                               uint32_t slot)
{
#if DIRECTX
  bool const is_withinLimit = (slot <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 &&
                               numSamplers <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1);

  if( is_withinLimit )
  {
    ID3D11SamplerState* SamplerPtrArr[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
    for( uint8_t i = 0; i < numSamplers; ++i )
    {
      SamplerPtrArr[i] = samplers[i].m_interface;
    }
    m_interface->PSSetSamplers(slot, numSamplers, SamplerPtrArr);
  }
  else
  {
    assert(("Error setting Sampler PSSetSamplers", slot <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1));
  }

#endif // DIRECTX
}

void
enDeviceContext::PSSetSampler(enSampler& sampler)
{
#if DIRECTX

  bool const is_withinLimit = (sampler.getIndex() <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1);
  if( is_withinLimit )
  {
    m_interface->PSSetSamplers(sampler.getIndex(),
                               1,
                               sampler.getInterfaceRef());
  }
  else
  {
    assert(("Error setting Sampler PSSetSamplers", sampler.getIndex() <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1));
  }

#endif // DIRECTX
}

void
enDeviceContext::DrawIndexed(uint32_t indexCount)
{
#if DIRECTX
  this->m_interface->DrawIndexed(indexCount, 0u, 0);
#elif OPENGL
#endif // DIRECTX
}

bool
enDeviceContext::SetShaders(enVertexShader& vertexShader,
                            enPixelShader& pixelShader)
{
#if DIRECTX

  this->VSSetShader(vertexShader);
  this->PSSetShader(pixelShader);

  return true;
#elif OPENGL
#endif // DIRECTX
  return false;
}


