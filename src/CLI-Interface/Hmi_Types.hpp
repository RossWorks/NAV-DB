#ifndef HMI_TYPES
#define HMI_TYPES

#include <string>

enum HMI_State{
    HMI_START,
    HMI_INIT_DB,
    HMI_SHOW_STATISTICS,
    HMI_SEARCH,
    HMI_SHOW_RESULTS,
    HMI_DETAIL_RESULT,
    HMI_LIST,
    HMI_BUILD_DB,
    HMI_END,
    HMI_TERMINATE
};

const std::string C_DbGenerationPath = "./DbGeneration/";

#endif