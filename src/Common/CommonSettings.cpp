#include "./CommonSettings.hpp"


Settings::Settings(){
  this->SortDb = true;
  this->ClearTerminalOnNewText = false;
  this->Verbosity = LOG_INFO;
  this->FileRead = false;
  this->A424FilesDir = "./Data/";
}

Settings::~Settings(){}

void Settings::ParseSettingFile(std::string SettingsFilePath){
  std::fstream SettingsFile(SettingsFilePath);
  uint64_t EqualIndex = 0;
  std::string FileLine;
  std::string key, value;
  uint32_t I = 0;
  this->FileRead = true;
  if (!SettingsFile.is_open()){
    this->FileRead = false;
    return;
  }
  while (SettingsFile.good()){
    std::getline(SettingsFile,FileLine);
    EqualIndex = FileLine.find('=');
    if (EqualIndex != std::string::npos){
      key = FileLine.substr(0,EqualIndex);
      for (I=0; I<key.size();I++){key.at(I) = std::toupper(key.at(I));}
      value = FileLine.substr(EqualIndex+1,FileLine.size()-EqualIndex);
      if (key == "SORT_DB"){
        this->SortDb = (std::toupper(value.at(0))=='Y') ? true : false;
        continue;
      }
      if (key == "LOG_LEVEL"){
        if (value == "LOG_ERROR"){this->Verbosity=LOG_ERROR;continue;}
        if (value == "LOG_WARNING"){this->Verbosity=LOG_WARN;continue;}
        if (value == "LOG_INFO"){this->Verbosity=LOG_INFO;continue;}        
      }
      if (key == "CLEAR_TERMINAL"){
        this->ClearTerminalOnNewText = (std::toupper(value.at(0))=='Y') ? true : false;
        continue;
      }
      if (key == "A424DIR"){
        this->A424FilesDir = value.substr(0,value.size());
      }
    }
  }
  SettingsFile.close();
  return;
}

void Settings::PrintSettings(){
  if (this->Verbosity >= LOG_WARN){
    if (this->FileRead){std::cout << "Settings file read\n";}
    else{std::cout << "Settings file NOT read\n";}
  }
  if (this->Verbosity == LOG_INFO){
    std::cout << "Database sorting is ";
    std::cout << (this->SortDb ? "ENABLED" : "DISABLED") << "\n";
    std::cout << "Terminal will " << (this->ClearTerminalOnNewText?"":"NOT ")<< "be cleared at each new print\n";
  }
  return;
}

bool Settings::GetDbSortSetting(){return this->SortDb;}

bool Settings::GetTermClearSetting(){return this->ClearTerminalOnNewText;}

std::string Settings::GetA424FilesDir(){return this->A424FilesDir;}

LOG_LEVEL Settings::GetVerbosity(){return this->Verbosity;}