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
  uint32_t TmpInt = 0;
  std::string UserInput("");
  E_LIST_TYPE MyList = VHF_LIST;
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
      std::cin >> UserInput;
      if (MySettings.GetTermClearSetting()){system("clear");}
      if (UserInput == "QUITNOW"){NextStep = HMI_TERMINATE; break;}
      NextStep = ParseCommand(UserInput, &(this->SearchKey), &TmpInt, &MyList);
      switch (NextStep){
        case HMI_LIST:
          this->SearchResults = this->StdDbPointer->GetList(MyList,TmpInt,10);
          NextStep = HMI_SHOW_RESULTS;
          break;
        case HMI_SEARCH:
          this->SearchResults = this->StdDbPointer->Search(SearchKey);
          NextStep = HMI_SHOW_RESULTS;
          break;
        case HMI_DETAIL_RESULT:
          ResultIndex = TmpInt;
          NextStep = HMI_DETAIL_RESULT;
          break;
        default:
          break;
      }
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