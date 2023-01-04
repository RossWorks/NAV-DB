#ifndef COMMON_SETTING_HPP
#define COMMON_SETTING_HPP

#include <string>

enum E_ArgParseError{
  NO_ERROR_ARG  ,
  INVALID_ARG
};

class Settings{
private:
  bool SortDb;
public:
  Settings();
  ~Settings();
  void Parse(int argc, char** argv);
  bool GetDbSortSetting();
};

#endif