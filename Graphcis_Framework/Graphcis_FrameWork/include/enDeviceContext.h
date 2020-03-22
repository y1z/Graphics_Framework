#pragma once
#include "util/soModule.h"
#include "util/Header.h"
#include "enViewport.h"
#include "enInputLayout.h"

#include "enVertexBuffer.h"
#include "enIndexBuffer.h"
#include "enConstBuffer.h"

#include "enVertexShader.h"
#include "enPixelShader.h"
#include "enSampler.h"

class enShaderResourceView;


class enDeviceContext : public soModule<enDeviceContext>
{
public:
  enDeviceContext() = default;
  ~enDeviceContext() = default;


public:
  virtual void
    OnShutDown() override;


  virtual int
    OnStartUp(void* _Desc)override;

public:// functions 

#if DIRECTX
  //! for interfacing with directX 
  ID3D11DeviceContext*
  getInterface();
  //! for function that need 2 pointers to pointers 
  ID3D11DeviceContext**
  getInterfaceRef();
#endif // DIRECTX

  //! resets all things set by device context to default settings 
  void
    ClearState();

  /*! set render-targets and can set multiple render-targets
      \param renderTragetsViews [out] the render-targets to be set
      \param  depthStencilView [out] the set depth-stencil view(note there can only be 1)
      \param numRenderTargets [in] to tell how many render target there are */
  void
  OMSetRenderTargets(enRenderTargetView renderTragetsViews[],
                     enDepthStencilView& depthStencilView,
                     uint32_t numRenderTragets = 1);
  /**
  set render-targets and can set multiple render-targets
    \param renderTragetsViews [out] the render-targets to be set
    \param  depthStencilView [out] the set depth-stencil view(note there can only be 1)
  */
  void
  OMSetRenderTargets(std::vector<enRenderTargetView>& renderTragetsViews,
                     enDepthStencilView& depthStencilView);


  /*!set the view port */
  void
  RSSetViewports(enViewport viewports[],
                 uint8_t numViewports = 1u);

  void //! set's how to interpret the data from the shaders 
  IASetInputLayout(enInputLayout& inputLayout);

  void //! set's the vertex-buffers (can set multiple at a time )
  IASetVertexBuffers(enVertexBuffer vertexBuffer[],
                     uint32_t numBuffers,
                     uint32_t Slot = 0);
  /*! set's the index buffer
      \param indexBuffer [in] the index-buffer that going to be set
      \param Format [in] how to interpret indices from the index-buffer*/

  void
  IASetIndexBuffer(enIndexBuffer& indexBuffer, int Format, int offSet = 0);

  void/*! set's the topology */
  IASetPrimitiveTopology(int enTopology);

  /*! updates some resource on the graphic card
    \param Buffer [in] the buffer that contains the data that going to the graphics card
    \param originOfData [in] this is the data the graphics card is going to receive
    */
  void
  UpdateSubresource(enBaseBuffer* Buffer,
                    const void* originOfData);

  void
  ClearRenderTargetView(enRenderTargetView& renderTargetView,
                        sColorf* color = nullptr);

  void //! clear the depthStencil with the choice of only clearing the depth/stencil portion or both
  ClearDepthStencilView(enDepthStencilView& depthStencilView,
                        bool ClearStencil = false,
                        bool ClearDepth = true,
                        uint8_t StencilClearValues = 0,
                        float DepthClearValue = 1.0f);

  void /*!sets the vertex shader */
  VSSetShader(enVertexShader& vertexShaderPath);

  void /*! set's the constant buffers */
  VSSetConstantBuffer(enConstBuffer& Buffer,
                      uint32_t Index);

  void//! set's the pixel shader 
  PSSetShader(enPixelShader& pixelShader);

  void/*! set's the resources for the pixel shader (can set multiple)*/
  PSSetShaderResources(enShaderResourceView  ShaderResources[],
                       uint32_t numResources = 1,
                       uint32_t Slots = 0);

  void
  PSSetShaderResources(std::vector<enShaderResourceView>& shaderResources,
                       uint32_t Slot = 0);

    // TODO IMPLEMENT
  //void/*! set's the resources for the pixel shader (can only set one) */
  //  PSSetShaderResources(cMesh& ShaderResources, uint32_t Slot = 0);

  /*! setting the const-buffers for the pixel shader (can only set one at a time)
      \param Slots the individual slots where the constant buffer goes to.*/
  void
  PSSetConstantBuffers(enConstBuffer& Buffer,
                       uint32_t Slots);

  void/*!set's the samplers (can set multiple) */
  PSSetSamplers(enSampler samplers[],
                uint32_t numSamplers = 1,
                uint32_t slot = 0);

  void
  PSSetSampler(enSampler &sampler);


    /*! this is the function that draws to the window
      \param IndexCount [in] how many indexes to drawi
      \param indexOffset [in] if for some reason you what an off set ???
    */
  void
  DrawIndexed(uint32_t indexCount);


  /**/
  bool
  SetShaders(enVertexShader& vertexShader,
             enPixelShader& pixelShader);


public:
#if DIRECTX
  ID3D11DeviceContext* m_interface = nullptr;

#endif // DIRECTX

};

