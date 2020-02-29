#pragma once
#include "util/cBaseApp.h"
#include "util/Header.h"

#include "enInputLayout.h"
#include "enPixelShader.h"

#include "enVertexShader.h"
#include "enVertexBuffer.h"

#include "enConstBuffer.h"
#include "enIndexBuffer.h"
#include "enViewport.h"
#include "enSampler.h"
#include "enSwapChain.h"

#include <filesystem>
#include <memory>
#include <string_view>
  /**
  *  Forward declarations
  */
class enPerspectiveFreeCamera;
class enFirstPersonCamera;
class enCameraManager;
class enShaderResourceView;


  /**
  * @brief : takes care of all graphics related operation
  * @bug :
  */
class appGraphcis :
  public cBaseApp
{
public:
  appGraphcis ();
  ~appGraphcis() = default;


public:
  /**
  * @brief :  where all resource are gathered and classes are initialized
  * @bug : no know bugs
  */
  virtual bool
  init() override;

  /**
  * @brief : this is where the main application starts after gathering resources
  * @bug : no know bugs
  */
  virtual int
  run()override;

  /**
  * @brief : used for freeing resources
  * @bug : no know bugs
  */
  virtual void
  destroy()override;

private:
  

  static LRESULT CALLBACK 
  WndProcRedirect(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
  /**
  * @brief : initializes all static classes. 
  * @bug : no known bug
  */
  bool 
  InitStatics();

  /**
  * @brief : initializes all modules classes.
  * @bug :
  */
  bool 
  initModules();
  
  /**
  * @brief : 
  * @bug :
  */
  enErrorCode
  initDevice();

  /**
  * @brief :initializes all the classes i create
  * @bug :
  */
  bool 
  initMyClasses();

  /**
  * @brief : prepares the app for rendering 
  * @bug : no known bugs
  */
  HRESULT
  InitForRender();

 /**
 * @TODO : move this function to it's own class
 * @brief : creates a window
 **/
  HRESULT
  InitWindow(HINSTANCE hInstance, int nCmdShow);

  /**
  * @brief : takes care of rendering the image to the screen.
  * @bug : no known bugs.
  */
  void
  Render();
public:

  HWND                                g_hWnd = nullptr;

  sHardWareInfo m_hardwareInfo;

  D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
  D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;


  HMODULE  m_moduleInstance = nullptr;
  //ID3D11Device* p_d3dDevice = nullptr;

  //static ID3D11DeviceContext* p_ImmediateContext;
  IDXGISwapChain* p_SwapChain = nullptr;
  std::unique_ptr<enSwapChain> mySwapchain = nullptr;
  //ID3D11RenderTargetView* p_RenderTargetView = nullptr;
  //ID3D11Texture2D* p_DepthStencil = nullptr;
  //ID3D11DepthStencilView* p_DepthStencilView = nullptr;

  std::unique_ptr<enDepthStencilView> myDepthStencilView = nullptr;
  std::unique_ptr<enRenderTargetView> myRenderTargetView = nullptr; 
  std::unique_ptr<enTexture2D> myDepthStencil = nullptr;

  std::unique_ptr<enPixelShader> myPixelShader = nullptr;
  std::unique_ptr<enVertexShader> myVertexShader = nullptr;

  std::unique_ptr<enInputLayout> myInputLayout = nullptr;

  std::unique_ptr<enViewport> myViewPort = nullptr;

  std::unique_ptr<enVertexBuffer> myVertexBuffer = nullptr;
  std::unique_ptr<enIndexBuffer> myIndexBuffer = nullptr;

  static enConstBuffer * myViewMatrixBuffer;
  static enConstBuffer * myProjectionMatrixBuffer;
  std::unique_ptr<enConstBuffer> myWorldMatrix;
  std::unique_ptr<enShaderResourceView> myResourceView  = nullptr;
  std::unique_ptr<enSampler> mySampler = nullptr;

  //ID3D11ShaderResourceView* p_TextureRV = nullptr;
  //ID3D11SamplerState* p_SamplerLinear = nullptr;

  /**************************************************/
  static enPerspectiveFreeCamera* myCamera;
  static enFirstPersonCamera* myFirstPersonCamera;
  static enCameraManager* myCameraManager;



  /**************************************************/
  glm::mat4x4 g_World = glm::mat4x4(1.0f);
  glm::mat4x4 g_Projection = glm::mat4x4(1.0f);
  glm::vec4 g_MeshColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);

  static bool my_initIsFinish;

  const std::filesystem::path g_initPath = std::filesystem::current_path();
};


