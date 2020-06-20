#include "enRenderManager.h"

enRenderManager::enRenderManager()
  :m_multiShader(std::make_unique<enMultiShader>()),
  m_selectedShader(0)
{}

void
enRenderManager::setSelectedShader()
{
  m_multiShader->setShader(m_selectedShader);
}

void 
enRenderManager::setShaderIndex(int32 index)
{
  m_selectedShader = index; 
}

int32 
enRenderManager::getShaderIndex() const
{
  return m_selectedShader;
}

int32 
enRenderManager::getShaderCount() const
{
  return m_multiShader->m_shaderPairs.size();
}

enMultiShader*
enRenderManager::getMultiShaderPtr()
{
  return m_multiShader.get();
}
