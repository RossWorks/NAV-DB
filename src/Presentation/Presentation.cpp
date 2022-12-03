#include "Presentation.hpp"

std::string PresentSearchResult(std::vector<DbRecord_t> List){
  std::string output = "";
  char TmpString[100];
  std::string TmpFreq;
  uint8_t NameLen = 0;
  std::string WpType;
  unsigned int I = 1;
  if (List.size() == 0){
    output += "LIST EMPTY\n";
  }
  for (DbRecord_t element: List){
    switch (element.Class){
    case  UNKNOWN : WpType = "\e[05;31mUNK   "; break;
    case  NDB     : WpType = "\e[01;31mNDB   "; break;
    case  VOR     : WpType = "\e[01;32mVOR   "; break;
    case  DME     : WpType = "\e[01;32mDME   "; break;
    case  VORDME  : WpType = "\e[01;32mVORDME"; break;
    case  TACAN   : WpType = "\e[01;32mTACAN "; break;
    case  VORTAC  : WpType = "\e[01;32mVORTAC"; break;
    case  ILS     : WpType = "\e[01;33mILS   "; break;
    case  ILSDME  : WpType = "\e[01;33mILSDME"; break;
    case  MILTAC  : WpType = "\e[01;31mMILTAC"; break;
    case  MLSDME  : WpType = "\e[01;31mMLSDME"; break;
    default: break;
    }
    WpType += "\e[m";
    TmpFreq = RenderFrequency(element.Freq);
    sprintf(TmpString,"%i -> |%-8s|%-8s|%s|\n",
            I,
            element.ICAO,
            WpType.c_str(),
            element.CountryCode);
    output += TmpString;
    I++;
  }
  return output;
}

std::string PresentDetailed(DbRecord_t element){
  std::string output = "";
  output += "NAME:\t" + std::string(element.ICAO) + "\n";
  output += "REGION:\t" + std::string(element.CountryCode) + "\n";
  output += "LATITUDE:\t" + std::string(std::to_string(element.Lat)) + "\n";
  output += "LONGITUDE:\t" + std::string(std::to_string(element.Lon)) + "\n";
  output += "NAME:\t" + std::string(element.ICAO) + "\n";
  output += "NAME:\t" + std::string(element.ICAO) + "\n";
  output += "NAME:\t" + std::string(element.ICAO) + "\n";
  return output;
}
