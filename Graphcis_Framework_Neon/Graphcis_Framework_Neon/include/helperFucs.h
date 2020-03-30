#pragma once
#include "util/Header.h"
#include <string>
#include <string_view>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"

#include "enDevice.h"
#include "enDeviceContext.h"
#include "enSwapChain.h"
#include "enWindow.h"
#include "cApiComponent.h"

namespace helper
{
  /*!converts radians to degrees*/
  static constexpr float
  radiansToDegrees(float radians)
  {
    return radians *= (180.0f / 3.14159f);
  }

  /*!converts degrees to radians*/
  static constexpr float
  degreesToRadians(float degrees)
  {
    return  degrees *= (3.14159f / 180.0f);
  }


  /**
  *@brief converts a wstring/const wchar_t* to it's string equivalent assuming it has any.
  */
  static std::string
  convertWStringToString(std::wstring_view wideString)
  {
    std::string Result(wideString.length() + 1, '\0');

    // converts a wide string to a char/multi-byte string
    std::size_t checkForError = std::wcstombs(Result.data(),
                                              wideString.data(),
                                              wideString.length());
    // how to check for errors  https://en.cppreference.com/w/cpp/string/multibyte/wcstombs
    if( checkForError == static_cast<std::size_t>(-1) )
    {
      assert(checkForError != static_cast<std::size_t>(-1) && "invalid string conversion");
    }

    return Result;
  }

  /**
  *@brief converts a string/const char* to it's wstring equivalent
  */
  static std::wstring
    convertStringToWString(std::string_view String)
  {
    std::wstring Result(String.length() + 1, '\0');

    std::size_t checkForError = std::mbstowcs(Result.data(), String.data(), String.length());

    if( checkForError == static_cast<std::size_t>(-1) )
    {
      assert(checkForError != static_cast<std::size_t>(-1) && "invalid string conversion");
    }

    return Result;
  }


  static std::string
    loadFileToString(std::string_view filePath)
  {
    std::string Result{ "Error" };
    std::ifstream File(filePath.data());

    if( File.is_open() )
    {
      std::stringstream SStream;
      SStream << File.rdbuf();
      Result = SStream.str();
      File.close();
      return Result;
    }
    else
    {
      std::cerr << "Error with path {" << filePath << "}\n";
    }
    return Result;
  }
  /*************/

  static std::string
    loadFileToString(std::wstring_view filePath)
  {
    std::string Result{ "Error" };
    std::wifstream File(filePath.data());
    if( File.is_open() )
    {
      std::stringstream SStream;
      SStream << File.rdbuf();
      Result = SStream.str();

      File.close();
      return Result;
    }
    else
    {
      std::wcerr << "error with path {" << filePath << "}\n";
    }
    return Result;
  }

  // TODO : add function to resource manager
  EN_NODISCARD static enErrorCode
  CreateDeviceAndSwapchain(enSwapChain& swapChain,
                           enWindow& window,
                           sHardWareInfo& hardwareInfo)
  {
    enDevice& device = enDevice::getInstance();
    enDeviceContext& deviceContext = enDeviceContext::getInstance();
  #if DIRECTX
    RECT rc;

    HWND handleForWindow = window.getHandle();
    GetClientRect(handleForWindow, &rc);
    uint32 width = rc.right - rc.left;
    uint32 height = rc.bottom - rc.top;

    uint32 createDeviceFlags = 0;
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
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);


    sSwapDesc swapchainDescriptor;
    swapchainDescriptor.buffCount = 1;
    swapchainDescriptor.buffFormat = enFormats::R8G8B8A8_uniform_norm;
    swapchainDescriptor.buffHeight = height;
    swapchainDescriptor.buffWidth = width;
    swapchainDescriptor.buffRefershDenominator = 1;
    swapchainDescriptor.buffRefershNumaretor = 60;
    swapchainDescriptor.buffUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDescriptor.outputWindow = handleForWindow;
    swapchainDescriptor.isWindowd = TRUE;
    swapchainDescriptor.sampCount = 1;
    swapchainDescriptor.sampQuality = 0;

    swapChain.init(swapchainDescriptor);

    DXGI_SWAP_CHAIN_DESC sd = swapChain.getdxSawpChainDesc();


