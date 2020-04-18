#pragma once
#include "en_defs.h"
#include <type_traits>


template<class TypeOfTracker,
  size_t sizeOfRange,
  std::enable_if_t<std::is_class_v<TypeOfTracker>
  , int> = 0 >
class baseRangeIndexTracker
{
public:

};

