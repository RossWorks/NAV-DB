#ifndef HMI_SM
#define HMI_SM

#include <string>
#include "../Common/CommonSettings.hpp"

enum HMI_State{
    HMI_START,
    HMI_SORTING_DB,
    HMI_SHOW_STATISTICS,
    HMI_SEARCH,
    HMI_SHOW_RESULTS,
    HMI_DETAIL_RESULT,
    HMI_END,
    HMI_TERMINATE
};



class Hmi_SM{
private:
  HMI_State State;
  std::string SettingFilePath;
  Settings MySettings;
public:
  Hmi_SM(std::string SettingFilePath);
  ~Hmi_SM();
 /**
   * @brief Executes the Hmi state machine
   * @return HMI_State The State of HMI Machine at the end of step execution
   */
  HMI_State ExecuteStep();
};

#endif //HMI_SM