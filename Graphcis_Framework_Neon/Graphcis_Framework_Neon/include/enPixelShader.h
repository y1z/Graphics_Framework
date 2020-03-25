#pragma once
#include "enBaseShader.h"

class enPixelShader :
  public enBaseShader
{
public:
  enPixelShader() = default;
  enPixelShader(const enPixelShader& other ) = delete;
  enPixelShader( enPixelShader&& other );
  ~enPixelShader();
public:
#if DIRECTX

  ID3D11PixelShader*
  getInterface();

  ID3D11PixelShader**
  getInterfaceRef();

#endif // DIRECTX

private:
#if DIRECTX
  ID3D11PixelShader* m_interface = nullptr;
#elif OPENGL
  int32 m_interface = 0;
#endif // DIRECTX

};

