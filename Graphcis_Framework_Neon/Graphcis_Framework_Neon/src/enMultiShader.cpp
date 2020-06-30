#include "enMultiShader.h"
#include "helperFucs.h"
#include "cApiComponent.h"
#include "enDeviceContext.h"
#include <cassert>


enRangeIndexTracker<uint32, enMultiShader::s_max >
enMultiShader::s_trackedPrograms;

bool
enMultiShader::createEveryShaderVersion(enShaderPathTracker const& shaderTracker,
                                        std::vector<std::string > const& definesForEachShader)
{
  std::vector<enMultiShader::sShaderSourcePair>
    sourceCodeForShaders = createShaderWithDefines(shaderTracker, definesForEachShader);

  return EN_SUCCESS(compileEveryVersionOfShader(sourceCodeForShaders));
}

enMultiShader::sShaderPair&
enMultiShader::getShaderPairPtr(std::size_t index)
{
  return m_shaderPairs[index];
}

enVertexShader& 
enMultiShader::getVertexShaderRef(std::size_t const index)
{
  return *m_shaderPairs.at(index).m_vertexShader;
}

enPixelShader&
enMultiShader::getPixelShaderRef(std::size_t const index)
{
  return *m_shaderPairs.at(index).m_pixelShader;
}

std::vector<enMultiShader::sShaderSourcePair>
enMultiShader::createShaderWithDefines(enShaderPathTracker const& shaderTracker,
                                       std::vector<std::string> const& definesForEachShader)const
{
  std::vector<enMultiShader::sShaderSourcePair> result(definesForEachShader.size());


  std::string const baseVertexFile = helper::loadFileToString(shaderTracker.getVertexShaderName());
  std::string const basePixelFile = helper::loadFileToString(shaderTracker.getPixelShaderName());

  auto const firstVertexSourceNewLineIndex = baseVertexFile.find('\n') + 1;
  auto const firstPixelSourceNewLineIndex = basePixelFile.find('\n') + 1;

  std::size_t i = 0;
  for( auto& shaderSource : result )
  {
    shaderSource.m_vertexSource = baseVertexFile;

    shaderSource.m_vertexSource.insert(firstVertexSourceNewLineIndex,
                                       definesForEachShader[i] + "\n");

    shaderSource.m_pixelSource = basePixelFile;

    shaderSource.m_pixelSource.insert(firstPixelSourceNewLineIndex,
                                      definesForEachShader[i] + "\n");

    ++i;
  }

  return result;
}

enErrorCode
enMultiShader::compileEveryVersionOfShader(const std::vector<enMultiShader::sShaderSourcePair>& shaderCode)
{
  m_shaderPairs.reserve(shaderCode.size());
  size_t index = 0;
  for(auto& code : shaderCode )
  {
    sShaderPair element;
    element.m_vertexShader = new enVertexShader;
    element.m_pixelShader = new enPixelShader;

    enErrorCode const isVertexOK =
    element.m_vertexShader->compileShader(code.m_vertexSource,
                                          "VS",
                                          "vs_4_0");

    if( EN_FAIL(isVertexOK) )
    { return isVertexOK; }

    enErrorCode const isPixelOK =
    element.m_pixelShader->compileShader(code.m_pixelSource,
                                         "PS",
                                         "ps_4_0");

    if( EN_FAIL(isPixelOK) )
    { return  isPixelOK; }

    element.m_shaderProgramIndex = s_trackedPrograms.getFirstUnusedIndex(false);

    element.isIndexCreated = s_trackedPrograms.registerIndexAsUsed(element.m_shaderProgramIndex);

    m_shaderPairs.emplace_back(std::move(element));

    ++index;
  }

  return enErrorCode::NoError;
}

std::size_t
enMultiShader::getShaderCount() const
{
  return m_shaderPairs.size();
}

void 
enMultiShader::initShader(std::size_t index)
{
  assert(index <= m_shaderPairs.size() - 1 && "trying to access out of bounds");

#if OPENGL
  auto* selectedShaderPair = &m_shaderPairs[index];
  if  ( !selectedShaderPair->isIndexCreated )
  {
    cApiComponents::createProgram(selectedShaderPair->m_shaderProgramIndex);
    selectedShaderPair->isIndexCreated = true;
  }
#elif DIRECTX

#endif // OPENGL
}

void
enMultiShader::setShader(std::size_t index)
{
  assert(index <= m_shaderPairs.size() - 1 && "trying to access out of bounds");
  auto* selectedShaderPair = &m_shaderPairs[index];
  enDeviceContext& deviceContext = enDeviceContext::getInstance();
  deviceContext.SetShaders(*selectedShaderPair->m_vertexShader, *selectedShaderPair->m_pixelShader);
}

