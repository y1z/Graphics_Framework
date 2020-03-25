//--------------------------------------------------------------------------------------
// my includes 
//--------------------------------------------------------------------------------------
#include "..\include\appGraphics.h"
#include "helperFucs.h"
#include "Resource.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"

#include "DirectXTK/include/DDSTextureLoader.h"
#include "DirectXTK/include/WICTextureLoader.h"

#include "../include/enPerspectiveFreeCamera.h"
#include "../include/enFirstPersonCamera.h"

#include "enCameraManager.h"
#include "enDevice.h"
#include "enDeviceContext.h"
#include "enShaderResourceView.h"
#include "enModel.h"

//--------------------------------------------------------------------------------------
// standard  includes 
//--------------------------------------------------------------------------------------
#include <iostream>
namespace dx = DirectX;
using std::make_unique;
using std::make_shared;


enPerspectiveFreeCamera* appGraphics::s_Camera = nullptr;
enFirstPersonCamera* appGraphics::s_FirstPersonCamera = nullptr;
enCameraManager* appGraphics::s_CameraManager = nullptr;

bool appGraphics::s_initIsFinish = false;
bool appGraphics::s_useFreeCam = true;

enConstBuffer* appGraphics::s_ViewMatrixBuffer = nullptr;
enConstBuffer* appGraphics::s_ProjectionMatrixBuffer = nullptr;

std::unique_ptr<imGuiManager> appGraphics::m_gui = make_unique<imGuiManager>();

bool
appGraphics::init()
{
  BOOL checkIfSucceeded = GetModuleHandleEx(0x00, NULL, &m_moduleInstance);

  if( checkIfSucceeded == FALSE )
    return false;

  /*Initializes the COM library
  this is so I can load images for directX */
  if( FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)) )
  {
    return false;
  }

  if( !InitStatics() )
    return false;

  if( !initModules() )
    return false;

  if( !createMyClasses() )
  {
    this->destroy();
    return false;
  }

  if( FAILED(InitWindow(m_moduleInstance, SW_SHOWDEFAULT)) )
  {
    this->destroy();
    return false;
  }

  if( initContainers() == false )
  {
    this->destroy();
    return false;
  }

  if( !EN_SUCCESS(initApi()) )
  {
    this->destroy();
    return false;
  }

  if( initForRender() == S_FALSE )
  {
    this->destroy();
    return false;
  }
  //enVector2 newSize = enVector2(2000, 2000);
  //bool isSuccessful = this->m_swapchain->ResizeSwapChain(*m_window,
  //                                                       *m_renderTargetView,
  //                                                       *m_depthStencilView,
  //                                                       newSize,
  //                                                       *m_viewport);

  //assert(isSuccessful);

  s_initIsFinish = true;

  return true;
}

int
appGraphics::run()
{
  // Main message loop
  MSG msg = { 0 };
  static glm::vec2 windowSize = helper::getWindowSize(*m_window);
  while( WM_QUIT != msg.message )
  {
    //glm::vec2 currentWindowSize = helper::getWindowSize(*m_window);
    if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      this->Render();
      this->Update();
    }
  }

  return (int)msg.wParam;
}

void
appGraphics::destroy()
{
  CoUninitialize();


  DELETE_PTR(s_Camera);
  DELETE_PTR(s_FirstPersonCamera);
  DELETE_PTR(s_CameraManager);
  DELETE_PTR(s_ViewMatrixBuffer);
  DELETE_PTR(s_ProjectionMatrixBuffer);

  m_ConstBufferContainer.clear();

  enDevice::ShutDown();
  enDeviceContext::ShutDown();
}


bool
appGraphics::InitStatics()
{
  try
  {
    s_Camera = new enPerspectiveFreeCamera();
    s_FirstPersonCamera = new enFirstPersonCamera();
    s_CameraManager = new enCameraManager();
    s_ViewMatrixBuffer = new enConstBuffer();
    s_ProjectionMatrixBuffer = new enConstBuffer();
  }
  catch( const std::bad_alloc & allocError )
  {
    std::cout << allocError.what() << std::endl;
    return false;
  }


  return true;
}

