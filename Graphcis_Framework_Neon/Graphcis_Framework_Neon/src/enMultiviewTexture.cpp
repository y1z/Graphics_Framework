#include "..\include\enMultiviewTexture.h"
#include "enDevice.h"
#include "enDeviceContext.h"
#include "helperFucs.h"
#include "util/helperTemplates.h"

using namespace helper;

enErrorCode
enMultiviewTexture::CreateDepthStencil(float width,
                                       float height, 
                                       int Format)
{
  enDevice& device = enDevice::getInstance();

  sDepthStencilDescriptor DepthStencilDesc;
  DepthStencilDesc.Format = Format;
  DepthStencilDesc.Dimension = DepthStencilFormat::two_dimention;
  DepthStencilDesc.Mip = 0;

  m_depthView.m_desc = DepthStencilDesc;
  
  bool const isSuccessful = device.CreateDepthStencilView(m_depthView,
                                                          m_sharedTexture);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return enErrorCode::FailedCreation;
  }

  return enErrorCode::NoError;
}

enErrorCode
enMultiviewTexture::CreateRenderTarget(float width,
                                       float height,
                                       int Format)
{
  enDevice& device = enDevice::getInstance();

  sRenderTargetDesc2D renderTargetDesc;
  renderTargetDesc.format = Format;
  renderTargetDesc.renderTargetType = enRenderTargetViewType::renderTarget2D;
  renderTargetDesc.mip = 0;

  bool const isSuccessful = device.CreateRenderTargetView(m_renderView,
                                                          m_sharedTexture,
                                                          renderTargetDesc);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return enErrorCode::FailedCreation;
  }

  return enErrorCode::NoError;
}


enErrorCode 
enMultiviewTexture::CreateShaderResource(float width, float height, int Format)
{

  enDevice& device = enDevice::getInstance();
  bool const isSuccessful = device.CreateShaderResourceFromTexture(*m_shaderResource,
                                                                   m_sharedTexture);

  if( !isSuccessful )
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::FailedCreation);
    return enErrorCode::FailedCreation;
  }



  return  enErrorCode::NoError;
}

bool
enMultiviewTexture::CreateAll(float width,
                              float height,
                              int format,
                              enBufferUse usage,
                              enMultiViewType typeOfView)
{
  m_type = typeOfView;
  sTextureDescriptor descriptor;
  descriptor.texWidth = width;
  descriptor.texHeight = height;
  descriptor.texFormat = format;
  descriptor.arraySize = 1;
  descriptor.CpuAccess = 0;
  descriptor.BindFlags = this->getBindingFlagsBasedOnMyType();
  descriptor.Usage = usage;

  m_sharedTexture.m_desc = descriptor; 

  enDevice& device = enDevice::getInstance();
  bool const isSuccessful = device.CreateTexture2D(m_sharedTexture.m_desc, m_sharedTexture);


  if( isSuccessful == false )
  {
    return false;
  }

  if ( (m_type & enMultiViewType::depthStencil) )
  {
    if( EN_FAIL(this->CreateDepthStencil(width, height, format)) )
    {
      return false;
    }
  }

  if( logicalAndComparisonEnum<enMultiViewType>(m_type, enMultiViewType::renderTarget) )
  {
    if( EN_FAIL(this->CreateRenderTarget(width, height, format)) )
    {
      return false;
    }
  }

  if( logicalAndComparisonEnum<enMultiViewType>(m_type, enMultiViewType::shaderResource) )
  {
    if( EN_FAIL(this->CreateShaderResource(width, height, format)) )
    {
      return false;
    }
  }

  return true;
}

bool
enMultiviewTexture::setRenderTarget()
{
  if( logicalAndComparisonEnum<enMultiViewType>(m_type, enMultiViewType::zeroType) )
  {
    enDeviceContext& deviceContext = enDeviceContext::getInstance();
    deviceContext.OMSetRenderTargets(&m_renderView, nullptr);
    return true;
  }


  return false;
}

bool 
enMultiviewTexture::setDepthStencil()
{
  if( logicalAndComparisonEnum<enMultiViewType>(m_type, enMultiViewType::zeroType) )
  {
    enDeviceContext& deviceContext = enDeviceContext::getInstance();
    deviceContext.setDepthStencilView(m_depthView);
    return true;
  }
 
  return false;
}

enBufferBind
enMultiviewTexture::getBindingFlagsBasedOnMyType()
{
  enBufferBind result = enBufferBind::NONE;
  if( logicalAndComparisonEnum<enMultiViewType>(m_type, enMultiViewType::renderTarget) )
    result = static_cast<enBufferBind> (result | enBufferBind::RenderTarget);

  if( logicalAndComparisonEnum<enMultiViewType>(m_type, enMultiViewType::depthStencil) )
    result = static_cast<enBufferBind> (result | enBufferBind::DepthStencil);

  if( logicalAndComparisonEnum<enMultiViewType>(enMultiViewType::shaderResource, m_type) )
    result = static_cast<enBufferBind> (result | enBufferBind::ShaderResource);

  return result;
}


bool 
enMultiviewTexture::CreateTexture(enMultiViewType type,
                                  float width,
                                  float height)
{
  enDevice& device = enDevice::getInstance();
  bool isSuccessful = false;
  switch( type )
  {
    case  enMultiViewType::zeroType:
      break;
    case enMultiViewType::renderTarget:
    {
      sTextureDescriptor renderDesc = helper::generateTextureDescForRenderTarget(width, height);
      isSuccessful = device.CreateTexture2D(renderDesc, this->m_sharedTexture);
       
      return  isSuccessful;
    }
    case enMultiViewType::depthStencil:
    {
      sTextureDescriptor depthStencilDesc = helper::generateTextureDescForDepthStencil(width, height);
      isSuccessful = device.CreateTexture2D(depthStencilDesc, this->m_sharedTexture);
      return  isSuccessful;
    }
    default:
      break;
  }
  return false;
}

enMultiViewType 
enMultiviewTexture::getType() const
{
  return m_type;
}
