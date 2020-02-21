#pragma once
#include "enBaseShader.h"

  /**
  * @brief :
  * @bug :
  */
class enVertexShader final :
  public enBaseShader
{
public :
  enVertexShader() = default;
  enVertexShader(const enVertexShader& other) = delete;
  enVertexShader(enVertexShader&& other) noexcept;
  ~enVertexShader();
public:

#if DIRECTX
  ID3D11VertexShader*
  getInterface();

  ID3D11VertexShader**
  getInterfaceRef();
#elif OPENGL

#endif // DIRECTX


private:
#if DIRECTX
  ID3D11VertexShader* m_interface = nullptr;
#elif OPENGL
  int32  m_interface = 0;
#endif // DIRECTX
};


