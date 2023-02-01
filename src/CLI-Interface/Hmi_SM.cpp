#include "./Hmi_SM.hpp"

Hmi_SM::Hmi_SM(std::string SettingFilePath){
  this->State = HMI_START;
  this->SettingFilePath = SettingFilePath;
}

Hmi_SM::~Hmi_SM(){
}

HMI_State Hmi_SM::ExecuteStep(){
  HMI_State NextStep = HMI_TERMINATE;
  switch (this->State){
    case HMI_START:
      MySettings.ParseSettingFile(this->SettingFilePath);
      MySettings.PrintSettings();
      NextStep = HMI_TERMINATE;
      break;
  
  default:
    break;
  }
  return NextStep;
}