#ifndef COMMON_SETTING_HPP
#define COMMON_SETTING_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>
enum LOG_LEVEL{LOG_ERROR, LOG_WARN, LOG_INFO};

class Settings{
private:
  bool SortDb;
  bool ClearTerminalOnNewText;
  bool FileRead;
  LOG_LEVEL Verbosity;
  std::string A424FilesDir;
public:
  Settings();
  ~Settings();
  void ParseSettingFile(std::string SettingsFilePath);
  void PrintSettings();
  bool GetDbSortSetting();
  bool GetTermClearSetting();
  std::string GetA424FilesDir();
  LOG_LEVEL GetVerbosity();
};

#endif
