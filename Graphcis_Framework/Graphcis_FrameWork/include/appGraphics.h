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
#include "enShaderResourceView.h"
#include "enWindow.h"
#include "enModel.h"
#include "imGuiManager.h"
#include "enMultiviewTexture.h"

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
  * @brief : Takes care of all graphics related operation.
  * @bug : no known bugs.
  */
class appGraphics :
  public cBaseApp
{
public:
  appGraphics() = default;
  ~appGraphics() = default;


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
  * @brief : starts the api. 
  * @bug :
  */
  enErrorCode
  initApi();

  /**
  * @brief : creates all the classes .
  * @bug : no known bugs.
  */
  bool
  createMyClasses();

  /**
  * @brief : where all the container 
  * @bug : no known bugs.
  */
  bool 
  initContainers();

  /**
  * @brief : prepares the app for rendering 
  * @bug : no known bugs
  */
  HRESULT
  initForRender();

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

  /**
  * @brief : where each class performs it own update.
  * @bug :no known bugs.
  */
  void
  Update();

public:

  sHardWareInfo m_hardwareInfo;

  HMODULE  m_moduleInstance = nullptr;

  std::unique_ptr<enSwapChain> m_swapchain = nullptr;
  std::unique_ptr<enWindow> m_window = nullptr;

  std::unique_ptr<enDepthStencilView> m_depthStencilView = nullptr;
  std::unique_ptr<enRenderTargetView> m_renderTargetView = nullptr; 

  std::unique_ptr<enPixelShader> m_pixelShader = nullptr;
  std::unique_ptr<enVertexShader> m_vertexShader = nullptr;

  std::unique_ptr<enInputLayout> m_inputLayout = nullptr;

  std::unique_ptr<enViewport> m_viewport = nullptr;

  std::unique_ptr<enVertexBuffer> m_vertexBuffer = nullptr;
  std::unique_ptr<enIndexBuffer> m_indexBuffer = nullptr;
  
  std::unique_ptr<enConstBuffer> m_worldMatrix = nullptr;

  std::unique_ptr<enSampler> m_sampler = nullptr;
  std::unique_ptr<enModel> m_model = nullptr;
  static std::unique_ptr<imGuiManager> m_gui; 

  std::unique_ptr<enMultiviewTexture> m_multiTexture = nullptr;

  //std::unique_ptr<enShaderResourceView> m_resourceView = nullptr;
  std::shared_ptr<enShaderResourceView> m_resourceView = nullptr;
  static std::shared_ptr<enShaderResourceView> s_viewIntoOtherCam;

  static enConstBuffer * s_ViewMatrixBuffer;
  static enConstBuffer * s_ProjectionMatrixBuffer;

  /**************************************************/
  static enPerspectiveFreeCamera* s_Camera;
  static enFirstPersonCamera* s_FirstPersonCamera;
  static enCameraManager* s_CameraManager;


  /**************************************************/
  glm::mat4x4 m_World = glm::mat4x4(1.0f);
  glm::mat4x4 m_Projection = glm::mat4x4(1.0f);

  std::vector<enConstBuffer*> m_ConstBufferContainer;

  glm::vec4 m_MeshColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);

  const std::filesystem::path m_initPath = std::filesystem::current_path();

  static bool s_initIsFinish;
  static bool s_useFreeCam;
};


