#pragma once
#include "util/Header.h"

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
  * @returns :  the current shader program.
  * @bug : no known bugs.
  */
  static uint32*
  getShaderProgram();


  static void
  setCurrentProgram(uint32& currentProgram);

  /**
  * @returns :a reference to the vertex array object 
  * @bug : no known bugs.
  */
  static uint32*
  getvertexArrayObject();

#endif // OPEN_GL

  sHardWareInfo m_hardWareInfo;

public:
//selected
 size_t m_selectedProgram = 0;
private:
 constexpr static size_t s_MaxPrograms = 30u;
#if DIRECTX
  //! used to know which version of the api is needed 
  D3D_FEATURE_LEVEL m_version;

#elif OPENGL


  static uint32 GlShaderProgram;
  static uint32 vertexArrayObject;

  int m_majorVersion;
  int m_minorVersion;
#endif
};