bool
appGraphics::initModules()
{
  if( enDevice::StartUp(nullptr) == -1 )
    return false;

  if( enDeviceContext::StartUp(nullptr) == -1 )
    return false;

  return true;
}

enErrorCode
appGraphics::initApi()
{
  enErrorCode checkForError = helper::CreateDeviceAndSwapchain(*m_swapchain,
                                                               *m_window,
                                                               m_hardwareInfo);



  if( !m_gui->is_initialized )
  {
    m_gui->Init(*m_window);
  }

  return checkForError;
}

bool
appGraphics::createMyClasses()
{
  try
  {
    m_vertexShader = make_unique<enVertexShader>();
    m_pixelShader = make_unique<enPixelShader>();

    m_vertexBuffer = make_unique<enVertexBuffer>();
    m_indexBuffer = make_unique<enIndexBuffer>();

    m_renderTargetView = make_unique<enRenderTargetView>();
    m_inputLayout = make_unique<enInputLayout>();

    m_depthStencilView = make_unique<enDepthStencilView>();
    m_viewport = make_unique<enViewport>();

    m_worldMatrix = make_unique<enConstBuffer>();

    m_resourceView = make_shared<enShaderResourceView>();
    m_sampler = make_unique<enSampler>();

    m_swapchain = make_unique<enSwapChain>();
    m_window = make_unique<enWindow>();

    m_model = make_unique<enModel>();
    m_renderTargetAndShaderResource = make_unique<enMultiviewTexture>();
  }
  catch( const std::exception & e )
  {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

bool
appGraphics::initContainers()
{
  m_ConstBufferContainer.push_back(s_ViewMatrixBuffer);
  m_ConstBufferContainer.push_back(s_ProjectionMatrixBuffer);

  return true;
}

HRESULT
appGraphics::initForRender()
{
  enDevice& device = enDevice::getInstance();
  enDeviceContext& deviceContext = enDeviceContext::getInstance();
  bool isSuccessful = false;

  HRESULT hr = S_FALSE;

  enVector2 windowSize = helper::getWindowSize(*m_window);

  // Create a render target view
  m_swapchain->ReciveBuckBuffer(*m_renderTargetView);

  isSuccessful = device.CreateRenderTargetView(*m_renderTargetView, 0);
  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }


  isSuccessful = device.CreateRenderTargetView(*m_renderTargetView, 1);

 //if( !EN_SUCCESS(m_renderTargetAndShaderResource->CreateRenderTarget(windowSize.x, windowSize.y, enFormats::fR16G16B16A16)) )
 //{
 //  EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
 //  return S_FALSE;
 //}

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }

  sTextureDescriptor descriptoDepth;
  descriptoDepth.texWidth = windowSize.x;
  descriptoDepth.texHeight = windowSize.y;
  descriptoDepth.CpuAccess = 0;
  descriptoDepth.texFormat = static_cast<int>(enFormats::depthStencil_format);
  descriptoDepth.Usage = enBufferUse::Default;
  descriptoDepth.BindFlags = enBufferBind::DepthStencil;
  descriptoDepth.arraySize = 1;

  isSuccessful = device.CreateTexture2D(descriptoDepth, m_depthStencilView->m_texture);
  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }

  sDepthStencilDescriptor DepthStencilDesc;
  DepthStencilDesc.Format = descriptoDepth.texFormat;
  DepthStencilDesc.Dimension = DepthStencilFormat::two_dimention;
  DepthStencilDesc.Mip = 0;

  m_depthStencilView->m_desc = DepthStencilDesc;
  isSuccessful = device.CreateDepthStencilView(*m_depthStencilView);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }

  deviceContext.OMSetRenderTargets(m_renderTargetView.get(),
                                   m_depthStencilView.get());
                                   //deviceContext.OMSetRenderTargets()
                                   // Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)windowSize.x;
  vp.Height = (FLOAT)windowSize.y;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;

  sViewportDesc viewDescriptor;
  viewDescriptor.width = static_cast<float>(windowSize.x);
  viewDescriptor.height = static_cast<float>(windowSize.y);
  viewDescriptor.maxDepth = 1.0f;

  m_viewport->init(viewDescriptor);

  deviceContext.RSSetViewports(m_viewport.get());

  enErrorCode errorCode = m_vertexShader->compileShaderFromFile("GraphcisFramework.fx",
                                                                "VS",
                                                                "vs_4_0");

  if( !EN_SUCCESS(errorCode) )
  {
    EN_LOG_ERROR_WITH_CODE(errorCode);
    MessageBox(NULL,
               L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.",
               L"Error",
               MB_OK);
    return S_FALSE;
  }


  // Create the vertex shader
  //hr = device.getInterface()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_vertexShader->m_interface);
  isSuccessful = device.CreateVertexShader(*m_vertexShader);
  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }

  m_inputLayout->ReadShaderData(*m_vertexShader);


  isSuccessful = device.CreateInputLayout(*m_inputLayout, *m_vertexShader);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }
  // Set the input layout
  //p_ImmediateContext->IASetInputLayout(m_inputLayout->getInterface());
  deviceContext.IASetInputLayout(*m_inputLayout);

  isSuccessful = m_pixelShader->compileShaderFromFile("GraphcisFramework.fx",
                                                      "PS",
                                                      "ps_4_0");
  if( FAILED(hr) )
  {
    MessageBox(NULL,
               L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.",
               L"Error",
               MB_OK);
    return hr;
  }

  // Create the pixel shader
  isSuccessful = device.CreatePixelShader(*m_pixelShader);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }

  // Create vertex buffer
  //SimpleVertex vertices[] =
  //{
  //    { glm::vec4(-1.0f, 1.0f, -1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
  //    { glm::vec4(1.0f, 1.0f, -1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
  //    { glm::vec4(1.0f, 1.0f, 1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
  //    { glm::vec4(-1.0f, 1.0f, 1.0f,1.0f), glm::vec2(0.0f, 1.0f) },

  //    { glm::vec4(-1.0f, -1.0f, -1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
  //    { glm::vec4(1.0f, -1.0f, -1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
  //    { glm::vec4(1.0f, -1.0f, 1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
  //    { glm::vec4(-1.0f, -1.0f, 1.0f,1.0f), glm::vec2(0.0f, 1.0f) },

  //    { glm::vec4(-1.0f, -1.0f, 1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
  //    { glm::vec4(-1.0f, -1.0f, -1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
  //    { glm::vec4(-1.0f, 1.0f, -1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
  //    { glm::vec4(-1.0f, 1.0f, 1.0f,1.0f), glm::vec2(0.0f, 1.0f) },

  //    { glm::vec4(1.0f, -1.0f, 1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
  //    { glm::vec4(1.0f, -1.0f, -1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
  //    { glm::vec4(1.0f, 1.0f, -1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
  //    { glm::vec4(1.0f, 1.0f, 1.0f,1.0f), glm::vec2(0.0f, 1.0f) },

  //    { glm::vec4(-1.0f, -1.0f, -1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
  //    { glm::vec4(1.0f, -1.0f, -1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
  //    { glm::vec4(1.0f, 1.0f, -1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
  //    { glm::vec4(-1.0f, 1.0f, -1.0f,1.0f), glm::vec2(0.0f, 1.0f) },

  //    { glm::vec4(-1.0f, -1.0f, 1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
  //    { glm::vec4(1.0f, -1.0f, 1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
  //    { glm::vec4(1.0f, 1.0f, 1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
  //    { glm::vec4(-1.0f, 1.0f, 1.0f,1.0f), glm::vec2(0.0f, 1.0f) },
  //};


  //sBufferDesc VertexBuffdescriptor;
  //VertexBuffdescriptor.cpuAccess = 0;
  //VertexBuffdescriptor.bindFlags = enBufferBind::Vertex;
  //VertexBuffdescriptor.elementCount = ARRAYSIZE(vertices);
  //VertexBuffdescriptor.stride = sizeof(SimpleVertex);
  //VertexBuffdescriptor.ptr_data = vertices;


  if( m_model->LoadModelFromFile("ryuko.fbx") == false )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }


  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }

  // Set vertex buffer
  UINT stride = sizeof(SimpleVertex);
  UINT offset = 0;

  deviceContext.IASetVertexBuffers(m_vertexBuffer.get(), 1);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }

  deviceContext.IASetIndexBuffer(*m_indexBuffer, enFormats::uR16);

  deviceContext.IASetPrimitiveTopology(static_cast<int>(enTopology::TriList));

  // Create the constant buffers
  sBufferDesc viewBuffer;
  viewBuffer.bindFlags = enBufferBind::Const;
  viewBuffer.stride = sizeof(viewMatrix);
  viewBuffer.elementCount = 1;
  viewBuffer.cpuAccess = 0;
  viewBuffer.index = 0;

  s_ViewMatrixBuffer->init(viewBuffer);

  device.CreateConstBuffer(*s_ViewMatrixBuffer);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }


  sBufferDesc ProjectionDescriptor;
  ProjectionDescriptor.bindFlags = enBufferBind::Const;
  ProjectionDescriptor.elementCount = 1;
  ProjectionDescriptor.stride = sizeof(projectionMatrix);
  ProjectionDescriptor.index = 1;

  s_ProjectionMatrixBuffer->init(ProjectionDescriptor);


  isSuccessful = device.CreateConstBuffer(*s_ProjectionMatrixBuffer);


  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }

  sBufferDesc worldMatrixDescriptor;
  worldMatrixDescriptor.elementCount = 1;
  worldMatrixDescriptor.stride = sizeof(ConstBufferWorldColor);
  worldMatrixDescriptor.bindFlags = enBufferBind::Const;
  worldMatrixDescriptor.usage = enBufferUse::Default;
  worldMatrixDescriptor.index = 2;

  m_worldMatrix->init(worldMatrixDescriptor);

  isSuccessful = device.CreateConstBuffer(*m_worldMatrix);


  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }

  m_resourceView->init();

  // Load the Texture
  isSuccessful = device.CreateShaderResourceFromFile(*m_resourceView,
                                                     "neon light.jpg");

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }

  enMultiViewType EverySingleView = helper::generateMultiViewType(enMultiViewType::renderTarget |
                                                                  enMultiViewType::shaderResource);
 
  //EverySingleView = static_cast<enMultiViewType>(EverySingleView | enMultiViewType::renderTarget);
  
  isSuccessful = m_renderTargetAndShaderResource->CreateAll(windowSize.x,
                                        windowSize.y,
                                        enFormats::fR16G16B16A16,
                                        enBufferUse::Default,
                                        EverySingleView);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }


  {
    sSamplerDesc samplerDescriptor;
    samplerDescriptor.filter = static_cast<uint32_t>(enFilter::MinMagMip_Linear);
    samplerDescriptor.addressU = static_cast<uint32_t>(enAddress::Wrap);
    samplerDescriptor.addressV = static_cast<uint32_t>(enAddress::Wrap);
    samplerDescriptor.addressW = static_cast<uint32_t>(enAddress::Wrap);
    samplerDescriptor.comparingFunc = static_cast<int>(enComparison::Never);
    samplerDescriptor.minLod = 0.0f;
    samplerDescriptor.maxLod = std::numeric_limits<float>::max();
    samplerDescriptor.AnisotropicLevel = 1u;
    samplerDescriptor.index = 0;

    m_sampler->init(samplerDescriptor);
    isSuccessful = device.CreateSamplerState(*m_sampler);
    if( !isSuccessful )
    {
      EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
      return S_FALSE;
    }
  }

