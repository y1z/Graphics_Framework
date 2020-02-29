#pragma once
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <cassert>
#include <array>
#include "util/GraphicsDefines.h"


#if DIRECTX
#include <d3d11.h>
#endif//DIRECTX

/**
 * @brief This file contains all the typedef for the entropy engine
 *
 */

/****************************/
// TYPE DEFS
/****************************/

// signed int type's
using int8 = int8_t; /*!<this is a 8-bit integer */
using int16 = int16_t;//<! this is a 16-bit integer 
using int32 = int32_t;//<! this is a 32-bit integer 
using int64 = int64_t;//<! this is a 64-bit integer 

// unsigned int type's 
using uint8 = uint8_t;//<! this is a 8-bit unsigned integer 
using uint16 = uint16_t;//<! this is a 16-bit unsigned integer 
using uint32 = uint32_t;//<! this is a 32-bit unsigned integer 
using uint64 = uint64_t;//<! this is a 64-bit unsigned integer 

using confInt = std::size_t;//<! this is a int that changes size in bytes depending on the configuration

// signed int type's that tell you how much memory they use 
using Byte = int_least8_t;//<! a single byte 
using Byte2 = int_least16_t;//<! 2 bytes per instance
using Byte4 = int_least32_t;//<! 4 bytes per instance 
using Byte8 = int_least64_t;//<! 8 bytes per instance 

// unsigned int type's that tell you how much memory they use 
using uByte = uint_least8_t;//<! a single byte 
using uByte2 = uint_least16_t;//<! 2 bytes per instance
using uByte4 = uint_least32_t;//<! 4 bytes per instance 
using uByte8 = uint_least64_t;//<! 8 bytes per instance 

/*++++++++++++++++++++++++++++++++++++*/
/* ENUMS */
/*++++++++++++++++++++++++++++++++++++*/

/**
*@brief used to determine the error that was committed
*/
enum enErrorCode :Byte4
{
  NoError = 0b000'0000'0000'0000'0000'0000'0000'0000, //!<  indicates that no error occurred  
  UnClassified = 0b000'0000'0000'0000'0000'0000'0000'0001,//!<  indicates that there is no-know name for the error 
  NotReady = 0b000'0000'0000'0000'0000'0000'0000'0010, //!<  means that the function/class/object needed more set up before it's ready to be used  
  InvalidPath = 0b000'0000'0000'0000'0000'0000'0000'0100,//!< means that a provided path is invalid 
  ShaderComplieError = 0b000'0000'0000'0000'0000'0000'0000'1000,//!< means that an error occurred with compiling the shader
  ShaderLinkError = 0b000'0000'0000'0000'0000'0000'0001'0000, //!< means that a provided path 
  ActorComponentError = 0b000'0000'0000'0000'0000'0000'0010'0000,//!< means that a provided path 
  FailedCreation = 0b000'0000'0000'0000'0000'0000'0100'0000,//!<means that something was not created correctly.
};

/**
*@brief used to tell the a.p.i which type of buffer we are dealing with.
*/
enum enBufferBind
{
  NONE = 0,
#if DIRECTX
  Vertex = D3D11_BIND_VERTEX_BUFFER,
  Index = D3D11_BIND_INDEX_BUFFER,
  Const = D3D11_BIND_CONSTANT_BUFFER,
  ShaderResource = D3D11_BIND_SHADER_RESOURCE,
  RenderTarget = D3D11_BIND_RENDER_TARGET,
  DepthStencil = D3D11_BIND_DEPTH_STENCIL,
#else
  Vertex = 0b00'00'0000'0001,
  Index = 0b00'00'0000'0010,
  Const = 0b00'00'0000'0100,
  ShaderResource = 0b00'00'0000'1000,
  RenderTarget = 0b00'00'0010'0000,
  DepthStencil = 0b00'00'0100'0000,
#endif // DIRECTX
};


enum enBufferUse
{
#if DIRECTX
  Default = D3D11_USAGE_DEFAULT,
#elif OPENGL
  Default = 0,
#endif//DIRECTX 
};


