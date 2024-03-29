#include "./StdDb/StdDb.hpp"
#include "./Common/CommonUtils.hpp"
#include "./Presentation/Presentation.hpp"
#include "./Common/CommonSettings.hpp"
#include "./CLI-Interface/Hmi_SM.hpp"

int main(int argc, char* argv[])
{
  std::string SettingFile = "";
  if (argc>1){
    SettingFile = std::string(argv[1]);
  }
  StdDb MyStdDb;
  Hmi_SM HMI(SettingFile, &MyStdDb);
  HMI_State HMIstate = HMI_START;
  while (HMIstate != HMI_TERMINATE){
    HMIstate = HMI.ExecuteStep();
  }
  return 0;
}
