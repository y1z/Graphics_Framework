#pragma once
#include "util/cBaseApp.h"
#include "util/Header.h"

#include "enInputLayout.h"
#include "enPixelShader.h"
#include "enVertexShader.h"
#include "enVertexBuffer.h"
#include "enConstBuffer.h"
#include "enIndexBuffer.h"

#include <filesystem>
#include <memory>
#include <string_view>
  /**
  *  Forward declarations
  */
class enPerspectiveFreeCamera;
class enFirstPersonCamera;
class enCameraManager;


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

  bool 
  InitStatics();

  
  enErrorCode
  initDevice();

  /**
  * @brief :initializes all the classes i create
  * @bug :
  */
  bool 
  initMyClasses();

  HRESULT
  InitEverythingElse();

  /**
  * @TODO : move this function to it's own class
  * @brief : compiles the chosen shader
  * @param[in] FilePath : the path to the current shader.
  * @param[in] EntryPoint : the name of the main function for the shader
  * @bug : no known bugs
  */
  HRESULT
  CompileShaderFromFile(const wchar_t* FilePath,
                        LPCSTR EntryPoint,
                        LPCSTR ShaderModel,
                        ID3DBlob** ppBlobOut);


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

  HINSTANCE                           g_hInst = nullptr;
  HWND                                g_hWnd = nullptr;
  D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
  D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;


  //ID3D11Device* p_d3dDevice = nullptr;

  static ID3D11DeviceContext* p_ImmediateContext;
  IDXGISwapChain* p_SwapChain = nullptr;
  //ID3D11RenderTargetView* p_RenderTargetView = nullptr;
  //ID3D11Texture2D* p_DepthStencil = nullptr;
  ID3D11DepthStencilView* p_DepthStencilView = nullptr;

  std::unique_ptr<enRenderTargetView> myRenderTargetView = nullptr; 
  std::unique_ptr<enTexture2D> myDepthStencil = nullptr;
  std::unique_ptr<enPixelShader> myPixelShader = nullptr;
  std::unique_ptr<enVertexShader> myVertexShader = nullptr;
  std::unique_ptr<enInputLayout> myInputLayout = nullptr;

  ID3D11InputLayout* p_VertexLayout = nullptr;
  ID3D11Buffer* p_VertexBuffer = nullptr;
  ID3D11Buffer* p_IndexBuffer = nullptr;
  static ID3D11Buffer* p_CBNeverChanges;
  static ID3D11Buffer* p_CBChangeOnResize;
  ID3D11Buffer* p_CBChangesEveryFrame = nullptr;
  ID3D11ShaderResourceView* p_TextureRV = nullptr;
  ID3D11SamplerState* p_SamplerLinear = nullptr;

  /**************************************************/
  static enPerspectiveFreeCamera* my_camera;
  static enFirstPersonCamera* my_firstPersonCamera;
  static enCameraManager* my_manager;



  /**************************************************/
  glm::mat4x4 g_World = glm::mat4x4(1.0f);
  glm::mat4x4 g_Projection = glm::mat4x4(1.0f);
  glm::vec4 g_MeshColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);

  static bool my_initIsFinish;

  const std::filesystem::path g_initPath = std::filesystem::current_path();
};