/** tells the a.p.i how the information is
formated
*/
enum enFormats
{
#if DIRECTX
  /* one channel */
  uR8 = DXGI_FORMAT_R8_UINT,
  uR16 = DXGI_FORMAT_R16_UINT,
  fR16 = DXGI_FORMAT_R16_FLOAT,
  uR32 = DXGI_FORMAT_R32_UINT,
  fR32 = DXGI_FORMAT_R32_FLOAT,

  /* two channel*/
  fR16G16 = DXGI_FORMAT_R16G16_FLOAT,
  /* three channel */

  /*Four channel */
  R8G8B8A8_uniform_norm = DXGI_FORMAT_R8G8B8A8_UNORM,
  fR16G16B16A16 = DXGI_FORMAT_R16G16B16A16_FLOAT,
  fR32G32B32A32 = DXGI_FORMAT_R32G32B32A32_FLOAT,
  /* other */
  depthStencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT
#elif OPEN_GL//TODO : GL
  /* one channel */
  uR8 = GL_UNSIGNED_BYTE, //GL_R8
  uR16 = GL_UNSIGNED_SHORT,
  fR16 = GL_HALF_FLOAT,
  uR32 = GL_UNSIGNED_INT,
  fR32 = GL_FLOAT,
/*two channel */
fR16G16 = GL_RGB16F,
/* three channel */

/*Four channel */
R8G8B8A8_uniform_norm,
fR16G16B16A16 = GL_RGBA16F,

fR32G32B32A32 = GL_RGBA32F,
/* other */
depthStencil_format = GL_DEPTH24_STENCIL8,
#else

  uR8,
  uR16,
  fR16,
  uR32,
  fR32,
/*two channel */
fR16G16,
/* three channel */

/*Four channel */
R8G8B8A8_uniform_norm,
fR16G16B16A16,
fR32G32B32A32,
/* other */
depthStencil_format,
#endif // DIRECTX
};


enum DepthStencilFormat
{
#if DIRECTX
  two_dimention = D3D11_DSV_DIMENSION_TEXTURE2D,
#elif OPENGL
  two_dimention,
#endif // DIRECTX
};

enum class enTopology
{
  UnDefined = 0,
#if DIRECTX
  TriList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
  PointList = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
  LineList = D3D_PRIMITIVE_TOPOLOGY_LINELIST
#elif OPENGL
  TriList = GL_TRIANGLES,
  PointList = GL_POINTS,
  LineList = GL_LINES
#endif // DIRECTX
};



enum class enFilter
{
#if DIRECTX
  MinMagMip_Point = D3D11_FILTER_MIN_MAG_MIP_POINT,
  MinMagMip_Point_Linear = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
  MinMagMip_Linear_Mip_Point = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
  MinMagMip_MagMip_Linear = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
  MinMagMip_Linear = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
  /**************************************/
  Anisotropic = D3D11_FILTER_ANISOTROPIC,
  Anisotropic_Comperasion = D3D11_FILTER_COMPARISON_ANISOTROPIC,
  Anisotropic_Minimum = D3D11_FILTER_MINIMUM_ANISOTROPIC,
  Anisotropic_Maximum = D3D11_FILTER_MAXIMUM_ANISOTROPIC
#elif OPEN_GL
  MinMagMip_Point,
  MinMagMip_Point_Linear,
  MinMagMip_Linear_Mip_Point,
  MinMagMip_MagMip_Linear,
  MinMagMip_Linear,
  /**************************************/
  Anisotropic,
  Anisotropic_Comperasion,
  Anisotropic_Minimum,
  Anisotropic_Maximum
#else
  MinMagMip_Point,
  MinMagMip_Point_Linear,
  MinMagMip_Linear_Mip_Point,
  MinMagMip_MagMip_Linear,
  MinMagMip_Linear,
    /**************************************/
    Anisotropic,
    Anisotropic_Comperasion,
    Anisotropic_Minimum,
    Anisotropic_Maximum,
  #endif // DIRECTX
};


