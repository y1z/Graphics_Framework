#pragma once
#include "util/Header.h"
#include <string>
#include <deque>
#include <array>
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

  /**
  * @brief : initializes a shader-resource-view.
  * @returns : true if the initialization succeeded
  * @bug : no known bugs.
  */
  bool 
  init(const sShaderResourceDesc &descriptor);

  /**
  * @brief :initializes a shader-resource-view automatically through
  * a series of checks.
  * @returns : true when the function succeeded false otherwise.
  * @bug : no known bugs.
  */
  bool
  autoInit();

  /**
  * @returns : the index of the shader resource.
  * @bug :no known bugs.
  */
  int32
  getIndex()const;

private:
  /**
  * @brief :
  * @bug :
  */
  static void
  registerIndexAsBeingUsed(int32 index);

  /**
  * @returns : a index thats not used.
  * @bug : no known bugs.
  */
  static int32 
  getLeatestUnusedIndex();

  /**
  * @returns : true when a index is already being used false otherwise.
  * @bug : no known bugs.
  */
  static bool
  checkIfIndexIsAlreadyUsed(int32 index);

private:
  constexpr static size_t s_maxIndexes = 11u;

  static std::array<sSignedIndexTracker , s_maxIndexes> s_indexes; 


public:

  /**
  * @brief : contains all data related with the shader Resource View
  */
  sShaderResourceDesc m_descriptor;

#if DIRECTX
  ID3D11ShaderResourceView* m_interface = nullptr;
#elif OPENGL
  uint32 m_interface = std::numeric_limits<uint32>::max();
#else
  void* m_interface = nullptr;
#endif // DIRECTX
};

