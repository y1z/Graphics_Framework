#pragma once
#include "util/Header.h"
#include <array>

  /**
  * @brief : contains elements that don't clearly fit in the current structurer of api.
  * @bug : no known bugs.
  */
class cApiComponents 
{
public:

  /**
  * @returns a string that give information about the hardware:
  * @bug :
  */
  std::string 
  getHardwareInfo() const;

#if OPENGL

/*! returns a reference to the shader program
for open_gl */
  /**
  * @returns : the current shader program.
  * @bug : no known bugs.
  */
  static uint32*
  getShaderProgram();

  /**
  * @brief : creates a shader program for open_gl.
  * @returns : true if the index does not go out of range and the shader program
  * is not already being used.
  * @param[in] index : which of all shader programs to use.
  */
  static bool
  createProgram(size_t index);

  /**
  * @brief : gives the
  * @returns : true 
  * @bug : no known bugs.
  */
  static void 
  startupShaderPrograms();


  /**
  * @returns : the maximum possible amount of shader programs.
  * @bug : no known bugs.
  */
  static size_t 
  getMaxPrograms();


  /**
  * @returns : true if the shader program exist.
  * @param[in] programsToSet : which program is going to be set to.
  * @bug : no known bugs.
  */
  static bool
  setCurrentProgram(uint32 programsToSet);

  /**
  * @returns :a reference to the vertex array object 
  * @bug : no known bugs.
  */
  static uint32*
  getvertexArrayObject();

protected:
  constexpr static size_t s_MaxPrograms = 40u;

public:
  /**
  * @returns : the container of the shader programs.
  * @bug : no known bugs.
  */
  static const std::array<uint32,s_MaxPrograms>
  getProgramContianer();

#endif // OPEN_GL

  sHardWareInfo m_hardWareInfo;

#if DIRECTX
  //! used to know which version of the api is needed 
  D3D_FEATURE_LEVEL m_version;

#elif OPENGL

  static uint32 GlShaderProgram;

  static uint32 vertexArrayObject;

  /**
  * @brief : the container of the shader programs.
  */
  static std::array<uint32,s_MaxPrograms> s_shaderprograms;

  /**
  * @brief : the major number in the version of openGL.
  */
  int m_majorVersion;

  /**
  * @brief : the minor number in the version of openGL.
  */
  int m_minorVersion;
#endif
};

