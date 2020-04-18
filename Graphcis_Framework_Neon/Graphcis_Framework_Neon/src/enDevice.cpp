#include "enDevice.h"

#include "enVertexBuffer.h"
#include "enIndexBuffer.h"
#include "enConstBuffer.h"
#include "enSampler.h"
#include "enInputLayout.h"
#include "enVertexShader.h"
#include "enPixelShader.h"
#include "enShaderResourceView.h"
#include "enRenderTargetView.h"
#include "enDepthStencilView.h"
#include "enTexture2D.h"
#include "helperFucs.h"

#if DIRECTX
#include "DirectXTK/include/DDSTextureLoader.h"
#include "DirectXTK/include/WICTextureLoader.h"
namespace dx = DirectX;
#elif OPENGL
#include "SOIL2/include/SOIL2.h"

#endif // DIRECTX

#include <vector>



int
enDevice::OnStartUp(void* _Descriptor)
{
#if DIRECTX
  m_interface = nullptr;
#elif OPENGL
  m_interface = std::numeric_limits<uint32>::max();
#endif // DIRECTX
  return 0;
}

void
enDevice::OnShutDown()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface);
#elif OPENGL
  m_interface = std::numeric_limits<uint32>::max();
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
enDevice::CreateRenderTargetView(enRenderTargetView& renderTraget,
                                 enTexture2D& texture,
                                 sRenderTargetDesc2D& Desc)
{
#if DIRECTX
  HRESULT hr = S_FALSE;
  D3D11_RENDER_TARGET_VIEW_DESC directXDesc;
  std::memset(&directXDesc, 0, sizeof(directXDesc));
  directXDesc.Format = static_cast<DXGI_FORMAT>(Desc.format);
  directXDesc.ViewDimension = static_cast<D3D11_RTV_DIMENSION> (Desc.renderTargetType);
  directXDesc.Texture2D.MipSlice = Desc.mip;

  hr = m_interface->CreateRenderTargetView(texture.m_interface,
                                           &directXDesc,
                                           &renderTraget.m_interface);
  if( SUCCEEDED(hr) )
  {
    return true;
  }

#elif OPENGL
  //TODO :  set up the frame buffer  http://docs.gl/gl4/glGenFramebuffers
  GlRemoveAllErrors();

  enTexture2D* ptrTexture = &texture;

  float const width = ptrTexture->m_desc.texWidth;
  float const height = ptrTexture->m_desc.texHeight;

  glGenRenderbuffers(1, &ptrTexture->m_interface);

  glBindRenderbuffer(GL_RENDERBUFFER,
                     static_cast<GLuint>(ptrTexture->getInterface()));

  glRenderbufferStorage(GL_RENDERBUFFER,
                        enFormats::renderTarget_format,
                        width,
                        height);

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glGenFramebuffers(1, &renderTraget.m_interface);

  glBindFramebuffer(GL_FRAMEBUFFER,
                    renderTraget.m_interface);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER,
                            static_cast<GLuint>(ptrTexture->getInterface()));


  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if( status != GL_FRAMEBUFFER_COMPLETE )
  {
    std::string ErrorCode = std::to_string(status);
    assert(true == false && "Frame buffer Errror :");
  }

  if( !GlCheckForError() )
  {
    // un bind the buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif // DIRECTX
  return false;
}

