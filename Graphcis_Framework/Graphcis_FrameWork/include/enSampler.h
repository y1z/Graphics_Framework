#pragma once
#include "util/Header.h"
#include <algorithm>

  /**
  * @brief :determines how the image on your screen is going
  * to be processed
  * @bug :
  */
class enSampler
{
public:
  enSampler() = default;
  ~enSampler()
  {
  #if DIRECTX
    RELEASE_DX_PTR(m_interface);
  #elif OPENGL
  #endif // DIRECTX
  };

public:
#if DIRECTX
  //! for interfacing with directX 
  ID3D11SamplerState* getInterface();
  //! for arguments that require 2 pointers 
  ID3D11SamplerState** getInterfaceRef();
#endif // DIRECTX

    void
    init(uint32_t filter,

         uint32_t addressU,
         uint32_t addressV,
         uint32_t addressW,
         int comparasionFunc,
         uint32_t maxAnisotropy = 1,
         float minLod = 0.0f,
         float maxLod = std::numeric_limits<float>::max());

  void
  setBorderColor(float red,

                 float green,
                 float blue,
                 float alpha = 1.0f);

#if DIRECTX
  D3D11_SAMPLER_DESC
    getDirectxSamplerDesc();

#endif // DIRECTX


public:
  sSamplerDesc m_desc;
#if DIRECTX
  ID3D11SamplerState* m_interface = nullptr;
#elif OPENGL
  int32 m_interface = 0;
#endif // DIRECTX
};

