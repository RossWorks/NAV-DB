#include "./Hmi_Support.hpp"

HMI_State ParseCommand(std::string Command, std::string* SearchKey,
                       uint32_t* Index, E_LIST_TYPE* STDList){
    int TmpInt = 0;
    if (Command.at(0) != '#'){
      *SearchKey = Command;
      return HMI_SEARCH;
    }
    if (Command.substr(1,3) == "VHF"){
      *STDList = VHF_LIST;
      *Index=std::stoi(Command.substr(4,Command.size()-4));
      return HMI_LIST;
    }
    if (Command.substr(1,3) == "APT"){
      *STDList = APT_LIST;
      *Index=std::stoi(Command.substr(4,Command.size()-4));
      return HMI_LIST;
    }
    if (Command.substr(1,3) == "NDB"){
      *STDList = NDB_LIST;
      *Index=std::stoi(Command.substr(4,Command.size()-4));
      return HMI_LIST;
    }
    try{
      TmpInt = std::stoi(Command.substr(1,Command.size()));
      *Index = TmpInt;
      return HMI_DETAIL_RESULT;
    }
    catch(const std::exception& e){return HMI_SEARCH;}
}