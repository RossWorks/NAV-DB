#include "./StdDb/StdDb.hpp"
#include "./Common/CommonUtils.hpp"
#include "./Presentation/Presentation.hpp"
#include "./Common/CommonSettings.hpp"
#include "./CLI-Interface/Hmi_SM.hpp"


int main(int argc, char* argv[])
{
  std::string SettingFile(argv[1]);
  Hmi_SM HMI(SettingFile);
  HMI_State HMIstate = HMI_START;
  while (HMIstate != HMI_TERMINATE){
    HMIstate = HMI.ExecuteStep();
  }
  return 0;
}
