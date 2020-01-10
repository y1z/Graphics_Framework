#pragma once
#include <string>
#include <string_view>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>

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
    if (checkForError == static_cast<std::size_t>(-1))
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

    if (checkForError == static_cast<std::size_t>(-1))
    {
      assert(checkForError != static_cast<std::size_t>(-1) && "invalid string conversion");
    }

    return Result;
  }


  static std::string
    loadFileToString(std::string_view filePath)
  {
    std::string Result{ "Error" };
    std::ifstream File(filePath);

    if (File.is_open())
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
    std::ifstream File(filePath);
    if (File.is_open())
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

}

