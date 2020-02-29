//--------------------------------------------------------------------------------------
// my includes 
//--------------------------------------------------------------------------------------
#include "..\include\appGraphics.h"
#include "helperFucs.h"
#include "Resource.h"

#include "glm/gtc/matrix_transform.hpp"

#include "DirectXTK/include/DDSTextureLoader.h"
#include "DirectXTK/include/WICTextureLoader.h"

#include "../include/enPerspectiveFreeCamera.h"
#include "../include/enFirstPersonCamera.h"

#include "enCameraManager.h"
#include "enDevice.h"
#include "enDeviceContext.h"
#include "enShaderResourceView.h"

//--------------------------------------------------------------------------------------
// standard  includes 
//--------------------------------------------------------------------------------------
#include <iostream>
namespace dx = DirectX;
using std::make_unique;


enPerspectiveFreeCamera* appGraphics::myCamera = nullptr;
enFirstPersonCamera* appGraphics::myFirstPersonCamera = nullptr;
enCameraManager* appGraphics::myCameraManager = nullptr;

//ID3D11DeviceContext* appGraphics::p_ImmediateContext = nullptr;
//ID3D11Buffer* appGraphics::p_CBNeverChanges = nullptr;
//ID3D11Buffer* appGraphics::p_CBChangeOnResize = nullptr;
bool appGraphics::my_initIsFinish = false;

enConstBuffer* appGraphics::myViewMatrixBuffer = nullptr;
enConstBuffer* appGraphics::myProjectionMatrixBuffer = nullptr;


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

  if( !EN_SUCCESS(initApi()) )
  {
    this->destroy();
    return false;
  }

  if( InitForRender() == S_FALSE )
  {
    this->destroy();
    return false;
  }

  my_initIsFinish = true;

  return true;
}

int
appGraphics::run()
{
  // Main message loop
  MSG msg = { 0 };
  while( WM_QUIT != msg.message )
  {
    if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      Render();
    }
  }

  return (int)msg.wParam;
}

void
appGraphics::destroy()
{
  CoUninitialize();


  DELETE_PTR(myCamera);
  DELETE_PTR(myFirstPersonCamera);
  DELETE_PTR(myCameraManager);
  DELETE_PTR(myViewMatrixBuffer);
  DELETE_PTR(myProjectionMatrixBuffer);

  //RELEASE_DX_PTR(p_SamplerLinear);
  //RELEASE_DX_PTR(p_TextureRV);
  //RELEASE_DX_PTR(p_CBChangeOnResize);
  //RELEASE_DX_PTR(p_CBChangesEveryFrame);

  //RELEASE_DX_PTR(p_SwapChain);


  enDevice::ShutDown();
  enDeviceContext::ShutDown();
  //if( p_d3dDevice ) p_d3dDevice->Release();
  /*
    if( p_CBChangeOnResize ) p_CBChangeOnResize->Release();
    if( p_VertexLayout ) p_VertexLayout->Release();
    if( p_IndexBuffer ) p_IndexBuffer->Release();
    if( p_VertexBuffer ) p_VertexBuffer->Release();
    if( p_CBChangesEveryFrame ) p_CBChangesEveryFrame->Release();
    if( p_CBNeverChanges ) p_CBNeverChanges->Release();
    if( p_TextureRV ) p_TextureRV->Release();
    if( p_SamplerLinear ) p_SamplerLinear->Release();
    if( p_VertexShader ) p_VertexShader->Release();
    if( p_PixelShader ) p_PixelShader->Release();
    if( p_DepthStencil ) p_DepthStencil->Release();
    if( p_SwapChain ) p_SwapChain->Release();
    if( p_DepthStencilView ) p_DepthStencilView->Release();
    if( p_RenderTargetView ) p_RenderTargetView->Release();
    if( p_SwapChain ) p_SwapChain->Release();
    if( p_ImmediateContext ) p_ImmediateContext->Release();
  */
}


