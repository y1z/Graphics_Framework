#include "..\include\appGraphcis.h"

#include "Resource.h"
//--------------------------------------------------------------------------------------
// standard  includes 
//--------------------------------------------------------------------------------------
#include <iostream>

//--------------------------------------------------------------------------------------
// my includes 
//--------------------------------------------------------------------------------------
#include "helperFucs.h"

#include "glm/gtc/matrix_transform.hpp"
#include "DirectXTK/include/DDSTextureLoader.h"
#include "DirectXTK/include/WICTextureLoader.h"
#include "../include/enPerspectiveFreeCamera.h"
#include "../include/enFirstPersonCamera.h"
#include"enCameraManager.h"


namespace dx = DirectX;


enPerspectiveFreeCamera* appGraphcis::my_camera = nullptr;
enFirstPersonCamera* appGraphcis::my_firstPersonCamera = nullptr;
enCameraManager* appGraphcis::my_manager = nullptr;

ID3D11DeviceContext* appGraphcis::p_ImmediateContext = nullptr;
ID3D11Buffer* appGraphcis::p_CBNeverChanges = nullptr;
ID3D11Buffer* appGraphcis::p_CBChangeOnResize = nullptr;
bool appGraphcis::my_initIsFinish = false;

appGraphcis::appGraphcis()
{}

bool
appGraphcis::init()
{
  HMODULE Hmodule;

  BOOL checkIfSucceeded = GetModuleHandleEx(0x00, NULL, &Hmodule);

  if( checkIfSucceeded == FALSE )
    return false;

  if( !InitStatics() )
    return false;

  if( FAILED(InitWindow(Hmodule, SW_SHOWDEFAULT)) )
    return false;

  /*Initializes the COM library
  this is so I can load images for directX */
  if( FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)) )
  {
    return false;
  }

  if( FAILED(InitDevice()) )
  {
    this->destroy();
    return false;
  }

  my_initIsFinish = true;

  return true;
}

int
appGraphcis::run()
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

  this->destroy();
  return (int)msg.wParam;
}

