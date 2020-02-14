#pragma once
#include "util/Header.h"
#include "util/soModule.h"

class enVertexBuffer;
class enIndexBuffer;
class enConstBuffer;


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

  void
  OnStartUp(void *_Descriptor) override;

  void
  OnShutDown() override;;

public:

  bool 
  init();

  bool
  CreateRenderTargetView(enTexture2D& texture, enRenderTargetView& renderTraget);

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


private:
#if DIRECTX

  ID3D11Device* m_interface = nullptr;

#elif OPENGL

  int32 m_interface = 0;
#endif // DIRECTX

  static bool is_Initalized;
};