//************************************//************************************//************************************/************************************/
//************************************//************************************//************************************/************************************/
  sPerspectiveCameraDesc descriptorCamera;
  descriptorCamera.upDir = enVector3(0.0f, 1.0f, 0.0f);
  descriptorCamera.lookAtPosition = enVector3(0.0f, 0.0f, -1.0f);
  descriptorCamera.height = windowSize.y;
  descriptorCamera.width = windowSize.x;

  s_Camera->init(descriptorCamera);


  sFirstPersonCameraDesc cameraDescriptor;
  s_FirstPersonCamera->init(cameraDescriptor);

  s_CameraManager->addCamera(s_Camera);

  s_CameraManager->addCamera(s_FirstPersonCamera);

  // Initialize the world matrices
  m_World = glm::identity<glm::mat4x4>();

  // Initialize the view matrix
  glm::vec3 Eye(0.0f, 3.0f, -6.0f);
  glm::vec3 At(0.0f, 0.0f, -1.0f);
  glm::vec3 Up(0.0f, 1.0f, 0.0f);

  viewMatrix cbNeverChanges;
  cbNeverChanges.mView = glm::transpose(s_Camera->getView() /* s_Camera.getView()*/);


  deviceContext.UpdateSubresource(s_ViewMatrixBuffer, &cbNeverChanges);


  // Initialize the projection matrix
  m_Projection = glm::perspectiveFovLH(glm::quarter_pi<float>(),
                                       windowSize.x,
                                       windowSize.y,
                                       0.01f,
                                       100.0f);

  projectionMatrix cbChangesOnResize;
  cbChangesOnResize.mProjection = glm::transpose(s_Camera->getProjection() /*s_Camera.getProjection()*/);

  deviceContext.UpdateSubresource(s_ProjectionMatrixBuffer, &cbChangesOnResize);


  for( auto& mesh : m_model->m_meshes )
  {
    mesh.mptr_resource = m_resourceView;//   m_renderTargetAndShaderResource->m_shaderResource;
  }

  //p_ImmediateContext->UpdateSubresource(p_CBChangeOnResize, 0, NULL, &projMatrixData, 0, 0);
  //deviceContext.getInterface()->UpdateSubresource(p_CBChangeOnResize, 0, NULL, &projMatrixData, 0, 0);
  return S_OK;
}


