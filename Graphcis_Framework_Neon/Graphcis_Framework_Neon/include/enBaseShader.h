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
  compileShaderFromFile(std::string_view const PathToShaderFile,
                        std::string_view const EntryPoint,
                        std::string_view const ShaderModel);

  enErrorCode
  compileShader(std::string const& ShaderSource,
                 std::string_view const& EntryPoint,
                 std::string_view const& ShaderModel);


#if DIRECTX
  ID3DBlob * 
  getShaderInfo();


  ID3DBlob **
  getShaderInfoRef();
#elif OPENGL

  void
  setProgramPtr(uint32* ptr);

  uint32*
  getProgramPtr();

  uint32
  getShaderInfo();

  uint32*
  getShaderInfoPtr();

  uint32&
  getShaderInfoRef();

  /**
  * @brief : the object/ value used to interact with the open-gl api.
  */
  uint32
  getinterface();

  /**
  * @brief : a pointer to the object/ value used to interact with the open-gl api.
  */
  uint32*
  getinterfacePtr();

  /**
  * @brief : a reference to the object/ value used to interact with the open-gl api.
  */
  uint32&
  getinterfaceRef();

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
  uint32 m_infoOfShader = std::numeric_limits<uint32>::max();
  uint32 m_interface = std::numeric_limits<uint32>::max();
  uint32* m_programPtr = nullptr ;
#endif // DIRECTX
protected:
  enShaderTypes m_type = enShaderTypes::noType;
};

