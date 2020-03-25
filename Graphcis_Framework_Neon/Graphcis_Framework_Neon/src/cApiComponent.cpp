#include "..\include\cApiComponent.h"
#include "enDevice.h"

#if OPENGL
uint32 cApiComponents::GlShaderProgram = 0;
uint32 cApiComponents::vertexArrayObject = 0;
#endif//OPEN_GL

std::string
cApiComponents::getHardwareInfo() const
{
  std::string result("version : ");
#if DIRECTX
  enDevice& device = enDevice::getInstance();
  D3D_FEATURE_LEVEL FeatureLevel = device.getInterface()->GetFeatureLevel();

  switch( FeatureLevel )
  {
    case D3D_FEATURE_LEVEL_11_0:
    result = GENERIC_CHAR(D3D_FEATURE_LEVEL_11_0);
      break;
    case D3D_FEATURE_LEVEL_11_1:
    result = GENERIC_CHAR(D3D_FEATURE_LEVEL_11_1);
      break;
    case D3D_FEATURE_LEVEL_12_0:
    result = GENERIC_CHAR(D3D_FEATURE_LEVEL_12_0);
      break;
    case D3D_FEATURE_LEVEL_12_1:
    result = GENERIC_CHAR(D3D_FEATURE_LEVEL_12_1);
      break;
    default:
    result = "NOT SUPPORTED";
      break;
  }

#elif OPENGL

#endif // DIRECTX


  return  result;
}




#if OPENGL

uint32*
cApiComponents::getShaderProgram()
{
  return &cApiComponents::GlShaderProgram;
}

void
cApiComponents::setCurrentProgram(uint32& currentProgram)
{
  GlShaderProgram = currentProgram;
}


uint32*
cApiComponents::getvertexArrayObject()
{
  return &cApiComponents::vertexArrayObject;
}

#endif // OPEN_GL
