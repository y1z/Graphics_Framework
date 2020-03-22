#include "..\include\enMultiviewTexture.h"
#include "enDevice.h"
#include "enDeviceContext.h"
#include  "helperFucs.h"


enErrorCode
enMultiviewTexture::CreateDepthStancil(float width,
                                       float height)
{
  sTextureDescriptor textureForDepthStencil = helper::generateDepthStencilDesc(width, height);
  enDevice& device = enDevice::getInstance();

  bool isSuccessful = device.CreateTexture2D(textureForDepthStencil, m_sharedTexture);
  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return enErrorCode::FailedCreation;
  }

  sDepthStencilDescriptor DepthStencilDesc;
  DepthStencilDesc.Format = textureForDepthStencil.texFormat;
  DepthStencilDesc.Dimension = DepthStencilFormat::two_dimention;
  DepthStencilDesc.Mip = 0;

  m_depthView.m_desc = DepthStencilDesc;
  isSuccessful = device.CreateDepthStencilView(m_depthView,m_sharedTexture);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return enErrorCode::FailedCreation;
  }

  return enErrorCode::NoError;
}

enErrorCode 
enMultiviewTexture::CreateRenderTarget(float width,
                                       float height)
{
  sTextureDescriptor textureForRenderTarget = helper::generateRenderTargetDesc(width, height);
  enDevice& device = enDevice::getInstance();

  bool isSuccessful = device.CreateTexture2D(textureForRenderTarget, m_sharedTexture);
  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return enErrorCode::FailedCreation;
  }

  sRenderTargetDesc2D renderTargetDesc;
  renderTargetDesc.format = textureForRenderTarget.texFormat;


  return enErrorCode::NoError;
}