enum class enComparison
{
#if DIRECTX
  Never = D3D11_COMPARISON_NEVER,
  Less = D3D11_COMPARISON_LESS,
  Equal = D3D11_COMPARISON_EQUAL,
  Less_Equal = D3D11_COMPARISON_LESS_EQUAL,
  Greater = D3D11_COMPARISON_GREATER,
#elif OPEN_GL 
  Never = GL_NEVER,
  Less = GL_LESS,
  Equal = GL_EQUAL,
  Less_Equal = GL_LEQUAL
#else 

  Never,
  Less,
  Equal,
  Less_Equal
#endif // DIRECTX
};

enum class enAddress : int32_t
{
#if DIRECTX
  Wrap = D3D11_TEXTURE_ADDRESS_WRAP,
  Mirror = D3D11_TEXTURE_ADDRESS_MIRROR,
  Clamp = D3D11_TEXTURE_ADDRESS_CLAMP,
  Border = D3D11_TEXTURE_ADDRESS_BORDER,
  Mirror_once = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
#elif OPEN_GL
  Wrap,
  Mirror,
  Clamp,
  Border,
  Mirror_once
#else
  Wrap,
  Mirror,
  Clamp,
  Border,
  Mirror_once
#endif // DIRECTXe
};

/*++++++++++++++++++++++++++++++++++++*/
/* Logger functions */
/*++++++++++++++++++++++++++++++++++++*/
namespace enError
{
  /*! Present error message to help with debugging */
  static void ENTROPY_log_error(const char* FunctionName,
                                std::size_t LineNumber,
                                const char* FileName)
  {
    std::cerr << "====================================" <<
      '\n' << "Error occurred in Function :\"" << FunctionName << "\"\n"
      << "On line [" << LineNumber << "]\n"
      << "In File \"" << FileName << '\"' << '\n';
  }

  static void ENTROPY_log_error_code(const char* FunctionName,
                                     std::size_t LineNumber,
                                     const char* FileName,
                                     enErrorCode errorCode = enErrorCode::UnClassified)
  {
    //this is the format all messages will follow 
    auto messageFormat = [](const char* FuncName, const char* ErrorReport) {
      std::clog << '\t' << ErrorReport << '\n' << '\t' << "in Function : '" << FuncName << "'\n\n";
    };

    ENTROPY_log_error(FunctionName,
                      LineNumber,
                      FileName);

    switch( errorCode )
    {
      case enErrorCode::NoError:
        messageFormat(FunctionName, "executed successfully");
        break;
      case enErrorCode::UnClassified:
        messageFormat(FunctionName, "Some Un-Classified Error occurred");
        break;
      case enErrorCode::NotReady:
        messageFormat(FunctionName, "Class needs more setup before it can be used");
        break;
      case enErrorCode::InvalidPath:
        messageFormat(FunctionName, "Provided path is invalid");
        break;
      case enErrorCode::ShaderComplieError:
        messageFormat(FunctionName, "failed to compile shader");
        break;
      case enErrorCode::ShaderLinkError:
        messageFormat(FunctionName, "failed to link shader");
        break;
      case enErrorCode::ActorComponentError:
        messageFormat(FunctionName, "Component is one of the following \n"
                      " ==> 1) does not exist in the current actor \n"
                      " ==> 2) the component needed more set-up in order to do it's job ");
        break;
      case enErrorCode::FailedCreation:
        messageFormat(FunctionName,
                      R"(the creation of some resource has failed, check if more setup is needed
or if the setup wrong.)");

        break;
      default:
        messageFormat(FunctionName, "No error massage set");
        break;
    }

  }

  static bool//success
    enCheckSuccess(enErrorCode code)
  {
    if( code == enErrorCode::NoError )
    {
      return true;
    }
    return false;
  }
}




/*++++++++++++++++++++++++++++++++++++*/
/* define's  Debug */
/*++++++++++++++++++++++++++++++++++++*/


#define EN_FILENAME std::strrchr(__FILE__,'\\')  ?  std::strrchr(__FILE__,'\\') + 1 : __FILE__ 

