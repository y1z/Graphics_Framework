#pragma once
#include "enMultiShader.h"
#include "en_defs.h"

#include <cstddef>
#include <memory>


class enRenderManager 
{
public: 

  enRenderManager();
  ~enRenderManager() = default;
public:

  /* *
  * @brief Choses a shader based on the variable "m_selectedShader"
  */
  void 
  setSelectedShader();

  void
  setShaderIndex(int32 index);

  int32
  getShaderIndex()const;

  int32
  getShaderCount()const;
  /**
  * @returns A pointer to the multi-shader.
  * @bug no known bugs.
  */
  enMultiShader*
  getMultiShaderPtr();
public:

  std::unique_ptr<enMultiShader> m_multiShader;

  int32 m_selectedShader;
};

