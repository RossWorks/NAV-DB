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
  E_DbError SpareError = IO_ERROR;
  static uint32_t ResultIndex = 0;
  switch (this->State){
    case HMI_START:
      MySettings.ParseSettingFile(this->SettingFilePath);
      MySettings.PrintSettings();
      NextStep = HMI_INIT_DB;
      break;
    case HMI_INIT_DB:
      SpareError = this->StdDbPointer->StdDbInitialization(MySettings.GetDbSortSetting(),
                                                           MySettings.GetA424FilesDir());
      if (SpareError != NO_ERROR){
        NextStep = HMI_TERMINATE;
        std::cout << "STD-DB NOT initalized!\n";
        break;
      }
      NextStep = HMI_SEARCH;
      break;
    case HMI_SEARCH:
      std::cout << "Enter search key: ";
      std::cin >> this->SearchKey;
      if (MySettings.GetTermClearSetting()){system("clear");}
      if (SearchKey == "QUITNOW"){NextStep = HMI_TERMINATE; break;}
      if (SearchKey.at(0) == '#'){
        try{
          ResultIndex = std::stoi(SearchKey.substr(1,SearchKey.size()));
          if (ResultIndex > (this->SearchResults.size()) || ResultIndex < 1){
            throw std::invalid_argument("s");
          }
        }
        catch(const std::exception& e){
          ResultIndex = 0;
          if (MySettings.GetTermClearSetting()){system("clear");}
          NextStep = HMI_SHOW_RESULTS;
          break; 
        }
        NextStep = HMI_DETAIL_RESULT;
        break;
      }
      this->SearchResults = this->StdDbPointer->Search(SearchKey);
      NextStep = HMI_SHOW_RESULTS;
      break;
    case HMI_SHOW_RESULTS:
      std::cout << PresentSearchResult(this->SearchResults);
      NextStep = HMI_SEARCH;
      break;
    case HMI_DETAIL_RESULT:
      if (MySettings.GetTermClearSetting()){system("clear");}
      switch (this->SearchResults.at(ResultIndex-1).ListType){
      case VHF_LIST:
        std::cout << PresentVHF(this->SearchResults.at(ResultIndex-1));
        break;
      case NDB_LIST:
        std::cout << PresentNDB(this->SearchResults.at(ResultIndex-1));
        break;
      case APT_LIST:
        std::cout << PresentAPT(this->SearchResults.at(ResultIndex-1));
        break;
      case WP_LIST:
        break;
      default:
        break;
      }
      NextStep = HMI_SEARCH;
      break;
    default:
      NextStep = HMI_TERMINATE;
      break;
  }
  this->State = NextStep;
  return NextStep;
}