#include "..\include\enPixelShader.h"

enPixelShader::enPixelShader(enPixelShader&& other)
:enBaseShader(std::forward<enBaseShader>(other))
{
#if DIRECTX
  this->m_interface = other.m_interface;
  other.m_interface = nullptr;
#endif // DIRECTX
}

enPixelShader::~enPixelShader()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface)
#endif // DIRECTX
}



#if DIRECTX

ID3D11PixelShader*
enPixelShader::getInterface()
{
  return m_interface;
}

ID3D11PixelShader**
enPixelShader::getInterfaceRef()
{
  return &m_interface;
}

#endif // DIRECTX

