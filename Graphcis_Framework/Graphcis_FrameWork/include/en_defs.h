#pragma once
#include <cstdint>
/**
 * @brief This file contains all the typedef for the entropy engine 
 *
 */

/****************************/
// TYPE DEFS
/****************************/

// signed int type's
using int8 = int8_t; /*!<this is a 8-bit integer */
using int16 = int16_t;//<! this is a 16-bit integer 
using int32 = int32_t;//<! this is a 32-bit integer 
using int64 = int64_t;//<! this is a 64-bit integer 

// unsigned int type's 
using uint8 = uint8_t;//<! this is a 8-bit unsigned integer 
using uint16 = uint16_t;//<! this is a 16-bit unsigned integer 
using uint32 = uint32_t;//<! this is a 32-bit unsigned integer 
using uint64 = uint64_t;//<! this is a 64-bit unsigned integer 


// signed int type's that tell you how much memory they use 
using Byte = int_least8_t;//<! a single byte 
using Byte2 = int_least16_t;//<! 2 bytes per instance
using Byte4 = int_least32_t;//<! 4 bytes per instance 
using Byte8 = int_least64_t;//<! 8 bytes per instance 

// unsigned int type's that tell you how much memory they use 
using uByte = uint_least8_t;//<! a single byte 
using uByte2 = uint_least16_t;//<! 2 bytes per instance
using uByte4 = uint_least32_t;//<! 4 bytes per instance 
using uByte8 = uint_least64_t;//<! 8 bytes per instance 