bool
enDevice::CreateRenderTargetView(enRenderTargetView& renderTraget,
                                 uint32 targetIndex)
{

  if( renderTraget.s_renderTargetMax - 1 < targetIndex )
  {
    EN_LOG_DB("target index going out of bounds");
    return false;
  }
#if DIRECTX
  HRESULT hr = S_FALSE;
  if( renderTraget.m_targets[targetIndex].m_interface )
  {
    hr = m_interface->CreateRenderTargetView(renderTraget.m_targets[targetIndex].m_interface,
                                             NULL,
                                             &renderTraget.m_interface);

  }

  if( SUCCEEDED(hr) )
  {
    renderTraget.m_usedTargets[targetIndex] = true;
    renderTraget.m_targetsCount++;

    return true;
  }
#elif OPENGL
  //TODO :  set up the frame buffer  http://docs.gl/gl4/glGenFramebuffers
  GlRemoveAllErrors();

  enTexture2D* ptrTexture = &renderTraget.m_targets[targetIndex];
  float const width = ptrTexture->m_desc.texWidth;
  float const height = ptrTexture->m_desc.texHeight;

  glGenRenderbuffers(1, &ptrTexture->m_interface);

  glBindRenderbuffer(GL_RENDERBUFFER,
                     static_cast<GLuint>(ptrTexture->getInterface()));

  glRenderbufferStorage(GL_RENDERBUFFER,
                        enFormats::renderTarget_format,
                        width,
                        height);

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glGenFramebuffers(1, &renderTraget.m_interface);

  glBindFramebuffer(GL_FRAMEBUFFER,
                    renderTraget.m_interface);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER,
                            static_cast<GLuint>(ptrTexture->getInterface()));


  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if( status != GL_FRAMEBUFFER_COMPLETE )
  {
    std::string ErrorCode = std::to_string(status);
    assert(true == false && "Frame buffer Errror :");
  }

  if( !GlCheckForError() )
  {
    // un bind the buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return false;
#endif // DIRECTX
  return false;
}

bool
enDevice::CreateRenderTargetView(enRenderTargetView& renderTragetView,
                                 enTexture2D& texture)
{

#if DIRECTX
  HRESULT hr = S_FALSE;
  hr = m_interface->CreateRenderTargetView(texture.m_interface,
                                           NULL,
                                           &renderTragetView.m_interface);


  if( SUCCEEDED(hr) )
  {
    return true;
  }
#elif OPENGL
  //TODO :  set up the frame buffer  http://docs.gl/gl4/glGenFramebuffers
  GlRemoveAllErrors();
  enTexture2D* ptrTexture = &texture;
  float const width = ptrTexture->m_desc.texWidth;
  float const height = ptrTexture->m_desc.texHeight;

  glGenRenderbuffers(1, &ptrTexture->m_interface);

  glBindRenderbuffer(GL_RENDERBUFFER,
                     static_cast<GLuint>(ptrTexture->getInterface()));

  glRenderbufferStorage(GL_RENDERBUFFER,
                        enFormats::renderTarget_format,
                        width,
                        height);


  glGenFramebuffers(1, texture.getInterfacePtr());

  glBindFramebuffer(GL_FRAMEBUFFER,
                    texture.getInterface());

  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER,
                            static_cast<GLuint>(ptrTexture->getInterface()));



  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  //if (status != GL_FRAMEBUFFER_COMPLETE)
  //{
  //	 std::string ErrorCode = std::to_string(status);
  //	 assert(true == false && "Frame buffer Errror :");
  //}

  if( !GlCheckForError() )
  {
    // un bind the buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return true;
#endif // DIRECTX
  return false;
}

bool
enDevice::CreateShaderResourceFromFile(enShaderResourceView& shaderResourceView,
                                       std::string_view filePath)
{
#if DIRECTX

  std::wstring wideFilePath = helper::convertStringToWString(filePath);

  HRESULT hr = S_FALSE;
  if( filePath.find(".dds") != filePath.npos )
  {
    hr = dx::CreateDDSTextureFromFile(m_interface, wideFilePath.c_str(), nullptr, &shaderResourceView.m_interface);
    if( FAILED(hr) )
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
      return false;
    }
  }
  else
  {
    hr = dx::CreateWICTextureFromFile(m_interface, wideFilePath.c_str(), nullptr, &shaderResourceView.m_interface);

    if( FAILED(hr) )
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
      return false;
    }
  }

#elif OPENGL
  GlRemoveAllErrors();
  std::string const imagePath(filePath);

  shaderResourceView.m_interface = SOIL_load_OGL_texture
  (
    imagePath.c_str(),
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB 
  );

  /* check for an error during the load process */
  if( 0 == shaderResourceView.m_interface )
  {
    printf("SOIL loading error: '%s'\n", SOIL_last_result());
  }
  else
  {
    glBindTexture(GL_TEXTURE_2D, shaderResourceView.m_interface);

  }

  if( GlCheckForError() )
  {
    EN_LOG_DB(" Error Creating Shader-Resource from File ");
    return false;
  }

