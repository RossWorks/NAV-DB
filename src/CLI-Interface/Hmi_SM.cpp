#include "./Hmi_SM.hpp"

Hmi_SM::Hmi_SM(std::string SettingFilePath, StdDb* StdDbObj){
  this->State = HMI_START;
  this->SettingFilePath = SettingFilePath;
  this->StdDbPointer = StdDbObj;
}

Hmi_SM::~Hmi_SM(){
}

HMI_State Hmi_SM::ExecuteStep(){
  HMI_State NextStep = HMI_TERMINATE;
  switch (this->State){
    case HMI_START:
      MySettings.ParseSettingFile(this->SettingFilePath);
      MySettings.PrintSettings();
      NextStep = HMI_INIT_DB;
      break;
    case HMI_INIT_DB:
      this->StdDbPointer->StdDbInitialization(MySettings.GetDbSortSetting());
      NextStep = HMI_SEARCH;
      break;
    case HMI_SEARCH:
      std::cout << "Enter search key: ";
      std::cin >> this->SearchKey;
      if (MySettings.GetTermClearSetting()){system("clear");}
      if (SearchKey == "QUITNOW"){NextStep = HMI_TERMINATE; break;}
      this->SearchResults = this->StdDbPointer->Search(SearchKey);
      NextStep = HMI_SHOW_RESULTS;
      break;
    case HMI_SHOW_RESULTS:
      std::cout << PresentSearchResult(this->SearchResults);
      NextStep = HMI_SEARCH;
      break;
    default:
      NextStep = HMI_TERMINATE;
      break;
  }
  this->State = NextStep;
  return NextStep;
}