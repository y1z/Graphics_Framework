#pragma once
#include <filesystem>

  /**
  * @brief Keep track of the file Names for each type of shader and automatically add the 
  * appropriate extensions.
  */
class enShaderPathTracker
{
public:

  enShaderPathTracker();
  enShaderPathTracker(const enShaderPathTracker& other) = default;
  enShaderPathTracker(enShaderPathTracker&& other) = default;
  ~enShaderPathTracker() = default;
public:
  /**
  * @brief Take 2 file names and add the appropriate file extensions depending
  * on what type they are, in DirectX it's ".fx" while in open-gl it's ".vert" and ".frag".
  * @bug no known bugs.
  * @param[in] vertexShaderName : The name of the Vertex shader.
  * @param[in] pixelShaderName : ThE name of the pixel shader.
  */
  bool
  init(std::string_view vertexShaderName,
       std::string_view pixelShaderName);

  /**
  * @returns A reference to the string that contains the vertex-shader file name.
  * @bug should first use 'init' function before calling.
  */
  std::string const&
  getVertexShaderName()const;

  /**
  * @returns A reference to the string that contains the vertex-shader file name.
  * @bug should first use 'init' function before calling.
  */
  std::string const&
  getPixelShaderName()const;

private:

  /**
  * @brief Add File extensions 
  * @bug
  */
  void 
  AddExtensions();

  /**
  * @brief Checks if the file names already have the appropriate extensions
  * @param extension : Which extension to check for.
  * @bug no known bugs.
  */
  bool
  checkForExtension(std::string_view extension,
                    std::string_view filePathToCheck)const;

  

  /**
  * @returns The extension for a vertex shader (".fx" or ".vert" )
  */
  const char*
  getVertexShaderExtension()const;

  /**
  * @returns The extension for a pixel shader (".fx" or ".frag" )
  */
  const char*
  getPixelShaderExtension()const;

  /**
  * @brief  Controls which extensions the files will receive.
  */
  enum class CurrentApi
  {
    DirectX,
    Open_Gl
  };
private:

  std::string m_vertexShaderName;

  std::string m_pixelShaderName;
  /**
  * @brief  keeps track of which api is being used.
  */
  CurrentApi m_currenApi;
private:
};

