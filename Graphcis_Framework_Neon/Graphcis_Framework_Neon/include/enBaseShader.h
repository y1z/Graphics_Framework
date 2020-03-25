#pragma once
#include "util/Header.h"
#include <string>
  /**
  * @brief : used to store all necessary information that every shader 
  *          has.
  * @bug :
  */
class enBaseShader
{
public:
  enBaseShader() = default;

  enBaseShader(const enBaseShader& other) = delete;

  enBaseShader(enBaseShader&& other) noexcept;

  virtual ~enBaseShader();

  /**
  * @brief : compiles a shader from a file path.
  * @param[in] PathToShaderFile : the path to the file.
  * @param[in] EntryPoint : the main function for the directX shader.
  * @param[in]  ShaderModel : which version of the shader model directX is using.
  * @bug :no known bugs.
  */
  enErrorCode
  compileShaderFromFile(std::string_view PathToShaderFile,
                        std::string_view EntryPoint,
                        std::string_view ShaderModel);

#if DIRECTX
  ID3DBlob * 
  getShaderInfo();


  ID3DBlob **
  getShaderInfoRef();
#endif // DIRECTX

public:
  /**
  * @brief : used to contain the source code of the shader
  */
  std::string m_shader;

  /**
  * @brief : used to know where the main function is for the 
  *          shader (only necessary in directX)
  */
  std::string m_entryPoint;

  /**
  * @brief : used to know which shader model we are using 
  *          only useful in directX.
  */
  std::string m_shaderModel;

 private:
#if DIRECTX
  ID3DBlob* m_infoOfShader = nullptr;
#elif OPENGL
// TODO : make getter function
  uint32 m_infoOfShader = 0u;
#endif // DIRECTX
};

