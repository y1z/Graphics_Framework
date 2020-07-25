#pragma once
#include "enPixelShader.h"
#include "enVertexShader.h"
#include "enShaderPathTracker.h"
#include "util/enRangeIndexTracker.h"
#include "en_defs.h"
#include <string>
#include <vector>
#include <memory>
#include <cstddef>


/**
* @brief Represents multiple version of the same shader
*/
class enMultiShader
{
public:// constructors 
  enMultiShader() = default;
  enMultiShader(enMultiShader const& other) = delete;
  enMultiShader(enMultiShader&& other)noexcept = default;
  ~enMultiShader() = default;
public: // sub-class/structs 

  struct sShaderPair
  {
    sShaderPair() = default;
    sShaderPair(sShaderPair && other) noexcept
      :m_vertexShader(other.m_vertexShader),
      m_pixelShader(other.m_pixelShader),
      m_shaderProgramIndex(other.m_shaderProgramIndex),
      isIndexCreated(other.isIndexCreated)
    {
      other.m_vertexShader = nullptr;
      other.m_pixelShader = nullptr;
      other.m_shaderProgramIndex = 0;
      other.isIndexCreated = false;
    };
    sShaderPair(const sShaderPair & other) = delete;
    ~sShaderPair()
    {
      SAFE_RELEASE(m_vertexShader);
      SAFE_RELEASE(m_pixelShader);
      m_shaderProgramIndex = 0;
    };
 
    enVertexShader* m_vertexShader = nullptr;
    enPixelShader* m_pixelShader = nullptr;
    std::size_t m_shaderProgramIndex = 0;
    bool isIndexCreated = false;
  };


  struct sShaderSourcePair
  {
    std::string m_vertexSource;
    std::string m_pixelSource;
  };

public:// operators 

  enMultiShader& 
  operator= (enMultiShader const& other) = delete;

  enMultiShader& 
  operator= (enMultiShader && other) noexcept = default;

public://methods 

  bool
  createEveryShaderVersion(enShaderPathTracker const & shaderTracker,
                          std::vector<std::string > const& definesForEachShader);
  

  enMultiShader::sShaderPair&
  getShaderPairPtr(std::size_t index);

  enVertexShader&
  getVertexShaderRef(std::size_t const index);

  enPixelShader&
  getPixelShaderRef(std::size_t const index);
  
private:
  /**
  * @brief Adds the defines to the shader.
  * @param[in] shaderTracker : Keeps track of every possible name for each file.
  * @param[in] definesForEachShader : 
  * @bug no known bugs.
  */
  std::vector<enMultiShader::sShaderSourcePair>
  createShaderWithDefines(enShaderPathTracker const& shaderTracker,
                          std::vector<std::string > const& definesForEachShader) const;

  /**
  * @brief compiles every version of the shader 
  * @param[in] shaderCode : All the code for each version of the shader.
  * @bug no known bugs.
  */
  enErrorCode
  compileEveryVersionOfShader(const std::vector<enMultiShader::sShaderSourcePair>& shaderCode);

  /**
  * @returns How many shaders the multiShader has.
  */
  std::size_t
  getShaderCount()const;

  void
  initShader(std::size_t index);

public:
  /**
  * @brief Sets a shader.
  * @param[in] index : which shader to use.
  * @bug no known bugs.
  */
  void
  setShader(std::size_t index);
  
public:// variables 
  /**
  * @brief Contains every vertex/pixel shader combo.
  */
  std::vector<sShaderPair> m_shaderPairs;

private:

  static constexpr std::size_t s_max = 40;

  static enRangeIndexTracker<uint32, s_max > s_trackedPrograms;
};

