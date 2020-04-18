#include "enShaderResourceView.h"

std::array<sSignedIndexTracker, enShaderResourceView::s_maxIndexes>
enShaderResourceView::s_indexes {
  sSignedIndexTracker(0),
  sSignedIndexTracker(1),
  sSignedIndexTracker(2),
  sSignedIndexTracker(3),
  sSignedIndexTracker(4),
  sSignedIndexTracker(5),
  sSignedIndexTracker(6),
  sSignedIndexTracker(7),
  sSignedIndexTracker(8),
  sSignedIndexTracker(9),
  sSignedIndexTracker(10),
 };


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
  bool const isIndexAlreadyUsed = enShaderResourceView::checkIfIndexIsAlreadyUsed(index);

  if( isIndexAlreadyUsed  )
  {
    EN_LOG_DB("index is not valid");
    return false;
  }
  else
  {
    enShaderResourceView::registerIndexAsBeingUsed(index);
  }
  return true;
}

bool 
enShaderResourceView::autoInit()
{
  int32 const Index = enShaderResourceView::getLeatestUnusedIndex();
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

void 
enShaderResourceView::registerIndexAsBeingUsed(int32 index)
{
  for( auto& element : enShaderResourceView::s_indexes )
  {
    if( element.m_index == index )
    {
      assert(element.m_isUsed == false && "element is already being used");
      element.m_isUsed = true;
      break;
    }
  }


}

int32 
enShaderResourceView::getLeatestUnusedIndex()
{
  for( sSignedIndexTracker& possibleIndex : enShaderResourceView::s_indexes )
  {
    if( possibleIndex.m_isUsed == false )
    {
      possibleIndex.m_isUsed = true;
      return possibleIndex.m_index;
    }
  }
    return -1; 
}

bool 
enShaderResourceView::checkIfIndexIsAlreadyUsed(int32 index)
{
  for( sSignedIndexTracker& element : enShaderResourceView::s_indexes )
  {
    if(element.m_index == index && element.m_isUsed == true)
      return true;
  }
  return false;
}

