#pragma once
#include "util/Header.h"
#include "util/soModule.h"

class enVertexBuffer;
class enIndexBuffer;
class enConstBuffer;

class enVertexShader;
class enPixelShader;

class enSampler;
class enInputLayout;


  /**
  * @brief : takes care of creating all resources
  * @bug :
  */
class enDevice : public soModule<enDevice>
{
public:
  enDevice() = default;
  ~enDevice() = default;

public:
  /**
  * @brief :initializes the module 
  * @bug : no known bugs
  */
  int 
  OnStartUp(void *_Descriptor) override;

  /**
  * @brief : this is used to free any and all memory associated with the module
  * @bug :
  */
  void
  OnShutDown() override;;

public:

  bool 
  init();

  bool
  CreateRenderTargetView(enTexture2D& texture, enRenderTargetView& renderTraget);

  /**
  * @brief : create a render-target-view with one render-target.
  * @param[out] renderTragetView : the view into a render-target.
  * @param[in] targetIndex : 
  * @bug :
  */
  bool
  CreateRenderTargetView(enRenderTargetView& renderTragetView, uint32 targetIndex = 0);

  bool
  CreateTexture2D(sTextureDescriptor& Description, enTexture2D& Texture);

  bool
  CreateDepthStencilView(enDepthStencilView& Depth);

  bool
  CreateVertexShader(enVertexShader& vertexShader);

  bool
  CreatePixelShader(enPixelShader& pixelShader);

  bool 
  CreateInputLayout(enInputLayout& inputLayout,
                    enVertexShader& vertexShaderPath);

  bool
  CreateIndexBuffer(enIndexBuffer & indexBuffer);

  bool 
  CreateVertexBuffer(enVertexBuffer &vertexBuffer);

  bool
  CreateConstBuffer(enConstBuffer &constBuffer);

   //! Creates a sampler 
  bool
  CreateSamplerState(enSampler& sampler);

#if DIRECTX
/*! returns a point directX device */
  ID3D11Device*
  getInterface();
/*! returns a reference to a pointer of a directX device */
  ID3D11Device**
  getInterfaceRef();

private:

  ID3D11Device* m_interface = nullptr;

#elif OPENGL

  int32 m_interface = 0;
#endif // DIRECTX
};

