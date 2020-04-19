#pragma once
#include <type_traits>
#include <array>

  /**
  * @brief : contains an index and if that index is being used.
  * @tpara IndexType : the integer type of the index.
  * @bug : no known bugs.
  */
template <class IndexType, std::enable_if_t<std::is_integral_v<IndexType>, int> = 0 >
struct sIndexData 
{
  IndexType m_index;
  bool m_isUsed = false;
};

  /**
  * @brief : defines a range of indexes that go's from 0..n-1 and if they are being used.
  * @tparam integralType : defines the type of the index.
  * @tparam sizeOfRange : how big is the range.
  * @bug : no known bugs.
  */
template<class integralType,
  size_t sizeOfRange,
  std::enable_if_t<std::is_integral_v<integralType>
  , int> = 0 >
class enRangeIndexTracker//tracker
{
public:

  using IndexType = sIndexData<integralType>;

public:
  enRangeIndexTracker()
  {
    integralType currentIndex = 0;
    for( auto& index : m_indexes )
    {
      index.m_index = currentIndex;
      currentIndex++;
    }

  }

  enRangeIndexTracker(const enRangeIndexTracker& other) = delete;
  enRangeIndexTracker(enRangeIndexTracker&& other) = default;
  ~enRangeIndexTracker() = default;

public:

  enRangeIndexTracker& operator = (const enRangeIndexTracker& other) = delete;
  enRangeIndexTracker& operator = (enRangeIndexTracker&& other) = default;

  /**
  * @brief : find and gets the first Unused index.
  * @returns -1 if there are no indexes available.
  * @param[in] registerTheIndex : to know if the index should be registered if 
  * one is available found.
  * @bug : no known bugs.
  */
  [[nodiscard]] integralType
  getFirstUnusedIndex(bool registerTheIndex) 
  {
    for( auto& index : m_indexes )
    {
      if( false == index.m_isUsed )
      {
        if( true == registerTheIndex )
          index.m_isUsed = true;

        return index.m_index;
      }
    }

    return static_cast< integralType >(-1);
  }

  /**
  * @brief : registers the index as being used.
  * @param[in] indexValue : the value that's going to be registered.
  * @returns : true when the value was registered, false otherwise.
  * @bug : no known bugs.
  */
  [[nodiscard]] bool
  registerIndexAsUsed(integralType indexValue)
  {

    for( auto& index : m_indexes )
    {
      if( index.m_index == indexValue && false == index.m_isUsed )
      {
        index.m_isUsed = true;
        return true;
      }
    }

    return false;
  }

  /**
  * @brief : registers the index as available to use again.
  * @param[in] indexValue : the index thats going to be registered.
  * @returns  true when a index is successfully registered, false otherwise.
  * @bug :no known bugs.
  */
  [[nodiscard]] bool
  registerIndexAsAvailable(integralType indexValue)
  {

    for( auto& index : m_indexes )
    {

      if( indexValue == index.m_index && true == index.m_isUsed )
      {
        index.m_isUsed = false;
        return true;
      }

    }

    return false;
  }

  /**
  * @returns :the size of the range.
  * @bug : no known bugs.
  */
  size_t 
  getSizeOfRange() const
  {
    return sizeOfRange;
  }


private:
  /**
  * @brief : contains all the indexes.
  */
  std::array< IndexType, sizeOfRange> m_indexes;
};

