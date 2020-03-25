#include "..\include\enBaseShader.h"
#include "helperFucs.h"


enBaseShader::enBaseShader(enBaseShader&& other) noexcept
:m_shader(std::move(other.m_shader)),
m_entryPoint(std::move(other.m_entryPoint)),
m_shaderModel(std::move(other.m_shaderModel))
{
#if DIRECTX
  this->m_infoOfShader = other.m_infoOfShader;
  other.m_infoOfShader = nullptr;
#elif OPENGL
  this->m_infoOfShader = other.m_infoOfShader;
  other.m_infoOfShader = 0;
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
#if DIRECTX
  uint32 dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
  std::wstring FilePath(helper::convertStringToWString( PathToShaderFile ));

  m_shader = (helper::loadFileToString(PathToShaderFile));

  if( !m_shader.compare("Error") )
  {
    EN_LOG_DB("did not managed to load the shader");
    return enErrorCode::InvalidPath;
  }

  m_entryPoint = (EntryPoint);
  m_shaderModel = (ShaderModel);

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

#endif // DIRECTX

