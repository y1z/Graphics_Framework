#pragma once
#include <cstdint>
#include <cstddef>
#include <iostream>
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

/**
*@brief used to determine the error that was committed
*/
enum class enErrorCode :Byte4
{
  Sucessful = 0b000'0000'0000'0000'0000'0000'0000'0000, //!<  indicates that no error occurred  
  UnClassified = 0b000'0000'0000'0000'0000'0000'0000'0001,//!<  indicates that there is no-know name for the error 
  NotReady = 0b000'0000'0000'0000'0000'0000'0000'0010, //!<  means that the function/class/object needed more set up before it's ready to be used  
  InvalidPath = 0b000'0000'0000'0000'0000'0000'0000'0100,//!< means that a provided path is invalid 
  ShaderComplieError = 0b000'0000'0000'0000'0000'0000'0000'1000,//!< means that an error occurred with compiling the shader
  ShaderLinkError = 0b000'0000'0000'0000'0000'0000'0001'0000, //!< means that a provided path 
  ActorComponentError = 0b000'0000'0000'0000'0000'0000'0010'0000,//!< means that a provided path 
  FailedCreation = 0b000'0000'0000'0000'0000'0000'0100'0000,//!<means that something was not created correctly.
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

    switch (errorCode)
    {
    case enErrorCode::Sucessful:
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
}

/*++++++++++++++++++++++++++++++++++++*/
/* #define's  */
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
#define EN_LOG_ERROR_WITH_CODE (ErrorCode);
#define EN_LOG_DB(message);
#endif // !NDEBUG

#define LONG_CHAR(Message) L##Message
#define GENERIC_CHAR(Message) #Message

#if _UNICODE || UNICODE
#define EN_CHAR(Message) LONG_CHAR(Message)

#else 
#define EN_CHAR(Message) GENERIC_CHAR(Message)

#endif// _UNICODE