bool
appGraphics::InitStatics()
{
  try
  {
    myCamera = new enPerspectiveFreeCamera();
    myFirstPersonCamera = new enFirstPersonCamera();
    myCameraManager = new enCameraManager();
    myViewMatrixBuffer = new enConstBuffer();
    myProjectionMatrixBuffer = new enConstBuffer();
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

  enErrorCode checkForError = helper::CreateDeviceAndSwapchain(*mySwapchain,
                                                               *myWindow,
                                                               m_hardwareInfo);

  return checkForError;
}

bool
appGraphics::createMyClasses()
{
  try
  {
    myVertexShader = make_unique<enVertexShader>();
    myPixelShader = make_unique<enPixelShader>();

    myVertexBuffer = make_unique<enVertexBuffer>();
    myIndexBuffer = make_unique<enIndexBuffer>();

    myDepthStencil = make_unique<enTexture2D>();
    myRenderTargetView = make_unique<enRenderTargetView>();
    myInputLayout = make_unique<enInputLayout>();

    myDepthStencilView = make_unique<enDepthStencilView>();
    myViewPort = make_unique<enViewport>();

    myWorldMatrix = make_unique<enConstBuffer>();

    myResourceView = make_unique<enShaderResourceView>();
    mySampler = make_unique<enSampler>();

    mySwapchain = make_unique<enSwapChain>();
    myWindow = make_unique<enWindow>();
  }
  catch( const std::exception & e )
  {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

HRESULT
appGraphics::InitForRender()
{
  enDevice& device = enDevice::getInstance();
  enDeviceContext& deviceContext = enDeviceContext::getInstance();
  bool isSuccessful = false;

  HRESULT hr = S_FALSE;

  enVector2 windowSize = helper::getWindowSize(*myWindow);

  // Create a render target view
  //ID3D11Texture2D* pBackBuffer = NULL;
  //hr = p_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&myRenderTargetView->m_targets[0].m_interface);
  if( FAILED(hr) )
    return hr;

  mySwapchain->ReciveBuckBuffer(*myRenderTargetView);

  isSuccessful = device.CreateRenderTargetView(*myRenderTargetView, 0);
  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }

  if( FAILED(hr) )
    return hr;

  sTextureDescriptor descriptoDepth;
  descriptoDepth.texWidth = windowSize.x ;
  descriptoDepth.texHeight =  windowSize.y;
  descriptoDepth.CpuAccess = 0;
  descriptoDepth.texFormat = static_cast<int>(enFormats::depthStencil_format);
  descriptoDepth.Usage = enBufferUse::Default;
  descriptoDepth.BindFlags = enBufferBind::DepthStencil;
  descriptoDepth.arraySize = 1;

  isSuccessful = device.CreateTexture2D(descriptoDepth, myDepthStencilView->m_texture);
  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }

  sDepthStencilDescriptor DepthStencilDesc;
  DepthStencilDesc.Format = descriptoDepth.texFormat;
  DepthStencilDesc.Dimension = DepthStencilFormat::two_dimention;
  DepthStencilDesc.Mip = 0;

  myDepthStencilView->m_desc = DepthStencilDesc;
  isSuccessful = device.CreateDepthStencilView(*myDepthStencilView);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }

  deviceContext.OMSetRenderTargets(myRenderTargetView.get(),
                                   *myDepthStencilView);
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

  myViewPort->init(viewDescriptor);

  deviceContext.RSSetViewports(myViewPort.get());

  enErrorCode errorCode = myVertexShader->compileShaderFromFile("GraphcisFramework.fx",
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
  //hr = device.getInterface()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &myVertexShader->m_interface);
  isSuccessful = device.CreateVertexShader(*myVertexShader);
  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }

  myInputLayout->ReadShaderData(*myVertexShader);


  isSuccessful = device.CreateInputLayout(*myInputLayout, *myVertexShader);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return S_FALSE;
  }
  // Set the input layout
  //p_ImmediateContext->IASetInputLayout(myInputLayout->getInterface());
  deviceContext.IASetInputLayout(*myInputLayout);

  isSuccessful = myPixelShader->compileShaderFromFile("GraphcisFramework.fx",
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
  isSuccessful = device.CreatePixelShader(*myPixelShader);
  //hr = device.getInterface()->CreatePixelShader( myPixelShader->m_desc.m_infoOfShader->GetBufferPointer(),
  //                                               myPixelShader->m_desc.m_infoOfShader->GetBufferSize(),
  //                                              NULL,
  //                                              &myPixelShader->m_interface);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }

  // Create vertex buffer
  SimpleVertex vertices[] =
  {
      { glm::vec4(-1.0f, 1.0f, -1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec4(1.0f, 1.0f, -1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec4(1.0f, 1.0f, 1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec4(-1.0f, 1.0f, 1.0f,1.0f), glm::vec2(0.0f, 1.0f) },

      { glm::vec4(-1.0f, -1.0f, -1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec4(1.0f, -1.0f, -1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec4(1.0f, -1.0f, 1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec4(-1.0f, -1.0f, 1.0f,1.0f), glm::vec2(0.0f, 1.0f) },

      { glm::vec4(-1.0f, -1.0f, 1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec4(-1.0f, -1.0f, -1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec4(-1.0f, 1.0f, -1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec4(-1.0f, 1.0f, 1.0f,1.0f), glm::vec2(0.0f, 1.0f) },

      { glm::vec4(1.0f, -1.0f, 1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec4(1.0f, -1.0f, -1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec4(1.0f, 1.0f, -1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec4(1.0f, 1.0f, 1.0f,1.0f), glm::vec2(0.0f, 1.0f) },

      { glm::vec4(-1.0f, -1.0f, -1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec4(1.0f, -1.0f, -1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec4(1.0f, 1.0f, -1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec4(-1.0f, 1.0f, -1.0f,1.0f), glm::vec2(0.0f, 1.0f) },

      { glm::vec4(-1.0f, -1.0f, 1.0f,1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec4(1.0f, -1.0f, 1.0f,1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec4(1.0f, 1.0f, 1.0f,1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec4(-1.0f, 1.0f, 1.0f,1.0f), glm::vec2(0.0f, 1.0f) },
  };

  D3D11_BUFFER_DESC bd;
  std::memset(&bd, 0, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(SimpleVertex) * 24;
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;

  sBufferDesc VertexBuffdescriptor;
  VertexBuffdescriptor.cpuAccess = 0;
  VertexBuffdescriptor.bindFlags = enBufferBind::Vertex;
  VertexBuffdescriptor.elementCount = ARRAYSIZE(vertices);
  VertexBuffdescriptor.stride = sizeof(SimpleVertex);
  VertexBuffdescriptor.ptr_data = vertices;


  myVertexBuffer->init(VertexBuffdescriptor);

  isSuccessful = device.CreateVertexBuffer(*myVertexBuffer);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }

  // Set vertex buffer
  UINT stride = sizeof(SimpleVertex);
  UINT offset = 0;

  deviceContext.IASetVertexBuffers(myVertexBuffer.get(), 1);

  // Create index buffer
  // Create vertex buffer
  WORD indices[] =
  {
      3,1,0,
      2,1,3,

      6,4,5,
      7,4,6,

      11,9,8,
      10,9,11,

      14,12,13,
      15,12,14,

      19,17,16,
      18,17,19,

      22,20,21,
      23,20,22
  };


  sBufferDesc IndexBufferDescriptor;
  IndexBufferDescriptor.bindFlags = enBufferBind::Index;
  IndexBufferDescriptor.cpuAccess = 0;
  IndexBufferDescriptor.elementCount = ARRAYSIZE(indices);
  IndexBufferDescriptor.stride = sizeof(WORD);
  IndexBufferDescriptor.ptr_data = indices;

  myIndexBuffer->init(IndexBufferDescriptor);

  device.CreateIndexBuffer(*myIndexBuffer);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }

  deviceContext.IASetIndexBuffer(*myIndexBuffer, enFormats::uR16);

  deviceContext.IASetPrimitiveTopology(static_cast<int>(enTopology::TriList));

  // Set index buffer
  //p_ImmediateContext->IASetIndexBuffer(p_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
  //deviceContext.getInterface()->IASetIndexBuffer(p_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
  // Set primitive topology
  //p_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//deviceContext.getInterface()-> IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>( ); enTopology::TriList)


  // Create the constant buffers
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(CBNeverChanges);
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bd.CPUAccessFlags = 0;

  sBufferDesc viewBuffer;
  viewBuffer.bindFlags = enBufferBind::Const;
  viewBuffer.stride = sizeof(CBNeverChanges);
  viewBuffer.elementCount = 1;
  viewBuffer.cpuAccess = 0;
  viewBuffer.index = 0;

  myViewMatrixBuffer->init(viewBuffer);

  device.CreateConstBuffer(*myViewMatrixBuffer);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }


  bd.ByteWidth = sizeof(CBChangeOnResize);

  sBufferDesc ProjectionDescriptor;
  ProjectionDescriptor.bindFlags = enBufferBind::Const;
  ProjectionDescriptor.elementCount = 1;
  ProjectionDescriptor.stride = sizeof(CBChangeOnResize);
  ProjectionDescriptor.index = 1;

  myProjectionMatrixBuffer->init(ProjectionDescriptor);


  isSuccessful = device.CreateConstBuffer(*myProjectionMatrixBuffer);


  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }

  bd.ByteWidth = sizeof(CBChangesEveryFrame);
  sBufferDesc worldMatrixDescriptor;
  worldMatrixDescriptor.elementCount = 1;
  worldMatrixDescriptor.stride = sizeof(CBChangesEveryFrame);
  worldMatrixDescriptor.bindFlags = enBufferBind::Const;
  worldMatrixDescriptor.usage = enBufferUse::Default;
  worldMatrixDescriptor.index = 2;

  myWorldMatrix->init(worldMatrixDescriptor);

  isSuccessful = device.CreateConstBuffer(*myWorldMatrix);


  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation)
      return S_FALSE;
  }

  // Load the Texture
  myResourceView->init();

  isSuccessful = device.CreateShaderResourceFromFile(*myResourceView,
                                                     "neon light.jpg");

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

    mySampler->init(samplerDescriptor);
    isSuccessful = device.CreateSamplerState(*mySampler);
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
  descriptorCamera.width =  windowSize.x;

  myCamera->init(descriptorCamera);


  sFirstPersonCameraDesc cameraDescriptor;
  myFirstPersonCamera->init(cameraDescriptor);

  myCameraManager->addCamera(myCamera);

  myCameraManager->addCamera(myFirstPersonCamera);

  // Initialize the world matrices
  g_World = glm::identity<glm::mat4x4>();

  // Initialize the view matrix
  glm::vec3 Eye(0.0f, 3.0f, -6.0f);
  glm::vec3 At(0.0f, 0.0f, -1.0f);
  glm::vec3 Up(0.0f, 1.0f, 0.0f);

  CBNeverChanges cbNeverChanges;
  cbNeverChanges.mView = glm::transpose(myCamera->getView() /* myCamera.getView()*/);


  //p_ImmediateContext->UpdateSubresource(p_CBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);
  deviceContext.getInterface()->UpdateSubresource(myViewMatrixBuffer->getInterface(), 0, NULL, &cbNeverChanges, 0, 0);

  // Initialize the projection matrix
  g_Projection = glm::perspectiveFovLH(glm::quarter_pi<float>(),
                                       windowSize.x,
                                       windowSize.y,
                                       0.01f,
                                       100.0f);

  CBChangeOnResize cbChangesOnResize;
  cbChangesOnResize.mProjection = glm::transpose(myCamera->getProjection() /*myCamera.getProjection()*/);

  deviceContext.UpdateSubresource(myProjectionMatrixBuffer, &cbChangesOnResize);

  //p_ImmediateContext->UpdateSubresource(p_CBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
  //deviceContext.getInterface()->UpdateSubresource(p_CBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
  return S_OK;
}


HRESULT
appGraphics::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
  bool isSuccessful = myWindow->init(WndProcRedirect,
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

 //// Register class
 // WNDCLASSEX wcex;
 // wcex.cbSize = sizeof(WNDCLASSEX);
 // wcex.style = CS_HREDRAW | CS_VREDRAW;
 // wcex.lpfnWndProc = WndProcRedirect;
 // wcex.cbClsExtra = 0;
 // wcex.cbWndExtra = 0;
 // wcex.hInstance = hInstance;
 // wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
 // wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
 // wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
 // wcex.lpszMenuName = NULL;
 // wcex.lpszClassName = L"TutorialWindowClass";
 // wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);

 // if( !RegisterClassEx(&wcex) )
 //   return E_FAIL;

 // // Create window
 // //g_hInst 
 // m_moduleInstance = hInstance;

 // RECT rc = { 0, 0, 640, 480 };

 // AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

 // g_hWnd = CreateWindow(L"TutorialWindowClass",
 //                       L"Direct3D 11 Tutorial 7",
 //                       WS_OVERLAPPEDWINDOW,
 //                       100,
 //                       100,
 //                       rc.right - rc.left,
 //                       rc.bottom - rc.top,
 //                       NULL,
 //                       NULL,
 //                       hInstance,
 //                       NULL);


 // if( !g_hWnd )
 //   return E_FAIL;

 // if( g_hWnd != INVALID_HANDLE_VALUE )
 // {
 //   ShowWindow(g_hWnd, nCmdShow);
 //   UpdateWindow(g_hWnd);
 // }

    return S_OK;
}

void
appGraphics::Render()
{
  // Update our time
  static float t = 0.0f;
  enDeviceContext& deviceContext = enDeviceContext::getInstance();
  if( g_driverType == D3D_DRIVER_TYPE_REFERENCE )
  {
    t += (float)glm::pi<float>() * 0.0125f;
  }
  else
  {
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if( dwTimeStart == 0 )
      dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;
  }

  // Modify the color
  g_MeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
  g_MeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
  g_MeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

  //
  // Clear the back buffer
  //
  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
  deviceContext.ClearRenderTargetView(*myRenderTargetView);

  //
  // Clear the depth buffer to 1.0 (max depth)
  //
  deviceContext.ClearDepthStencilView(*myDepthStencilView);

  //
  // Render the cube
  //
  deviceContext.getInterface()->VSSetShader(myVertexShader->getInterface(), NULL, 0);
  deviceContext.getInterface()->VSSetConstantBuffers(0, 1, myViewMatrixBuffer->getInterfaceRef());

  deviceContext.VSSetConstantBuffer(*myProjectionMatrixBuffer,
                                    myProjectionMatrixBuffer->getIndex());

  deviceContext.VSSetConstantBuffer(*myWorldMatrix,
                                    myWorldMatrix->getIndex());

  deviceContext.PSSetShader(*myPixelShader);

  deviceContext.PSSetConstantBuffers(*myWorldMatrix, myWorldMatrix->getIndex());
  deviceContext.PSSetShaderResources(myResourceView.get(), 1);
  deviceContext.PSSetSampler(*mySampler);

  CBChangesEveryFrame cb;
  cb.vMeshColor = g_MeshColor;
  cb.mWorld = glm::transpose(g_World);

  //deviceContext.getInterface()->UpdateSubresource(p_CBChangesEveryFrame, 0, NULL, &cb, 0, 0);
  deviceContext.UpdateSubresource(myWorldMatrix.get(), &cb);


  deviceContext.DrawIndexed(36);
  //deviceContext.getInterface()->DrawIndexed(36, 0, 0);

  g_World = glm::identity<glm::mat4x4>();

  // Present our back buffer to our front buffer
  //
  //p_SwapChain->Present(0, 0);
  mySwapchain->Present();
}

LRESULT
appGraphics::WndProcRedirect(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;
  static bool useFreeCamera = false;

  enDeviceContext& deviceContext = enDeviceContext::getInstance();
  if( my_initIsFinish == false )
  {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  BasePerspectiveCamera* cameraPtr = nullptr;
  if( useFreeCamera )
  {
    cameraPtr = myCameraManager->getFirstPersonCamera();
  }
  else
  {
    cameraPtr = myCameraManager->getFreeCamera();
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
        if( useFreeCamera )
        {
          useFreeCamera = false;
          std::cout << "false\n";
        }
        else
        {
          useFreeCamera = true;
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
        //myFirstPersonCamera.TranslateRelative(0.0f, 0.0f, -1.0f);

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
        //myFirstPersonCamera.TranslateRelative(-1.0f, 0.0f, 0.0f);
      }
      if( wParam == (WPARAM)'A' )
      {
       // myFirstPersonCamera.TranslateRelative(1.0f, 0.0f, 0.0f);

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
        //myFirstPersonCamera.TranslateRelative(0.0f, 1.0f, 0.0f);

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
        //myFirstPersonCamera.TranslateRelative(0.0f, -1.0f, 0.0f);

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
    //        myFirstPersonCamera.rotateInYaw(-10.0f);

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
        ///myFirstPersonCamera.rotateInYaw(10.0f);
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
        //myFirstPersonCamera.rotateInPitch(-10.0f);
      }

      if( wParam == VK_DOWN )
      {
        //myFirstPersonCamera.rotateInPitch(10.0f);

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
        //myFirstPersonCamera.rotateInRoll(10.0f);

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
        //myFirstPersonCamera.rotateInRoll(-10.0f);

        if( auto* freeCam = dynamic_cast<enPerspectiveFreeCamera*>(cameraPtr) )
        {
          freeCam->rotateInRoll(-10.0f);
        }

        if( auto* FirstPersonCam = dynamic_cast<enFirstPersonCamera*>(cameraPtr) )
        {
          FirstPersonCam->rotateInRoll(-10.0f);
        }
      }


      CBNeverChanges cbNeverChanges;
      cbNeverChanges.mView = glm::transpose(cameraPtr->getView());
      //deviceContext.getInterface()->UpdateSubresource(myViewMatrixBuffer->getInterface(), 0, NULL, &cbNeverChanges, 0, 0);
      deviceContext.UpdateSubresource(myViewMatrixBuffer, &cbNeverChanges);


      CBChangeOnResize cbChangesOnResize;
      cbChangesOnResize.mProjection = glm::transpose(cameraPtr->getProjection());
      deviceContext.UpdateSubresource(myProjectionMatrixBuffer, &cbChangesOnResize);
      //deviceContext.getInterface()->UpdateSubresource(myProjectionMatrixBuffer, 0, NULL, &cbChangesOnResize, 0, 0);
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

        CBNeverChanges cbNeverChanges;
        cbNeverChanges.mView = glm::transpose(cameraPtr->getView());
        deviceContext.getInterface()->UpdateSubresource(myViewMatrixBuffer->getInterface(), 0, NULL, &cbNeverChanges, 0, 0);


        CBChangeOnResize cbChangesOnResize;
        cbChangesOnResize.mProjection = glm::transpose(cameraPtr->getProjection());
        deviceContext.getInterface()->UpdateSubresource(myProjectionMatrixBuffer->getInterface(), 0, NULL, &cbChangesOnResize, 0, 0);
      }
    }
    break;

    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
}
