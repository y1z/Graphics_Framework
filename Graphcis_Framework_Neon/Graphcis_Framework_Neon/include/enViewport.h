#pragma once
#include "util/Header.h"

  /**
  * @brief : describes the view port
  * @bug :
  */
class enViewport
{
public:
enViewport() = default;
~enViewport() = default;

  void
  init(const sViewportDesc& viewportDescriptor);
#if DIRECTX

  D3D11_VIEWPORT 
  getDxViewport()const;
#endif // DIRECTX

  uint32 
  getWidth() const;

  uint32 
  getHeight() const;

  sViewportDesc m_descriptor;
};