HRESULT
appGraphics::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
  bool isSuccessful = m_window->init(WndProcRedirect,
                                     hInstance,
                                     " Graphics window ");

  if( isSuccessful )
  {
    return S_OK;
  }
  else
  {
    return S_FALSE;
  }

}

void 
appGraphics::setShaderAndBuffers()
{
  enDeviceContext& deviceContext = enDeviceContext::getInstance();
  deviceContext.VSSetShader(*m_vertexShader);
  deviceContext.VSSetConstantBuffer(*s_ViewMatrixBuffer, s_ViewMatrixBuffer->getIndex());

  deviceContext.VSSetConstantBuffer(*s_ProjectionMatrixBuffer,
                                    s_ProjectionMatrixBuffer->getIndex());

  deviceContext.VSSetConstantBuffer(*m_worldMatrix,
                                    m_worldMatrix->getIndex());

  deviceContext.PSSetShader(*m_pixelShader);

  deviceContext.PSSetConstantBuffers(*m_worldMatrix, m_worldMatrix->getIndex());
  deviceContext.PSSetShaderResources(m_resourceView.get(), 1);
  deviceContext.PSSetSampler(*m_sampler);
}

void 
appGraphics::drawWithSeletecRenderTarget(size_t renderTargetIndex)
{
  enRenderTargetView* ptr_renderTarget = nullptr;
  if( renderTargetIndex == 0 )
  {
    ptr_renderTarget = m_renderTargetView.get();
  }
  else if(renderTargetIndex == 1  ){
    ptr_renderTarget = &m_renderTargetAndShaderResource->m_renderView;
  }

  enDeviceContext& deviceContext = enDeviceContext::getInstance();

  deviceContext.OMSetRenderTargets(ptr_renderTarget, m_depthStencilView.get());
  
  m_model->DrawMeshes(m_ConstBufferContainer);

}