    HRESULT hr = S_OK;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
      hardwareInfo.m_dxHardwareInfo = driverTypes[driverTypeIndex];
      hr = D3D11CreateDeviceAndSwapChain(NULL,
                                         hardwareInfo.m_dxHardwareInfo,
                                         NULL,
                                         createDeviceFlags,
                                         featureLevels,
                                         numFeatureLevels,
                                         D3D11_SDK_VERSION,
                                         &sd,
                                         &swapChain.m_interface,
                                         device.getInterfaceRef(),
                                         &hardwareInfo.m_dxApiversion,
                                         deviceContext.getInterfaceRef());
      if( SUCCEEDED(hr) )
        break;
    }

    if( SUCCEEDED(hr) )
      return enErrorCode::NoError;


    return enErrorCode::FailedCreation;
    #elif OPENGL
    GlRemoveAllErrors();

    swapChain.setWindow(window);

    int32 majorVersion{ 0 };
    int32 minorVersion{ 0 };

    glfwGetVersion(&majorVersion, &minorVersion, NULL);

    const unsigned char* OpenglVersion = glGetString(GL_VERSION);
    const unsigned char* GlslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const unsigned char* OpenglRenderer = glGetString(GL_RENDERER);

    std::cout << "GLFW version : " << "Major [" << majorVersion << "] Minor [" << minorVersion << "]\n"
      << "Open_gl version : " << OpenglVersion << '\n'
      << "Glsl  Shader version : " << GlslVersion << '\n'
      << "Open_gl renderer : " << OpenglRenderer << '\n';
      //"alignment of sLightData : " << alignof(sLightData) << '\n' <<
      //" size of sLightData : " << sizeof(sLightData) << std::endl;

    cApiComponents apiComponent;

    uint32* ptr_vertexArrayObject = cApiComponents::getvertexArrayObject();

    glGenVertexArrays(1, ptr_vertexArrayObject);
    glBindVertexArray(*ptr_vertexArrayObject);

    glfwSwapInterval(1);

    uint32* ptr_ShaderProgram = cApiComponents::getShaderProgram();

    *(ptr_ShaderProgram) = glCreateProgram();

    if( GlCheckForError() )
    {
      EN_LOG_ERROR;
      return   enErrorCode::NotReady;
    }

    return enErrorCode::NoError;
  #endif // DIRECTX

    return  enErrorCode::UnClassified;
  }


  EN_NODISCARD static enVector2 
  getWindowSize(enWindow& window)
  {
    int32 windowWidth = -1337;
    int32 windowHeight = -1337;
  #if DIRECTX
    RECT widowDimensions;
    GetClientRect(window.getHandle(), &widowDimensions);
    windowWidth = widowDimensions.right - widowDimensions.left;
    windowHeight = widowDimensions.bottom - widowDimensions.top;

  #elif OPENGL
    glfwGetWindowSize(window.getHandle(), &windowWidth, &windowHeight);

  #endif // DIRECTX
    return enVector2(windowWidth, windowHeight);
  }

  static void 
  arrangeForApi(enMatrix4x4& mat)
  {
  #if DIRECTX
     mat = glm::transpose(mat);
  #elif OPENGL
  #endif // DIRECTX
  }

  EN_NODISCARD static sTextureDescriptor
  generateTextureDescForDepthStencil(float const Width,
                                     float const Height)
  {
    sTextureDescriptor result;
    result.texWidth = Width;
    result.texHeight = Height;
    result.CpuAccess = 0;
    result.texFormat = static_cast<int>(enFormats::depthStencil_format);
    result.Usage = enBufferUse::Default;
    result.BindFlags = enBufferBind::DepthStencil;
    result.arraySize = 1;
    return result;
  }

  EN_NODISCARD static sTextureDescriptor
  generateTextureDescForRenderTarget(float const Width,
                                     float const Height)
  {
    sTextureDescriptor result;
    result.texWidth = Width;
    result.texHeight = Height;
    result.CpuAccess = 0;
    result.BindFlags = enBufferBind::RenderTarget;
    result.texFormat = static_cast<int>(enFormats::renderTarget_format);
    result.Usage = enBufferUse::Default;
    result.arraySize = 1;
    return result;
  }
  
  EN_NODISCARD static enMultiViewType 
  generateMultiViewType(int32 valueOfType)
  {
    enMultiViewType result = static_cast<enMultiViewType>(0);
    result = static_cast<enMultiViewType>(valueOfType);
    return result;
  }

  EN_NODISCARD static sUniformDetails
  GlCreateUniformDetail(std::string_view name, enConstBufferElem type)
  {
    sUniformDetails result;
    result.name = name;
    result.element = type;

    return result;
  }
}