#endif // DIRECTX

  return true;
}

bool
enDevice::CreateShaderResourceFromTexture(enShaderResourceView& shaderResourceView,
                                          enTexture2D& texture)
{
#if DIRECTX
  D3D11_SHADER_RESOURCE_VIEW_DESC  ViewDescriptor;
  std::memset(&ViewDescriptor, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
  ViewDescriptor.Format = static_cast<DXGI_FORMAT>(texture.m_desc.texFormat);
  ViewDescriptor.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
  ViewDescriptor.Texture2D.MipLevels = texture.m_desc.Mips;

  HRESULT const hr = 
  m_interface->CreateShaderResourceView(texture.getInterface(),
                                        &ViewDescriptor,
                                        &shaderResourceView.m_interface);

  if( FAILED(hr) )
  {
    return false;
  }

#elif OPENGL
// TODO : Create a shader Resource From a Texture.
#endif // DIRECTX
  return true;
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
  GlRemoveAllErrors();

  glGenTextures(1, Texture.getInterfacePtr());

  glBindTexture(GL_TEXTURE_2D, Texture.getInterface());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               Description.texWidth,
               Description.texHeight,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               NULL);

  glBindTexture(GL_TEXTURE_2D, 0);

  if( !GlCheckForError() )
  {
    return true;
  }
#endif // DIRECTX
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

  hr = m_interface->CreateDepthStencilView(DepthView.m_texture.m_interface,
                                           &descDSV,
                                           &DepthView.m_interface);

  if( SUCCEEDED(hr) )
  {
    return true;
  }

#elif OPENGL
  GlRemoveAllErrors();
  enTexture2D& refToTexture = DepthView.m_texture;

  glGenRenderbuffers(1, &DepthView.m_interface);
  glBindRenderbuffer(GL_RENDERBUFFER, DepthView.m_interface);

  glRenderbufferStorage(GL_RENDERBUFFER,
                        GL_DEPTH_COMPONENT,
                        refToTexture.m_desc.texWidth,
                        refToTexture.m_desc.texHeight);

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  if( !GlCheckForError() )
  {
    return true;
  }
#endif // DIRECTX
  return false;
}

bool
enDevice::CreateDepthStencilView(enDepthStencilView& DepthView,
                                 enTexture2D& Texture)
{
#if DIRECTX
  HRESULT hr;
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  SecureZeroMemory(&descDSV, sizeof(descDSV));
  descDSV.Format = static_cast<DXGI_FORMAT>(DepthView.m_desc.Format);
  descDSV.ViewDimension = static_cast<D3D11_DSV_DIMENSION>(DepthView.m_desc.Dimension);
  descDSV.Texture2D.MipSlice = DepthView.m_desc.Mip;

  hr = m_interface->CreateDepthStencilView(Texture.m_interface,
                                           &descDSV,
                                           &DepthView.m_interface);

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
  hr = m_interface->CreateVertexShader(vertexShader.getShaderInfo()->GetBufferPointer(),
                                       vertexShader.getShaderInfo()->GetBufferSize(),
                                       NULL,
                                       vertexShader.getInterfaceRef());

  if( SUCCEEDED(hr) )
  {
    return true;
  }

#elif OPENGL
  GlRemoveAllErrors();
  glAttachShader(*cApiComponents::getShaderProgram(), vertexShader.getShaderInfo());
  glLinkProgram(*cApiComponents::getShaderProgram());
  glValidateProgram(*cApiComponents::getShaderProgram());

  int Status;
  glGetProgramiv(*cApiComponents::getShaderProgram(), GL_VALIDATE_STATUS, &Status);
  if( Status == GL_FALSE )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::ShaderLinkError);
  }

  if( GlCheckForError() )
  {
    return false;
  }

  return true;
#endif // DIRECTX
  return false;
}

