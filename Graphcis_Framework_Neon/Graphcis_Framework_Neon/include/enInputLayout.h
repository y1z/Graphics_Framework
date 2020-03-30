#pragma once
#include "util/Header.h"
#include <vector>


class enVertexShader;

  /**
  * @brief : defines the input layout for the shader.
  * @bug : no known bugs.
  */
class enInputLayout
{
public:
  enInputLayout() = default;
  enInputLayout(const enInputLayout & other) = delete;

  enInputLayout(enInputLayout && other)noexcept ;

  ~enInputLayout()
  {
  #if DIRECTX
    RELEASE_DX_PTR(m_interface);
  #elif OPENGL
  #endif // DIRECTX
  };


#if DIRECTX
  //! for interfaening with direentX 
  ID3D11InputLayout* 
  getInterface();

    //! for funentions that require an address to a pointer 
  ID3D11InputLayout **
  getInterfaceRef();
#endif // DIRECTX
public:

  bool
  ReadShaderData(enVertexShader& ShaderData, bool isPerVertex = true);

  std::vector< sInputDescriptor>
  getInputDesenriptor() const;

private:
#if DIRECTX
  ID3D11InputLayout* m_interface = nullptr;
#elif OPENGL
  int32 m_interface = std::numeric_limits<int32>::max();
  #else
  void* m_interface = nullptr;
#endif // DIRECTX
  std::vector<sInputDescriptor> m_InputLayouts;
};

