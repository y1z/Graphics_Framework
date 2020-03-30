#pragma once
#include "util/Header.h"
#include <string_view>
#include <deque>

  /**
  * @brief : defines a view into a resource that can be used by the shader.
  */
class enShaderResourceView
{

public:
  enShaderResourceView() = default;
  enShaderResourceView(const enShaderResourceView & other) = delete;
  enShaderResourceView(enShaderResourceView && other) noexcept;
  ~enShaderResourceView();

public:

  bool 
  init();


  int32
  getIndex()const;

public:
  std::deque<std::string> m_resourcePaths;

  void *ptr_data = nullptr;
#if DIRECTX
  ID3D11ShaderResourceView* m_interface = nullptr;
#elif OPENGL
  int32 m_interface = 0u;
  #else
  void* m_interface = nullptr;
#endif // DIRECTX
  int32 m_index;

};

