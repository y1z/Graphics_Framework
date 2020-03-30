#include "enVertexShader.h"

enVertexShader::enVertexShader()
:enBaseShader()
{
  m_type = enShaderTypes::vertexType;
}

enVertexShader::enVertexShader(enVertexShader&& other) noexcept
:enBaseShader(std::forward<enBaseShader>(other))
{
#if DIRECTX
 this->m_interface = other.m_interface;
 other.m_interface = nullptr;
#elif OPENGL
#endif // DIRECTX
}

enVertexShader::~enVertexShader()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface)
#elif OPENGL
#endif // DIRECTX
}


#if DIRECTX
ID3D11VertexShader*
enVertexShader::getInterface()
{
  return m_interface;
}

ID3D11VertexShader**
enVertexShader::getInterfaceRef()
{
  return &m_interface;
}
#endif // DIRECTX
