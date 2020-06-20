#include "enShaderPathTracker.h"


enShaderPathTracker::enShaderPathTracker()
{
#if DIRECTX
  m_currenApi = CurrentApi::DirectX;
#elif OPENGL
  m_currenApi = CurrentApi::Open_Gl;
#endif
}

bool
enShaderPathTracker::init(std::string_view vertexShaderName,
                          std::string_view pixelShaderName)
{
  m_vertexShaderName = vertexShaderName;
  m_pixelShaderName = pixelShaderName;

  const char* vertexExtention = getVertexShaderExtension();
  const char* pixelExtention = getPixelShaderExtension();

  if( nullptr == vertexExtention ||
     nullptr == pixelExtention )
    return false;

  if( checkForExtension(vertexExtention, m_vertexShaderName) )
  {
    m_vertexShaderName.append(vertexExtention);
  }

  if( checkForExtension(pixelExtention, m_pixelShaderName) )
  {
    m_pixelShaderName.append(pixelExtention);
  }

  return true;
}

std::string const&
enShaderPathTracker::getVertexShaderName() const
{
  return m_vertexShaderName;
}

std::string const&
enShaderPathTracker::getPixelShaderName() const
{
  return m_pixelShaderName;
}

void
enShaderPathTracker::AddExtensions()
{
  const char* vertExtension = getVertexShaderExtension();
  if( nullptr == vertExtension )
    return ;

  if( m_vertexShaderName.find(vertExtension) == std::string::npos )
  {
    m_vertexShaderName.append(vertExtension);
  }

  const char* pixelExtension = getPixelShaderExtension();
  if( nullptr == pixelExtension )
    return ;

  if( m_pixelShaderName.find(pixelExtension) == std::string::npos )
  {
    m_pixelShaderName.append(pixelExtension);
  }

}

bool 
enShaderPathTracker::checkForExtension(std::string_view extension,
                                       std::string_view filePathToCheck) const
{
  return filePathToCheck.find(extension) == std::string::npos;
}

const char*
enShaderPathTracker::getVertexShaderExtension() const
{
  switch( m_currenApi )
  {
    case enShaderPathTracker::CurrentApi::DirectX:
    {
      return ".fx";
    }
    case enShaderPathTracker::CurrentApi::Open_Gl:
    {
      return ".vert";
    }
    default:
    break;
  }

  return nullptr;
}

const char*
enShaderPathTracker::getPixelShaderExtension() const
{
  switch( m_currenApi )
  {
    case enShaderPathTracker::CurrentApi::DirectX:
    {
      return ".fx";
    }
    case enShaderPathTracker::CurrentApi::Open_Gl:
    {

      return ".frag";
    }
    default:
    break;
  }
  return nullptr;
}
