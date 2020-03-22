#pragma once
#include "util/Header.h"
#include "util/soModule.h"

#include <string_view>
  /**
  * FORWARD DECLARATIONS
  */
class enVertexBuffer;
class enIndexBuffer;
class enConstBuffer;
class enVertexShader;
class enPixelShader;
class enSampler;
class enInputLayout;
class enShaderResourceView;


  /**
  * @brief : takes care of creating all resources
  * @bug : no known bugs.
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
  * @bug : no known bugs.
  */
  void
  OnShutDown() override;;

public:

  bool 
  init();

  /**
  * @brief : creates a render-target View.
  * @param[in] texture: used as a render-target.
  * @param[out] renderTraget: the resulting RenderTargetView.
  * @bug : no known bugs.
  */
  bool
  CreateRenderTargetView(enTexture2D& texture,
                         enRenderTargetView& renderTraget);

  /**
  * @brief : create a render-target-view with one render-target.
  * @returns :true if the operation succeeded, False other wise.
  * @param[out] renderTragetView : the view into a render-target.
  * @param[in] targetIndex :which of the texture own by the renderTarget View to use.
  * @bug : no known bugs.
  */
  bool
  CreateRenderTargetView(enRenderTargetView& renderTragetView,
                         uint32 targetIndex = 0);

  /**
  * @brief :create a render-target-view with a detached texture.
  * @returns :true if the operation succeeded, False other wise.
  * @param[out] renderTragetView : the view into a render-target.
  * @param[in] texture :which of the texture own by the renderTarget View to use.
  * @bug : no known bugs.
  */
  bool
  CreateRenderTargetView(enRenderTargetView& renderTragetView,
                         enTexture2D& texture);

  /**
  * @brief : creates makes a texture usable by a shader.
  * @returns :true if the operation succeeded, False other wise.
  * @param[out] shaderResourceView: the resulting ShaderResourceView. 
  * @bug : no known bug.
  */
  bool
  CreateShaderResourceFromFile(enShaderResourceView &shaderResourceView,
                               std::string_view filePath);

  /**
  * @brief :creates a texture.
  * @returns :true if the operation succeeded, False other wise.
  * @param[in] Description : controls how the texture is made.
  * @param[out] Texture: the resulting texture.
  * @bug : no known bugs.
  */
  bool
  CreateTexture2D(sTextureDescriptor& Description,
                  enTexture2D& Texture);

  /**
  * @brief : creates a depth-StencilView with it's own texture.
  * @returns :true if the operation succeeded, False other wise.
  * @param[out] Depth: the resulting Depth-Stencil View.
  * @bug : no known bugs.
  */
  bool
  CreateDepthStencilView(enDepthStencilView& Depth);

  /**
  * @brief : creates a depth-StencilView using a different texture from it's own.
  * @param[out] DepthView: the resulting DepthStencilView.
  * @param[in] Texture: the texture that used as the DepthStencil.
  * @bug : no known bugs.
  */
  bool
  CreateDepthStencilView(enDepthStencilView& DepthView,
                         enTexture2D& Texture);

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

