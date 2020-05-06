#include "enShaderResourceView.h"

enRangeIndexTracker<int32, enShaderResourceView::s_maxIndexes> enShaderResourceView::s_indexes;



enShaderResourceView::enShaderResourceView(enShaderResourceView&& other) noexcept
  :m_descriptor(other.m_descriptor),
  m_interface(other.m_interface)
{
  other.m_descriptor.mp_data = nullptr;
#if DIRECTX
  other.m_interface = nullptr;
#elif OPENGL
  other.m_interface = std::numeric_limits<uint32>::max();
#else
  other.m_interface = nullptr;
#endif // DIRECTX
}

enShaderResourceView::~enShaderResourceView()
{
#if DIRECTX
  RELEASE_DX_PTR(m_interface);
#elif OPENGL
  if( m_interface != std::numeric_limits<uint32>::max() )
  {
    glDeleteTextures(1, &m_interface);
  }
#endif // DIRECTX
}


bool 
enShaderResourceView::init(const sShaderResourceDesc& descriptor)
{
  this->m_descriptor = descriptor;

  int32 const index = m_descriptor.m_index;
  bool const isRegisterSuccessful = s_indexes.registerIndexAsUsed(index);

  if( false == isRegisterSuccessful )
  {
    EN_LOG_DB("index is not valid");
    return false;
  }

  return true;
}

bool 
enShaderResourceView::autoInit()
{
  int32 const Index = s_indexes.getFirstUnusedIndex(true);
  if( Index == -1 )
  {
    return false;
  }
  m_descriptor.m_index = Index;

  return true;
}

int32_t
enShaderResourceView::getIndex() const
{
  return this->m_descriptor.m_index;
}

