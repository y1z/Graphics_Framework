#include "..\include\cApiComponent.h"
#include "enDevice.h"

#if OPENGL
uint32 cApiComponents::GlShaderProgram = 0;
uint32 cApiComponents::vertexArrayObject = 0;
std::array<uint32,cApiComponents::s_MaxPrograms> cApiComponents::s_shaderprograms{};
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


bool
cApiComponents::createProgram(size_t index)
{
  bool const isInRange = (s_shaderprograms.size() > index);
  if( isInRange && (s_shaderprograms[index] == std::numeric_limits<uint32>::max()) )
  {
    s_shaderprograms[index] = glCreateProgram();
    GlShaderProgram = s_shaderprograms[index];
    return true;
  }

  return false;
}

void
cApiComponents::startupShaderPrograms()
{
  for( auto& val : s_shaderprograms )
  {
    val = std::numeric_limits<uint32>::max();
  }
}

size_t
cApiComponents::getMaxPrograms()
{
  return s_shaderprograms.size();
}


bool
cApiComponents::setCurrentProgram(uint32 programsToSet)
{
  for( uint32& val : s_shaderprograms )
  {
    if( val == programsToSet )
    {
      GlShaderProgram = val;
      return true;
    }
  }
  return false;
}


uint32*
cApiComponents::getvertexArrayObject()
{
  return &cApiComponents::vertexArrayObject;
}

const std::array<uint32, cApiComponents::s_MaxPrograms>
cApiComponents::getProgramContianer()
{
  return s_shaderprograms;
}

#endif // OPEN_GL
