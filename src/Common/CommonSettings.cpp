#include "./CommonSettings.hpp"


Settings::Settings(){
  this->SortDb = true;
}

Settings::~Settings(){}

void Settings::Parse(int argc, char** argv){
  int c = -1;
  std::string TmpArg;
  for (c=1; c<argc; c++){
    TmpArg = argv[c];
    if (TmpArg[0] != '-'){
      break;
    }
    if (TmpArg[1] == '-'){/*handle logarg*/}
    if (TmpArg[1] == 's'){this->SortDb = false;}
  }  
}

bool Settings::GetDbSortSetting(){return this->SortDb;}