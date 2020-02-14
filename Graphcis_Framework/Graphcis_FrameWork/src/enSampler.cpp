#include "enSampler.h"
#include "..\include\enSampler.h"
#if DIRECTX

ID3D11SamplerState*
enSampler::getInterface()
{
  return m_interface;
}

ID3D11SamplerState**
enSampler::getInterfaceRef()
{
  return &m_interface;
}


#endif // DIRECTX


void
enSampler::init(uint32_t filter,
                uint32_t addressU,
                uint32_t addressV,
                uint32_t addressW,
                int comparasionFunc,
                uint32_t maxAnisotropy,
                float minLod,
                float maxLod)
{
  m_desc.filter = filter;
  m_desc.addressU = addressU;
  m_desc.addressV = addressV;
  m_desc.addressW = addressW;
  m_desc.comparingFunc = comparasionFunc;
  m_desc.AnisotropicLevel = maxAnisotropy;
  m_desc.minLod = minLod;
  m_desc.maxLod = maxLod;

}

void
enSampler::setBorderColor(float red,
                          float green,
                          float blue,
                          float alpha)
{
  m_desc.boderColor[0] = red;
  m_desc.boderColor[1] = green;
  m_desc.boderColor[2] = blue;
  m_desc.boderColor[3] = alpha;
}
#if DIRECTX

D3D11_SAMPLER_DESC
enSampler::getDirectxSamplerDesc()
{
  D3D11_SAMPLER_DESC Result;
  std::memset(&Result, 0, sizeof(Result));
  Result.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_desc.addressU);
  Result.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_desc.addressV);
  Result.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_desc.addressW);
  Result.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(m_desc.comparingFunc);
  Result.Filter = static_cast<D3D11_FILTER>(m_desc.filter);
  Result.MaxAnisotropy = m_desc.AnisotropicLevel;
  Result.MaxLOD = m_desc.maxLod;
  Result.MinLOD = m_desc.minLod;
  for( uint8_t i = 0; i < 4; ++i )
  {
    Result.BorderColor[i] = m_desc.boderColor[i];
  }
  return Result;
}

#endif // DIRECTX
