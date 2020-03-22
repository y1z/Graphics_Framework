#pragma once
#include "util/Header.h"
#include <vector>


class enVertexShader;


class enInputLayout
{
public:
  enInputLayout() = default;
  enInputLayout(const enInputLayout & other) = delete;

  enInputLayout(enInputLayout && other)noexcept 
    :m_interface(other.m_interface)
  {
  #if DIRECTX 
    other.m_interface = nullptr;
    #elif OPENGL
    #else
    other.m_interface = nullptr;
  #endif // DIRECTX
  }
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
  int32 m_interface = 0;
  #else
  void* m_interface = nullptr;
#endif // DIRECTX
  std::vector<sInputDescriptor> m_InputLayouts;
};