void 
appGraphics::switchCamera()
{
  BasePerspectiveCamera* ptr_camera = nullptr;
  if( s_useFreeCam )
  {
    ptr_camera = s_CameraManager->getFirstPersonCamera();
    s_useFreeCam = false;
  }

  else if( !s_useFreeCam )
  {
    ptr_camera = s_CameraManager->getFreeCamera();
    s_useFreeCam = true;
  }

  enDeviceContext& deviceContext = enDeviceContext::getInstance();

  viewMatrix cbNeverChanges;
  cbNeverChanges.mView = glm::transpose(ptr_camera->getView());
  deviceContext.UpdateSubresource(s_ViewMatrixBuffer, &cbNeverChanges);


  projectionMatrix cbChangesOnResize;
  cbChangesOnResize.mProjection = glm::transpose(ptr_camera->getProjection());
  deviceContext.UpdateSubresource(s_ProjectionMatrixBuffer, &cbChangesOnResize);

}

void
appGraphics::Render()
{
  static float t = 0.0f;
  enDeviceContext& deviceContext = enDeviceContext::getInstance();
  {
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if( dwTimeStart == 0 )
      dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;
  }

  // Modify the color
  m_MeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
  m_MeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
  m_MeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

  //
  // Clear the back buffer
  //
  //float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
  static sColorf ClearColor{ 0.40f,0.50f,1.00f,1.0f };

  deviceContext.ClearRenderTargetView(m_renderTargetAndShaderResource->m_renderView, &ClearColor);
  //
  // Clear the depth buffer to 1.0 (max depth)
  //
  deviceContext.ClearDepthStencilView(*m_depthStencilView);

  this->setShaderAndBuffers();

  this->drawWithSeletecRenderTarget(1);
/*+++++++++++++++++++++++++++++++++++++++++++++*/

  this->switchCamera();
  deviceContext.ClearRenderTargetView(*m_renderTargetView);
  //
  // Clear the depth buffer to 1.0 (max depth)
  //
  deviceContext.ClearDepthStencilView(*m_depthStencilView);

  this->setShaderAndBuffers();

  this->drawWithSeletecRenderTarget(0);

/*+++++++++++++++++++++++++++++++++++++++++++++*/

  this->switchCamera();
  deviceContext.ClearRenderTargetView(m_renderTargetAndShaderResource->m_renderView);

  // Clear the depth buffer to 1.0 (max depth)

  deviceContext.ClearDepthStencilView(*m_depthStencilView);
  this->setShaderAndBuffers();

  this->drawWithSeletecRenderTarget(1);


  this->drawWithSeletecRenderTarget(0);
  //this->setShaderAndBuffers();

  //this->drawWithSeletecRenderTarget(0);

  ConstBufferWorldColor cb;
  cb.vMeshColor = m_MeshColor;
  cb.mWorld = glm::transpose(m_World);

  //deviceContext.getInterface()->UpdateSubresource(p_CBChangesEveryFrame, 0, NULL, &cb, 0, 0);
  deviceContext.UpdateSubresource(m_worldMatrix.get(), &cb);
 // deviceContext.DrawIndexed(36);



 //deviceContext.getInterface()->DrawIndexed(36, 0, 0);

  //m_model->DrawMeshes(m_ConstBufferContainer);
  //m_World = glm::identity<glm::mat4x4>();



  m_gui->beginFrame("camera view");
  m_gui->addImage(*m_renderTargetAndShaderResource->m_shaderResource);
  m_gui->addButton("switch Cam", s_useFreeCam);

  m_gui->endFrame();
  // Present our back buffer to our front buffer
  //
  //p_SwapChain->Present(0, 0);
  m_swapchain->Present();
}