void
appGraphcis::destroy()
{
  CoUninitialize();

  if( p_ImmediateContext )
  {
    p_ImmediateContext->ClearState();
    //p_ImmediateContext = nullptr;
  }

  DELETE_PTR(my_camera)
    DELETE_PTR(my_firstPersonCamera)
    DELETE_PTR(my_manager)

      //RELEASE_DX_PTR(p_ImmediateContext)
    RELEASE_DX_PTR(p_SamplerLinear)
    RELEASE_DX_PTR(p_TextureRV)
    RELEASE_DX_PTR(p_CBNeverChanges)
    RELEASE_DX_PTR(p_CBChangeOnResize)
    RELEASE_DX_PTR(p_CBChangesEveryFrame)
    RELEASE_DX_PTR(p_VertexBuffer)
    RELEASE_DX_PTR(p_IndexBuffer)
    RELEASE_DX_PTR(p_VertexLayout)
    RELEASE_DX_PTR(p_VertexShader)
    RELEASE_DX_PTR(p_PixelShader)
    RELEASE_DX_PTR(p_DepthStencil)
    RELEASE_DX_PTR(p_DepthStencilView)
    RELEASE_DX_PTR(p_RenderTargetView)
    RELEASE_DX_PTR(p_SwapChain)
    RELEASE_DX_PTR(p_ImmediateContext)
    RELEASE_DX_PTR(p_d3dDevice)

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

LRESULT
appGraphcis::WndProcRedirect(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;
  static bool useFreeCamera = false;

  if( my_initIsFinish == false )
  {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  BasePerspectiveCamera* cameraPtr = nullptr;
  if( useFreeCamera )
  {
    cameraPtr = my_manager->getFirstPersonCamera();
  }
  else
  {
    cameraPtr = my_manager->getFreeCamera();
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
        //my_firstPersonCamera.TranslateRelative(0.0f, 0.0f, -1.0f);

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
        //my_firstPersonCamera.TranslateRelative(-1.0f, 0.0f, 0.0f);
      }
      if( wParam == (WPARAM)'A' )
      {
       // my_firstPersonCamera.TranslateRelative(1.0f, 0.0f, 0.0f);

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
        //my_firstPersonCamera.TranslateRelative(0.0f, 1.0f, 0.0f);

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
        //my_firstPersonCamera.TranslateRelative(0.0f, -1.0f, 0.0f);

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
    //        my_firstPersonCamera.rotateInYaw(-10.0f);

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
        ///my_firstPersonCamera.rotateInYaw(10.0f);
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
        //my_firstPersonCamera.rotateInPitch(-10.0f);
      }

      if( wParam == VK_DOWN )
      {
        //my_firstPersonCamera.rotateInPitch(10.0f);

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
        //my_firstPersonCamera.rotateInRoll(10.0f);

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
        //my_firstPersonCamera.rotateInRoll(-10.0f);

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
      p_ImmediateContext->UpdateSubresource(p_CBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

      CBChangeOnResize cbChangesOnResize;
      cbChangesOnResize.mProjection = glm::transpose(cameraPtr->getProjection());
      p_ImmediateContext->UpdateSubresource(p_CBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
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

       // my_camera.rotateVector(mouseDir);

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
        p_ImmediateContext->UpdateSubresource(p_CBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

        CBChangeOnResize cbChangesOnResize;
        cbChangesOnResize.mProjection = glm::transpose(cameraPtr->getProjection());
        p_ImmediateContext->UpdateSubresource(p_CBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
      }
    }
    break;

    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
}

bool
appGraphcis::InitStatics()
{
  try
  {
    my_camera = new  enPerspectiveFreeCamera();
    my_firstPersonCamera = new enFirstPersonCamera();
    my_manager = new enCameraManager();
  }
  catch( const std::bad_alloc & allocError )
  {

    std::cout << allocError.what() << '\n';
    return false;
  }


  return true;
}

HRESULT
appGraphcis::InitDevice()
{
  HRESULT hr = S_OK;

  RECT rc;
  GetClientRect(g_hWnd, &rc);
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

  UINT createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_DRIVER_TYPE driverTypes[] =
  {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
  };
  UINT numDriverTypes = ARRAYSIZE(driverTypes);

  D3D_FEATURE_LEVEL featureLevels[] =
  {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };
  UINT numFeatureLevels = ARRAYSIZE(featureLevels);

  DXGI_SWAP_CHAIN_DESC sd;
  std::memset(&sd, 0, sizeof(sd));

  sd.BufferCount = 1;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = g_hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
  {
    g_driverType = driverTypes[driverTypeIndex];
    hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                       D3D11_SDK_VERSION, &sd, &p_SwapChain, &p_d3dDevice, &g_featureLevel, &p_ImmediateContext);
    if( SUCCEEDED(hr) )
      break;
  }
  if( FAILED(hr) )
    return hr;

  // Create a render target view
  ID3D11Texture2D* pBackBuffer = NULL;
  hr = p_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  if( FAILED(hr) )
    return hr;

  hr = p_d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &p_RenderTargetView);
  pBackBuffer->Release();
  if( FAILED(hr) )
    return hr;

  // Create depth stencil texture
  D3D11_TEXTURE2D_DESC descDepth;
  std::memset(&descDepth, 0, sizeof(descDepth));
  descDepth.Width = width;
  descDepth.Height = height;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D11_USAGE_DEFAULT;
  descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  descDepth.CPUAccessFlags = 0;
  descDepth.MiscFlags = 0;
  hr = p_d3dDevice->CreateTexture2D(&descDepth, NULL, &p_DepthStencil);
  if( FAILED(hr) )
    return hr;

  // Create the depth stencil view
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  SecureZeroMemory(&descDSV, sizeof(descDSV));
  descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;
  hr = p_d3dDevice->CreateDepthStencilView(p_DepthStencil, &descDSV, &p_DepthStencilView);
  if( FAILED(hr) )
    return hr;

  p_ImmediateContext->OMSetRenderTargets(1, &p_RenderTargetView, p_DepthStencilView);

  // Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)width;
  vp.Height = (FLOAT)height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  p_ImmediateContext->RSSetViewports(1, &vp);

  // Compile the vertex shader
  ID3DBlob* pVSBlob = NULL;
  hr = CompileShaderFromFile(L"GraphcisFramework.fx", "VS", "vs_4_0", &pVSBlob);
  if( FAILED(hr) )
  {
    MessageBox(NULL,
               L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
    return hr;
  }

  // Create the vertex shader
  hr = p_d3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &p_VertexShader);
  if( FAILED(hr) )
  {
    pVSBlob->Release();
    return hr;
  }

  // Define the input layout
  D3D11_INPUT_ELEMENT_DESC layout[] =
  {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };
  UINT numElements = ARRAYSIZE(layout);

  // Create the input layout
  hr = p_d3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
                                      pVSBlob->GetBufferSize(), &p_VertexLayout);
  pVSBlob->Release();
  if( FAILED(hr) )
    return hr;

  // Set the input layout
  p_ImmediateContext->IASetInputLayout(p_VertexLayout);

  // Compile the pixel shader
  ID3DBlob* pPSBlob = NULL;
  hr = CompileShaderFromFile(L"GraphcisFramework.fx", "PS", "ps_4_0", &pPSBlob);
  if( FAILED(hr) )
  {
    MessageBox(NULL,
               L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
    return hr;
  }

  // Create the pixel shader
  hr = p_d3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &p_PixelShader);
  pPSBlob->Release();
  if( FAILED(hr) )
    return hr;

  // Create vertex buffer
  SimpleVertex vertices[] =
  {
      { glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

      { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

      { glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

      { glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

      { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f) },

      { glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
      { glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
  };

  D3D11_BUFFER_DESC bd;
  std::memset(&bd, 0, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(SimpleVertex) * 24;
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;

  D3D11_SUBRESOURCE_DATA InitData;
  std::memset(&InitData, 0, sizeof(InitData));
  InitData.pSysMem = vertices;

  hr = p_d3dDevice->CreateBuffer(&bd, &InitData, &p_VertexBuffer);

  if( FAILED(hr) )
    return hr;

  // Set vertex buffer
  UINT stride = sizeof(SimpleVertex);
  UINT offset = 0;
  p_ImmediateContext->IASetVertexBuffers(0, 1, &p_VertexBuffer, &stride, &offset);

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

  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(WORD) * 36;
  bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  bd.CPUAccessFlags = 0;
  InitData.pSysMem = indices;
  hr = p_d3dDevice->CreateBuffer(&bd, &InitData, &p_IndexBuffer);
  if( FAILED(hr) )
    return hr;

  // Set index buffer
  p_ImmediateContext->IASetIndexBuffer(p_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

  // Set primitive topology
  p_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Create the constant buffers
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(CBNeverChanges);
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bd.CPUAccessFlags = 0;
  hr = p_d3dDevice->CreateBuffer(&bd, NULL, &p_CBNeverChanges);
  if( FAILED(hr) )
    return hr;

  bd.ByteWidth = sizeof(CBChangeOnResize);
  hr = p_d3dDevice->CreateBuffer(&bd, NULL, &p_CBChangeOnResize);
  if( FAILED(hr) )
    return hr;

  bd.ByteWidth = sizeof(CBChangesEveryFrame);
  hr = p_d3dDevice->CreateBuffer(&bd, NULL, &p_CBChangesEveryFrame);
  if( FAILED(hr) )
    return hr;

  // Load the Texture
  //hr = dx::CreateDDSTextureFromFile(p_d3dDevice, L"seafloor.dds", nullptr, &p_TextureRV);
  hr = dx::CreateWICTextureFromFile(p_d3dDevice, L"neon light.jpg", nullptr, &p_TextureRV);

  if( FAILED(hr) )
    return hr;

  // Create the sample state
  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
  hr = p_d3dDevice->CreateSamplerState(&sampDesc, &p_SamplerLinear);
  if( FAILED(hr) )
    return hr;

//************************************//************************************//************************************/************************************/
//************************************//************************************//************************************/************************************/
//************************************//************************************//************************************/************************************/
  sPerspectiveCameraDesc descriptorCamera;
  descriptorCamera.upDir = enVector3(0.0f, 1.0f, 0.0f);
  descriptorCamera.lookAtPosition = enVector3(0.0f, 0.0f, -1.0f);
  descriptorCamera.height = height;
  descriptorCamera.width = width;

  my_camera->init(descriptorCamera);


  sFirstPersonCameraDesc cameraDescriptor;
  my_firstPersonCamera->init(cameraDescriptor);

  my_manager->addCamera(my_camera);

  my_manager->addCamera(my_firstPersonCamera);

  // Initialize the world matrices
  g_World = glm::identity<glm::mat4x4>();

  // Initialize the view matrix
  glm::vec3 Eye(0.0f, 3.0f, -6.0f);
  glm::vec3 At(0.0f, 0.0f, -1.0f);
  glm::vec3 Up(0.0f, 1.0f, 0.0f);

  CBNeverChanges cbNeverChanges;
  cbNeverChanges.mView = glm::transpose(my_camera->getView() /* my_camera.getView()*/);


  p_ImmediateContext->UpdateSubresource(p_CBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

  // Initialize the projection matrix
  float TempWidth = width;
  float TempHeight = height;

  g_Projection = glm::perspectiveFovLH(glm::quarter_pi<float>(),
                                       TempWidth,
                                       TempHeight,
                                       0.01f,
                                       100.0f);

  CBChangeOnResize cbChangesOnResize;
  cbChangesOnResize.mProjection = glm::transpose(my_camera->getProjection() /*my_camera.getProjection()*/);
  p_ImmediateContext->UpdateSubresource(p_CBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);

  return S_OK;
}

HRESULT
appGraphcis::CompileShaderFromFile(const wchar_t* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
  HRESULT hr = S_OK;

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

  ID3DBlob* pErrorBlob;

  hr = D3DCompileFromFile(szFileName,
                          nullptr,
                          nullptr,
                          szEntryPoint,
                          szShaderModel,
                          dwShaderFlags,
                          0,
                          ppBlobOut,
                          &pErrorBlob);


  if( FAILED(hr) )
  {
    if( pErrorBlob != NULL )
      OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
    if( pErrorBlob ) pErrorBlob->Release();
    return hr;
  }
  if( pErrorBlob ) pErrorBlob->Release();

  return S_OK;
}

HRESULT
appGraphcis::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
  // Register class
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProcRedirect;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = L"TutorialWindowClass";
  wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);

  if( !RegisterClassEx(&wcex) )
    return E_FAIL;

  // Create window
  g_hInst = hInstance;

  RECT rc = { 0, 0, 640, 480 };

  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

  g_hWnd = CreateWindow(L"TutorialWindowClass",
                        L"Direct3D 11 Tutorial 7",
                        WS_OVERLAPPEDWINDOW,
                        100,
                        100,
                        rc.right - rc.left,
                        rc.bottom - rc.top,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);


  if( !g_hWnd )
    return E_FAIL;

  if( g_hWnd != INVALID_HANDLE_VALUE )
  {
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);
    return S_OK;
  }

  return S_FALSE;
}

void
appGraphcis::Render()
{
  // Update our time
  static float t = 0.0f;
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
  p_ImmediateContext->ClearRenderTargetView(p_RenderTargetView, ClearColor);

  //
  // Clear the depth buffer to 1.0 (max depth)
  //
  p_ImmediateContext->ClearDepthStencilView(p_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

  //
  // Render the cube
  //
  p_ImmediateContext->VSSetShader(p_VertexShader, NULL, 0);
  p_ImmediateContext->VSSetConstantBuffers(0, 1, &p_CBNeverChanges);
  p_ImmediateContext->VSSetConstantBuffers(1, 1, &p_CBChangeOnResize);
  p_ImmediateContext->VSSetConstantBuffers(2, 1, &p_CBChangesEveryFrame);
  p_ImmediateContext->PSSetShader(p_PixelShader, NULL, 0);
  p_ImmediateContext->PSSetConstantBuffers(2, 1, &p_CBChangesEveryFrame);
  p_ImmediateContext->PSSetShaderResources(0, 1, &p_TextureRV);
  p_ImmediateContext->PSSetSamplers(0, 1, &p_SamplerLinear);

  CBChangesEveryFrame cb;
  cb.vMeshColor = g_MeshColor;
  cb.mWorld = glm::transpose(g_World);

  //EveryFrame.mWorld = (cubeMatrix);

  p_ImmediateContext->UpdateSubresource(p_CBChangesEveryFrame, 0, NULL, &cb, 0, 0);


  p_ImmediateContext->DrawIndexed(36, 0, 0);

  g_World = glm::identity<glm::mat4x4>();

  // Present our back buffer to our front buffer
  //
  p_SwapChain->Present(0, 0);
}