#ifndef  NDEBUG 
/*! output that an error occurred
in some function in some file in some line
does not indicate the error only where it possibly
happen */
#define EN_LOG_ERROR enError::ENTROPY_log_error(__func__,__LINE__, (EN_FILENAME) );
#define EN_LOG_ERROR_WITH_CODE(ErrorCode) enError::ENTROPY_log_error_code(__func__,__LINE__, (EN_FILENAME),ErrorCode);
#define EN_LOG_DB(message) std::cerr << #message << '\n';
#else
// does nothing 
#define EN_LOG_ERROR
#define EN_LOG_ERROR_WITH_CODE(ErrorCode)
#define EN_LOG_DB(message)
#endif // !NDEBUG

#define LONG_CHAR(Message) L##Message
#define GENERIC_CHAR(Message) #Message

#if _UNICODE || UNICODE
#define EN_CHAR(Message) LONG_CHAR(Message)

#else 
#define EN_CHAR(Message) GENERIC_CHAR(Message)

#endif// _UNICODE

/*++++++++++++++++++++++++++++++++++++*/
/* define's  utility*/
/*++++++++++++++++++++++++++++++++++++*/

#define DELETE_PTR(ptr) if(ptr != nullptr) {delete ptr; ptr= nullptr;}
#define RELEASE_DX_PTR(dx_ptr) if(dx_ptr){dx_ptr->Release(); dx_ptr = nullptr;}
  /**
  * @retuns : true when the ErrorCode is `NoError` anything else returns false
  * @bug : no known bugs
  */
#define EN_SUCCESS(errorCode) enError::enCheckSuccess(errorCode)


/*++++++++++++++++++++++++++++++++++++*/
/* struct's */
/*++++++++++++++++++++++++++++++++++++*/

struct sColorf
{
  union
  {
    struct
    {
      float red;
      float green;
      float blue;
      float alpha;
    };
    float allColor[4]{ 0.0f,0.0f,0.0f,1.0f };
  };
};


struct sTextureDescriptor
{
  uint32 texWidth{ 0 };
  uint32 texHeight{ 0 };
  int texFormat{ 0 };//! used to know how to interpret the data 
  int Usage{ 0 };
  int BindFlags{ 0 };
  int CpuAccess{ 0 };
  // this is in case the texture comes in an array format 
  int arraySize{ 1 };
};

//! this will represent the data my buffer has 
struct sBufferDesc
{
  void* ptr_data{ nullptr };
  confInt sizeOfBuffer{ 0u };
  confInt elementCount{ 0u };
  confInt stride{ 0u };
  enBufferBind bindFlags = enBufferBind::NONE;

  int32 usage{ 0 };
  uint32 cpuAccess{ 0 };
  uint32 miscFlags{ 0 };
  uint32 structured;
  uint32 index = static_cast<uint32>(-1);
};


struct sDepthStencilDescriptor
{
  int Format{ 0 };
  int Dimension{ 0 };
  int Mip{ 0 };
};

/*! tells the input layout how to interpret the data */
struct sInputDescriptor
{
  std::string Name{ "" };
  uint32_t Index{ 0 };
  uint32_t Format{ 0 };
  uint32_t Slot{ 0 };
  uint32_t Alignment{ 0 };
  uint32_t SlotClass{ 0 };
  uint32_t InstanceData{ 0 };
};

struct sSamplerDesc
{
  uint32_t filter{ 0 };//<! how to filter the data 
  // how to address the x , y ,z coordinates of a sampled texture  
  uint32_t addressU{ 0 };//<! how to address the x axis 
  uint32_t addressV{ 0 };//<! how to address the y axis 
  uint32_t addressW{ 0 };//<! how to address the z axis 

  int comparingFunc{ 0 };//<! how to compare 
  uint32_t AnisotropicLevel{ 1 };//<! controls the how anisotropic the texture is 
  int32_t index = 0;
  float boderColor[4]{ 0.0f,0.0f,0.0f,0.0f }; //<! is only used in certain modes 
  float minLod{ 0.0f };//<! lowest level of detail 
  float maxLod{ 0.0f };//<! highest level of detail
};

