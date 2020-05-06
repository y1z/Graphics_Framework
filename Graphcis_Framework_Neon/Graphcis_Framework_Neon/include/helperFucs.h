#pragma once
#include "util/Header.h"
#include <string>
#include <string_view>
#include <cstdlib>
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

    size_t index = wideString.length() - 1;

    for( auto &element : wideString ){

      int status;
      wctomb_s(&status, &Result[index], sizeof(char), element);
      --index;
      assert(status != -1 && "conversion is not possible give the current local");
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

    assert(checkForError != static_cast<std::size_t>(-1) && "invalid string conversion");

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
      std::wcerr << "error with path { " << filePath << " }\n";
    }
    return Result;
  }


  /**
  * @brief : initializes important part of the api.
  * @bug : no known bugs.
  */
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
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUGGABLE;	
  #endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
   uint32 const numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    uint32 const numFeatureLevels = ARRAYSIZE(featureLevels);


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

    hardwareInfo.majorVersion = majorVersion;
    hardwareInfo.minorVersion = minorVersion;

    const unsigned char* OpenglVersion = glGetString(GL_VERSION);
    const unsigned char* GlslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const unsigned char* OpenglRenderer = glGetString(GL_RENDERER);
    const unsigned char* OpenglVender = glGetString(GL_VENDOR);

    std::cout << "GLFW version : " << "Major [" << majorVersion << "] Minor [" << minorVersion << "]\n"
      << "Open_gl version : " << OpenglVersion << '\n'
      << "open_gl Vendor : "<< OpenglVender <<'\n'
      << "Glsl  Shader version : " << GlslVersion << '\n'
      << "Open_gl renderer : " << OpenglRenderer <<"\n\n\n";


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


  /**
  * @returns : the size of the window .
  * @bug : no known bugs.
  */
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
    glfwGetFramebufferSize(window.getHandle(), &windowWidth, &windowHeight);

  #endif // DIRECTX
    return enVector2(windowWidth, windowHeight);
  }

  /**
  * @returns : a matrix that may or may not be transposed ( depends on the api).
  * @bug : no known bugs.
  */
  static enMatrix4x4 
  arrangeForApi(enMatrix4x4& mat)
  {
  #if DIRECTX
     mat = glm::transpose(mat);
     return mat ;
  #elif OPENGL

     return mat ;
  #endif // DIRECTX
  }

  /**
  * @returns : descriptor that used for a DepthStencil .
  * @bug :no known bugs.
  */
  EN_NODISCARD static constexpr sTextureDescriptor
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

  /**
  * @returns : descriptor that used for a render-Target.
  * @bug :no known bugs.
  */
  EN_NODISCARD static constexpr sTextureDescriptor
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

  /**
  * @brief : creates instance of 'enMultiViewType' that possible has multiple different values OR'ED 
  *  together example (enMultiViewType::renderTarget  | enMultiViewType::depthStencil )
  *  
  * @bug : no known bugs.
  */
  EN_NODISCARD static enMultiViewType 
  generateMultiViewType(int32 valueOfType)
  {
    enMultiViewType result = enMultiViewType::zeroType;
    result = static_cast<enMultiViewType>(valueOfType);
    return result;
  }


  /**
  * @brief : creates a 'sUniformDetails'
  * @bug : no known bugs.
  */
  EN_NODISCARD static sUniformDetails
  GlCreateUniformDetail(std::string_view name, enConstBufferElem type)
  {
    sUniformDetails result;
    result.name = name;
    result.element = type;

    return result;
  }

  /**
  * @brief : updates the uniform variables in the glsl shader
  * @param[in] details : contains the type of the variable and the values
  * that the variable should contain.
  * @bug : no known bug.
  */
  EN_NODISCARD static void
  GlUpdateUniform(sUniformDetails& details)
  {
    if( details.ptr_data == nullptr )
    {
      std::cerr << "uniform data pointer is null ";
      assert(details.ptr_data != nullptr);
    }

    switch( details.element )
    {
      case enConstBufferElem::NONE:
      assert(details.element != enConstBufferElem::NONE && "element in the buffer has no type");
      break;

      case enConstBufferElem::mat4x4:
      {
        glUniformMatrix4fv(details.id,
                           1,
                           GL_TRUE,
                           static_cast< const GLfloat* > (details.ptr_data));
      }
      break;

      case enConstBufferElem::mat3x3:
      {
        glUniformMatrix3fv(details.id,
                           1,
                           GL_TRUE,
                           static_cast< const  GLfloat* > (details.ptr_data));
      }
      break;

      case enConstBufferElem::vec4:

      {
        glUniform4fv(details.id,
                     1,
                     static_cast< const  GLfloat* > (details.ptr_data));
      }
      break;
      
      case enConstBufferElem::vec3:
      {
        glUniform3fv(details.id,
                     1,
                     static_cast< const GLfloat* > (details.ptr_data));
      }
      break;

      case enConstBufferElem::vec2:
      {
        glUniform2fv(details.id,
                     1,
                     static_cast< const  GLfloat* > (details.ptr_data));
      }
      break;

      case enConstBufferElem::single_float:
      {
        glUniform1fv(details.id,
                     1,
                     static_cast< const  GLfloat* > (details.ptr_data));
      }
      break;

      case enConstBufferElem::imat4x4:
      break;

      case enConstBufferElem::imat3x3:
      break;

      case enConstBufferElem::ivec4:
      break;

      case enConstBufferElem::ivec3:
      break;
      case enConstBufferElem::ivec2:
      break;
      case enConstBufferElem::single_int:
      break;
      default:
      assert(details.element != enConstBufferElem::NONE && "element in the buffer has no type");
      break;
    }

  }


}

