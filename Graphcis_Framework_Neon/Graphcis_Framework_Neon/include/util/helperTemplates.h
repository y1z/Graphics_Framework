  /**
  * @brief : where all the templates helper function are kept.
  * @bug :
  */
#pragma once
#include "en_defs.h"
#include <type_traits>

namespace helper
{

  /**
  * @brief : does the logical AND comparison with 2 enums.
  * @returns true when the logical AND comparison is true.
  * @tparam EnumType: the type of the enum.
  * @tparam BaseEnumType : the underlining primitive type of the enum( most cases it's int).
  */
  template<class EnumType, class BaseEnumType = int>
  std::enable_if_t<std::is_enum_v<EnumType>, bool>
  logicalAndComparisonEnum(EnumType leftEnum,
                           EnumType rightEnum)
  {
    return static_cast< BaseEnumType >(leftEnum) & static_cast< BaseEnumType >(rightEnum);
  }


}
