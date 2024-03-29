#include "./Hmi_Support.hpp"

HMI_State ParseCommand(std::string Command, std::string* SearchKey,
                       uint32_t* Index, E_LIST_TYPE* STDList, std::string *OrderFromKey){
    int TmpInt = 0;
    if (Command.at(0) != '#'){
      *SearchKey = Command;
      return HMI_SEARCH;
    }
    if (Command.substr(1,3) == "VHF"){
      *STDList = VHF_LIST;
      if (Command.substr(4,1) == "-"){
        *OrderFromKey = Command.substr(5);
      }
      else if (Command.substr(4,1) == "/"){
        try{
          *Index=std::stoi(Command.substr(5));
        }
        catch(std::invalid_argument& InvalidOrderFromIndexCast){
          return HMI_SEARCH;
        }      }
      return HMI_LIST;
    }
    if (Command.substr(1,3) == "APT"){
      *STDList = APT_LIST;
      if (Command.substr(4,1) == "-"){
        *OrderFromKey = Command.substr(5);
      }
      else if (Command.substr(4,1) == "/"){
        try{
          *Index=std::stoi(Command.substr(5));
        }
        catch(std::invalid_argument& InvalidOrderFromIndexCast){
          return HMI_SEARCH;
        }      }
      return HMI_LIST;
    }
    if (Command.substr(1,3) == "NDB"){
      *STDList = NDB_LIST;
      if (Command.substr(4,1) == "-"){
        *OrderFromKey = Command.substr(5);
      }
      else if (Command.substr(4,1) == "/"){
        try{
          *Index=std::stoi(Command.substr(5));
        }
        catch(std::invalid_argument& InvalidOrderFromIndexCast){
          return HMI_SEARCH;
        }
      }
      return HMI_LIST;
    }
    if (Command.substr(1,3) == "WPT"){
      *STDList = WP_LIST;
      if (Command.substr(4,1) == "-"){
        *OrderFromKey = Command.substr(5);
      }
      else if (Command.substr(4,1) == "/"){
        try{
          *Index=std::stoi(Command.substr(5));
        }
        catch(std::invalid_argument& InvalidOrderFromIndexCast){
          return HMI_SEARCH;
        }
      }
      return HMI_LIST;
    }
    if (Command.substr(1,5) == "BUILD"){
      if(Command.substr(6,1) == "B"){*Index = 1;}
      else{*Index = 0;}
      return HMI_BUILD_DB;
    }
    try{
      TmpInt = std::stoi(Command.substr(1,Command.size()));
      *Index = TmpInt;
      return HMI_DETAIL_RESULT;
    }
    catch(const std::exception& e){return HMI_SEARCH;}
}