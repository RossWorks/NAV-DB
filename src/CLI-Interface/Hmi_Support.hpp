#ifndef HMI_SUPPORT
#define HMI_SUPPORT

#include <string>
#include <stdexcept>
#include "./Hmi_Types.hpp"
#include "../StdDb/DbCommonTypes.hpp"

HMI_State ParseCommand(std::string Command, std::string* SearchKey,
                       uint32_t* Index, E_LIST_TYPE* STDList, std::string *OrderFromKey);

#endif