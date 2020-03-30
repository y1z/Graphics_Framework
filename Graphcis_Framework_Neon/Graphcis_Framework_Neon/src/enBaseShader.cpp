#include "..\include\enBaseShader.h"
#include "helperFucs.h"
#include "cApiComponent.h"


enBaseShader::enBaseShader(enBaseShader&& other) noexcept
  :m_shader(std::move(other.m_shader)),
  m_entryPoint(std::move(other.m_entryPoint)),
  m_shaderModel(std::move(other.m_shaderModel)),
  m_type(other.m_type)
{
#if DIRECTX
  this->m_infoOfShader = other.m_infoOfShader;
  other.m_infoOfShader = nullptr;
#elif OPENGL
  this->m_infoOfShader = other.m_infoOfShader;
  other.m_infoOfShader = std::numeric_limits<uint32>::max();
#endif // DIRECTX
}

enBaseShader::~enBaseShader()
{
#if DIRECTX
  RELEASE_DX_PTR(m_infoOfShader);
#elif OPENGL
#endif // DIRECTX
};

enErrorCode
enBaseShader::compileShaderFromFile(std::string_view PathToShaderFile,
                                    std::string_view EntryPoint,
                                    std::string_view ShaderModel)
{
  m_shader = (helper::loadFileToString(PathToShaderFile));
  m_entryPoint = (EntryPoint);
  m_shaderModel = (ShaderModel);
  if( !m_shader.compare("Error") )
  {
    EN_LOG_DB("did not managed to load the shader");
    return enErrorCode::InvalidPath;
  }
#if DIRECTX
  uint32 dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  #if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
  #endif

  ID3DBlob* pErrorBlob = nullptr;

  HRESULT hr = D3DCompile(m_shader.c_str(),
                          m_shader.length(),
                          nullptr,
                          nullptr,
                          nullptr,
                          m_entryPoint.c_str(),
                          m_shaderModel.c_str(),
                          dwShaderFlags,
                          0,
                          &this->m_infoOfShader,
                          &pErrorBlob);

  if( FAILED(hr) )
  {
    if( pErrorBlob != NULL )
    {
      std::cerr << (char*)pErrorBlob->GetBufferPointer();
    }
    if( pErrorBlob ) pErrorBlob->Release();
    return  enErrorCode::ShaderComplieError;
  }
  if( pErrorBlob ) pErrorBlob->Release();

  return enErrorCode::NoError;
#elif OPENGL
  GlRemoveAllErrors();
  uint32* shaderProgram = cApiComponents::getShaderProgram();
  uint32_t shaderType = 0u;

  if( m_type == enShaderTypes::vertexType )
  {
    shaderType = GL_VERTEX_SHADER;
  }
  else if( m_type == enShaderTypes::pixelType )
  {
    shaderType = GL_FRAGMENT_SHADER;
  }

  m_infoOfShader = glCreateShader(shaderType);

  if( GlCheckForError() )
  {
    EN_LOG_ERROR;
    return enErrorCode::FailedCreation;
  }

  const char* refToSource = m_shader.c_str();
  glShaderSource(m_infoOfShader, 1, &refToSource, nullptr);
  glCompileShader(m_infoOfShader);

  int Result = 0;
  glGetShaderiv(m_infoOfShader, GL_COMPILE_STATUS, &Result);
  // how long is the error message 
  if( Result == GL_FALSE )
  {
    int MessageSize;
    glGetShaderiv(m_infoOfShader, GL_INFO_LOG_LENGTH, &MessageSize);

    char* ptr_message = new char[MessageSize + 1];
    glGetShaderInfoLog(m_infoOfShader, 2048, &MessageSize, ptr_message);

    std::cerr << ptr_message << std::endl;
    delete[] ptr_message;
    return enErrorCode::ShaderLinkError;
  }

  return enErrorCode::NoError;
#endif // DIRECTX

  return enErrorCode::UnClassified;
}


#if DIRECTX
ID3DBlob*
enBaseShader::getShaderInfo()
{
  return m_infoOfShader;
}

ID3DBlob**
enBaseShader::getShaderInfoRef()
{
  return &m_infoOfShader;
}
#elif OPENGL


uint32
enBaseShader::getShaderInfo()
{
  return m_infoOfShader;
}

uint32*
enBaseShader::getShaderInfoPtr()
{
  return &m_infoOfShader;
}

uint32&
enBaseShader::getShaderInfoRef()
{
  return m_infoOfShader;
}


uint32
enBaseShader::getinterface()
{
  return m_interface;
}

uint32*
enBaseShader::getinterfacePtr()
{
  return &m_interface;
}

uint32&
enBaseShader::getinterfaceRef()
{
  return m_interface;
}

#endif // DIRECTX

