#pragma once
#include "util/Header.h"
#include <string_view>
#include <deque>


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
#endif // DIRECTX
  int32 m_index;

};