bool
enDevice::CreatePixelShader(enPixelShader& pixelShader)
{
#if DIRECTX
  HRESULT hr = S_FALSE;
  hr = m_interface->CreatePixelShader(pixelShader.getShaderInfo()->GetBufferPointer(),
                                      pixelShader.getShaderInfo()->GetBufferSize(),
                                      NULL,
                                      pixelShader.getInterfaceRef());

  if( SUCCEEDED(hr) )
  {
    return true;
  }
#elif OPENGL
  GlRemoveAllErrors();
  glAttachShader(*cApiComponents::getShaderProgram(), pixelShader.getShaderInfo());
  glLinkProgram(*cApiComponents::getShaderProgram());
  glValidateProgram(*cApiComponents::getShaderProgram());

  int Status;
  glGetProgramiv(*cApiComponents::getShaderProgram(), GL_VALIDATE_STATUS, &Status);
  if( Status == GL_FALSE )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::ShaderLinkError);
  }

  if( GlCheckForError() )
  {
    return false;
  }

  return true;

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
  std::vector<sInputDescriptor> intermidateLayout = inputLayout.getInputDesenriptor();
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
                                               vertexShaderPath.getShaderInfo()->GetBufferPointer(),
                                               vertexShaderPath.getShaderInfo()->GetBufferSize(),
                                               inputLayout.getInterfaceRef());
  if( SUCCEEDED(hr) )
  {
    return true;
  }

#elif OPENGL  
  GlRemoveAllErrors();
  glEnableVertexAttribArray(0);// position 
  glEnableVertexAttribArray(1);// normals 

  if( GlCheckForError() )
  {
    EN_LOG_ERROR;
    return false;
  }
  return true;
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
  GlRemoveAllErrors();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.getInterface());

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, // buffer type 
               indexBuffer.getBufferSize(),
               indexBuffer.getData(),
               GL_STATIC_DRAW);


  int32 size = 0;
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER,
                         GL_BUFFER_SIZE,
                         &size);

  if( size != indexBuffer.getBufferSize() )
  {
    EN_LOG_ERROR;
    return false;
  }


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if( GlCheckForError() )
  {
    EN_LOG_ERROR;
    return false;
  }
  else
  {
    return true;
  }
#endif// DIRECTX

  return false;
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
#elif OPENGL
  GlRemoveAllErrors();

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.getInterface());

  glBufferData(GL_ARRAY_BUFFER, // buffer type 
               vertexBuffer.getBufferSize(),
               vertexBuffer.getData(),
               GL_DYNAMIC_DRAW);


  int32 size = 0;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

  if( size != vertexBuffer.getBufferSize() )
  {
    EN_LOG_ERROR;
    return false;
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);


  if( GlCheckForError() )
  {
    EN_LOG_ERROR;
    return false;
  }
  else
  {
    return true;
  }
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
#elif OPENGL
  GlRemoveAllErrors();

  uint32* shaderProgram = cApiComponents::getShaderProgram();

  glUseProgram(*shaderProgram);

  auto refToContainer = &constBuffer.m_containedVariables;

  auto FindAndAddUniformID = [&refToContainer](uint32* ShaderProgram) {

    for(size_t i = 0; i < refToContainer->size(); ++i )
    {
      refToContainer->at(i).id = glGetUniformLocation(*ShaderProgram,
                                                      refToContainer->at(i).name.c_str());
    }
  };

  if( constBuffer.getIndex() == 0 )
  {
    refToContainer->push_back(helper::GlCreateUniformDetail("u_view", enConstBufferElem::mat4x4));
    FindAndAddUniformID(shaderProgram);

  }
  else if( constBuffer.getIndex() == 1 )
  {
    refToContainer->push_back(helper::GlCreateUniformDetail("u_projection", enConstBufferElem::mat4x4));
    FindAndAddUniformID(shaderProgram);
  }
  else if( constBuffer.getIndex() == 2 )
  {
    refToContainer->push_back(helper::GlCreateUniformDetail("u_world", enConstBufferElem::mat4x4));
    refToContainer->push_back(helper::GlCreateUniformDetail("uColor", enConstBufferElem::vec4));
    FindAndAddUniformID(shaderProgram);
  }


  if( !GlCheckForError() )
  {
    return true;
  }
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
#elif OPENGL 
// TODO : add sampler functionality


  return true;
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
