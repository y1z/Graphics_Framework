#pragma once
#include "util/Header.h"




class cApiComponents
{
public:


  bool
  init();
  /**
  * @returns a string that give information about the hardware:
  * @bug :
  */
  std::string 
  getHardwareInfo() const;

  /*! set the version of the api used
  \param [in] majorVersion keeps track of the bigger version changes of the api
  \param [in] minorVersion */
  void
  setSupportedVersion(int majorVersion, int minorVersion = 0);

  /*! set the hardware being used
  by the api */
  void
  setHardwareVersion(int HardwareVersion);

private:
#if DIRECTX
  //! used to know which version of the api is needed 
  D3D_FEATURE_LEVEL m_version;

#elif OPEN_GL
  static uint32 GlShaderProgram;
  static uint32 vertexArrayObject;
  int m_majorVersion;
  int m_minorVersion;
#endif
};