//! intermediate for using it in multiple API's  
struct sViewportDesc
{
  uint32 width{ 1u };
  uint32 height{ 1u };
  float minDepth{ 0.0f };
  float maxDepth{ 0.0f };
  float TopLeftX{ 0.0f };
  float TopLeftY{ 0.0f };
};

struct sHardWareInfo
{
#if DIRECTX
  D3D_FEATURE_LEVEL m_dxApiversion;
  D3D_DRIVER_TYPE m_dxHardwareInfo;
#elif OPENGL
    
    
 union
 {
   struct
   {
      int majorVersion;
      int minorVersion;
   };
   int m_glApiversion[2]{ 0 };
 };

#endif // DIRECTX



};


/*! controls the settings for the swap-chain*/
struct sSwapDesc
{
#if DIRECTX
  HWND outputWindow; //! used only in directX 
#elif OPEN_GL
#endif // DIRECTX
  uint32 buffWidth = 1;
  uint32 buffHeight = 1;
  int buffUsage = 0;
  int buffFormat = 0;
//! controls the refresh rate  buffNumaretor / buffDenominator
  uint16 buffRefershNumaretor = 60;
  uint16 buffRefershDenominator = 1;
  uint8 buffCount{ 0 };
  uint8 sampCount{ 0 };
  uint8 sampQuality{ 0 };
  bool isWindowd = true;
};

// TODO : convert to class
struct enTexture2D
{
  enTexture2D() = default;
  enTexture2D(const enTexture2D& other) = delete;
  enTexture2D(enTexture2D&& other) noexcept
    :m_interface(other.m_interface)
  {
  #if DIRECTX
    other.m_interface = nullptr;
  #endif // DIRECTX
  }

  ~enTexture2D()
  {
  #if DIRECTX
    RELEASE_DX_PTR(m_interface);
  #elif OPENGL
    m_interface = 0;
  #endif // DIRECTX
  }

#if DIRECTX
  ID3D11Texture2D* m_interface = nullptr;

#elif OPENGL
  int32 m_interface = 0;
#endif // DIRECTX

};

// TODO : convert to class
struct enRenderTargetView
{
  enRenderTargetView() = default;
  enRenderTargetView(const enRenderTargetView& other) = delete;
  enRenderTargetView(enRenderTargetView&& other) noexcept
    :m_interface(other.m_interface)
  {
  #if DIRECTX
    other.m_interface = nullptr;
  #elif OPENGL

  #endif // DIRECTX
  }

  ~enRenderTargetView()
  {
  #if DIRECTX
    RELEASE_DX_PTR(m_interface);
  #elif OPENGL
    m_interface = 0;
  #endif // DIRECTX
  }


#if DIRECTX
  ID3D11RenderTargetView* m_interface = nullptr;
#elif OPENGL
  int32 m_interface;
#endif // DIRECTX

  static constexpr uint32 c_randerTargetMax = 8;

  uint32 m_targetsCount = 0U;
  std::array<enTexture2D, c_randerTargetMax> m_targets{};
  std::array<bool, c_randerTargetMax > m_usedTargets{};
};

// TODO : convert to class
struct enDepthStencilView
{
  enDepthStencilView() = default;
  enDepthStencilView(const enDepthStencilView& other) = delete;
  enDepthStencilView(enDepthStencilView&& other) noexcept
    :m_interface(other.m_interface)
  {
  #if DIRECTX
    other.m_interface = nullptr;
  #endif // DIRECTX
  }

  ~enDepthStencilView()
  {
  #if DIRECTX
    RELEASE_DX_PTR(m_interface);
  #elif OPENGL
  #endif // DIRECTX
  };
#if DIRECTX
  ID3D11DepthStencilView* m_interface = nullptr;
  sDepthStencilDescriptor m_desc;
  enTexture2D m_texture;
#elif OPENGL
  int32 m_interface = 0;
#endif // DIRECTX
};



/*********/
/*********/

