#ifndef HMI_SM
#define HMI_SM

#include <string>
#include <vector>
#include "../Common/CommonSettings.hpp"
#include "../StdDb/StdDb.hpp"
#include "../Presentation/Presentation.hpp"
#include "./Hmi_Support.hpp"
#include "./Hmi_Types.hpp"

class Hmi_SM{
private:
  StdDb* StdDbPointer;
  HMI_State State;
  std::string SettingFilePath;
  Settings MySettings;
  std::string SearchKey;
  std::vector<DbRecord_t> SearchResults;
public:
  Hmi_SM(std::string SettingFilePath, StdDb* StdDbObj);
  ~Hmi_SM();
 /**
   * @brief Executes the Hmi state machine
   * @return HMI_State The State of HMI Machine at the end of step execution
   */
  HMI_State ExecuteStep();
};

#endif //HMI_SM