void
appGraphics::Update()
{
  uint32c heightBeforeUpdate = m_window->getHeight();
  uint32c widthBeforeUpdate = m_window->getWidth();
  m_window->update();

  if( heightBeforeUpdate != m_window->getHeight() ||
     widthBeforeUpdate != m_window->getWidth() )
  {
    uint32c newWidth = m_window->getWidth();
    uint32c newHeight = m_window->getHeight();

    enVector2 const NewSize(newWidth, newHeight);

    m_swapchain->ResizeSwapChain(*m_window,
                                 *m_renderTargetView,
                                 *m_depthStencilView,
                                 NewSize,
                                 *m_viewport);

    enDeviceContext& deviceContext = enDeviceContext::getInstance();


    s_CameraManager->updateCameras(newWidth, newHeight);

    enPerspectiveFreeCamera* ptr_freeCam = s_CameraManager->getFreeCamera();
    viewMatrix ViewMatrixData;
    ViewMatrixData.mView = ptr_freeCam->getView();
    helper::arrangeForApi(ViewMatrixData.mView);
    deviceContext.UpdateSubresource(s_ViewMatrixBuffer, &ViewMatrixData.mView);


    projectionMatrix  projMatrixData;
    projMatrixData.mProjection = ptr_freeCam->getProjection();
    helper::arrangeForApi(projMatrixData.mProjection);
    deviceContext.UpdateSubresource(s_ProjectionMatrixBuffer, &projMatrixData.mProjection);
  }

}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT
appGraphics::WndProcRedirect(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;

  if( ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam) )
    return true;

  enDeviceContext& deviceContext = enDeviceContext::getInstance();
  if( s_initIsFinish == false )
  {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  BasePerspectiveCamera* cameraPtr = nullptr;
  if( s_useFreeCam )
  {
    cameraPtr = s_CameraManager->getFirstPersonCamera();
  }
  else
  {
    cameraPtr = s_CameraManager->getFreeCamera();
  }


  switch( message )
  {
    case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    case WM_KEYDOWN:
    {

      if( wParam == (WPARAM)'1' )
      {
        if( s_useFreeCam )
        {
          s_useFreeCam = false;
          std::cout << "false\n";
        }
        else
        {
          s_useFreeCam = true;
          std::cout << "true\n";
        }
      }

      if( wParam == (WPARAM)'W' )
      {
        if( auto* freeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          freeCam->TranslateRelative(0.0f, 0.0f, 1.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->TranslateRelative(0.0f, 0.0f, 1.0f);
        }

      }

      if( wParam == (WPARAM)'S' )
      {
        //s_FirstPersonCamera.TranslateRelative(0.0f, 0.0f, -1.0f);

        if( auto* freeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          freeCam->TranslateRelative(0.0f, 0.0f, -1.0f);
        }

        if( auto* firstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          firstPersonCam->TranslateRelative(0.0f, 0.0f, -1.0f);
        }
      }
      if( wParam == (WPARAM)'D' )

      {
        if( auto* FreeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          FreeCam->TranslateRelative(-1.0f, 0.0f, 0.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->TranslateRelative(-1.0f, 0.0f, 0.0f);
        }
        //s_FirstPersonCamera.TranslateRelative(-1.0f, 0.0f, 0.0f);
      }
      if( wParam == (WPARAM)'A' )
      {
       // s_FirstPersonCamera.TranslateRelative(1.0f, 0.0f, 0.0f);

        if( auto* freeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          freeCam->TranslateRelative(1.0f, 0.0f, 0.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->TranslateRelative(1.0f, 0.0f, 0.0f);
        }
      }
      if( wParam == static_cast<WPARAM>('E') )
      {
        //s_FirstPersonCamera.TranslateRelative(0.0f, 1.0f, 0.0f);

        if( auto* freeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          freeCam->TranslateRelative(0.0f, 1.0f, 0.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->TranslateRelative(0.0f, 1.0f, 0.0f);
        }
      }

      if( wParam == static_cast<WPARAM>('Q') )
      {
        //s_FirstPersonCamera.TranslateRelative(0.0f, -1.0f, 0.0f);

        if( auto* freeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          freeCam->TranslateRelative(0.0f, -1.0f, 0.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->TranslateRelative(0.0f, -1.0f, 0.0f);
        }
      }

      if( wParam == VK_RIGHT )
      {

        if( auto* FreeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          FreeCam->rotateInYaw(-10.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->rotateInYaw(-10.0f);
        }
      }

      if( wParam == VK_LEFT )
      {
        if( auto* FreeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          FreeCam->rotateInYaw(10.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->rotateInYaw(10.0f);
        }
        ///s_FirstPersonCamera.rotateInYaw(10.0f);
      }

      if( wParam == VK_UP )
      {

        if( auto* freeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          freeCam->rotateInPitch(-10.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->rotateInPitch(-10.0f);
        }
        //s_FirstPersonCamera.rotateInPitch(-10.0f);
      }

      if( wParam == VK_DOWN )
      {
        //s_FirstPersonCamera.rotateInPitch(10.0f);

        if( auto* FreeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          FreeCam->rotateInPitch(10.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->rotateInPitch(10.0f);
        }
      }

      if( wParam == static_cast<WPARAM>('I') )
      {
        //s_FirstPersonCamera.rotateInRoll(10.0f);

        if( auto* freeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          freeCam->rotateInRoll(10.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->rotateInRoll(10.0f);
        }
      }

      if( wParam == static_cast<WPARAM>('O') )
      {
        //s_FirstPersonCamera.rotateInRoll(-10.0f);

        if( auto* freeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          freeCam->rotateInRoll(-10.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->rotateInRoll(-10.0f);
        }
      }


      viewMatrix cbNeverChanges;
      cbNeverChanges.mView = glm::transpose(cameraPtr->getView());
      deviceContext.UpdateSubresource(s_ViewMatrixBuffer, &cbNeverChanges);


      projectionMatrix cbChangesOnResize;
      cbChangesOnResize.mProjection = glm::transpose(cameraPtr->getProjection());
      deviceContext.UpdateSubresource(s_ProjectionMatrixBuffer, &cbChangesOnResize);
    }
    break;

    case(WM_MOUSEMOVE):
    {
      if( wParam & MK_SHIFT )
      {
        WINDOWINFO windInfo;
        windInfo.cbSize = sizeof(WINDOWINFO);
        GetWindowInfo(hWnd, &windInfo);

        const int32_t WindowLocationX = (windInfo.rcWindow.right - windInfo.rcWindow.left);
        const int32_t WindowLocationY = (windInfo.rcWindow.bottom - windInfo.rcWindow.top);

        POINT  currentPos;
        GetCursorPos(&currentPos);

        SetCursorPos(WindowLocationX * .5f,
                     WindowLocationY * .5f);

        enVector3 mouseDir(currentPos.x - (WindowLocationX * .5f),
                           currentPos.y - (WindowLocationY * .5f),
                           0.0f);

        mouseDir.x = -mouseDir.x;

        if( auto* FirstPersonCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          FirstPersonCam->rotateVector(mouseDir);
        }

        if( auto* freeCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          freeCam->rotateVector(mouseDir);
        }

        viewMatrix cbNeverChanges;
        cbNeverChanges.mView = cameraPtr->getView();
        helper::arrangeForApi(cbNeverChanges.mView);
        deviceContext.UpdateSubresource(s_ViewMatrixBuffer, &cbNeverChanges);

        projectionMatrix cbChangesOnResize;
        cbChangesOnResize.mProjection = cameraPtr->getProjection();
        helper::arrangeForApi(cbChangesOnResize.mProjection);
        deviceContext.UpdateSubresource(s_ProjectionMatrixBuffer, &cbChangesOnResize);
      }
    }
    break;

    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
}