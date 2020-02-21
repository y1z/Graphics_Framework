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

  /*************/
#if 0
  static void
    makeMaze(ID3D11Buffer& cubeBuffer,
             enMatrix4x4& cubeMatrix,
             ID3D11DeviceContext& dContext)
  {
    static  std::vector<enVector3> translations =
    {
     {enVector3(0.0f,0.0f,2.0f)},{enVector3(0.0f,0.0f,2.0f)},
     {enVector3(0.0f,0.0f,2.0f)},{enVector3(0.0f,0.0f,2.0f)},
     {enVector3(0.0f,0.0f,2.0f)},

     {enVector3(-2.0f,0.0f,0.0f)},{enVector3(-2.0f,0.0f,0.0f)},
     {enVector3(-2.0f,0.0f,0.0f)},{enVector3(-2.0f,0.0f,0.0f)},
     {enVector3(-2.0f,0.0f,0.0f)},{enVector3(-2.0f,0.0f,0.0f)},

     {enVector3(0.0f,0.0f,2.0f)},{enVector3(0.0f,0.0f,2.0f)},
     {enVector3(0.0f,0.0f,2.0f)},{enVector3(0.0f,0.0f,2.0f)},
     {enVector3(0.0f,0.0f,2.0f)},{enVector3(0.0f,0.0f,2.0f)},
     {enVector3(0.0f,0.0f,2.0f)},


     {enVector3(2.0f,0.0f,0.0f)},{enVector3(2.0f,0.0f,0.0f)},
     {enVector3(2.0f,0.0f,0.0f)},{enVector3(2.0f,0.0f,0.0f)},
     {enVector3(2.0f,0.0f,0.0f)},


     {enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},
     {enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},


     {enVector3(2.0f,0.0f,0.0f)},{enVector3(2.0f,0.0f,0.0f)},
     {enVector3(2.0f,0.0f,0.0f)},{enVector3(2.0f,0.0f,0.0f)},
     {enVector3(2.0f,0.0f,0.0f)},{enVector3(2.0f,0.0f,0.0f)},
     {enVector3(2.0f,0.0f,0.0f)},{enVector3(2.0f,0.0f,0.0f)},


     {enVector3(0.0f,0.0f,2.0f)},{enVector3(0.0f,0.0f,2.0f)},
     {enVector3(0.0f,0.0f,2.0f)},{enVector3(0.0f,0.0f,2.0f)},
     {enVector3(0.0f,0.0f,2.0f)},{enVector3(0.0f,0.0f,2.0f)},
     {enVector3(0.0f,0.0f,2.0f)},{enVector3(0.0f,0.0f,2.0f)},
     {enVector3(0.0f,0.0f,2.0f)},{enVector3(0.0f,0.0f,2.0f)},


     {enVector3(0.0f,0.0f,2.0f * 4.0f)},


     {enVector3(2.0f,0.0f,0.0f)},{enVector3(2.0f,0.0f,0.0f)},
     {enVector3(2.0f,0.0f,0.0f)},{enVector3(2.0f,0.0f,0.0f)},
     {enVector3(2.0f,0.0f,0.0f)},


     {enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},
     {enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},
     {enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},
     {enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},
     {enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},
     {enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},


     {enVector3(-2.0f,0.0f,0.0f)},{enVector3(-2.0f,0.0f,0.0f)},
     {enVector3(-2.0f,0.0f,0.0f)},{enVector3(-2.0f,0.0f,0.0f)},
     {enVector3(-2.0f,0.0f,0.0f)},{enVector3(-2.0f,0.0f,0.0f)},


     {enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},
     {enVector3(0.0f,0.0f,-2.0f)},{enVector3(0.0f,0.0f,-2.0f)},

    };


    CBChangesEveryFrame EveryFrame;
    EveryFrame.vMeshColor = enVector4(0.6f, 0.6f, 0.6f, 0.6f);

    enMatrix4x4 scaleMatrixInY(1.0f);
    scaleMatrixInY = glm::row(scaleMatrixInY, 1, enVector4(0.0f, 5.0f, 0.0f, 0.0f));
    cubeMatrix *= scaleMatrixInY;
    for( const enVector3& vector : translations )
    {
      cubeMatrix = glm::translate(cubeMatrix, vector);
      EveryFrame.mWorld = glm::transpose(cubeMatrix);


      dContext.UpdateSubresource(&cubeBuffer,
                                 0,
                                 NULL,
                                 &EveryFrame,
                                 0,
                                 0);

      dContext.DrawIndexed(36, 0, 0);
    }

    cubeMatrix = glm::identity<enMatrix4x4>();

  }
#endif 
}

