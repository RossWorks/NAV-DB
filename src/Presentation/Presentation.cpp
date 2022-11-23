#include "Presentation.hpp"

std::string PresentSearchResult(std::vector<DbRecord_t> List){
  std::string output = "+--------+--------+----------+--------+--------+--------+\n";
  char TmpString[100];
  std::string TmpFreq;
  uint8_t NameLen = 0;
  std::string WpType;
  if (List.size() == 0){
    output += "LIST EMPTY\n";
  }
  for (DbRecord_t element: List){
    switch (element.Class){
    case  UNKNOWN : WpType = "\e[05;31mUNK";    break;
    case  NDB     : WpType = "\e[01;31mNDB";    break;
    case  VOR     : WpType = "\e[01;32mVOR";    break;
    case  DME     : WpType = "\e[01;32mDME";    break;
    case  VORDME  : WpType = "\e[01;32mVORDME"; break;
    case  TACAN   : WpType = "\e[01;32mTACAN";  break;
    case  VORTAC  : WpType = "\e[01;32mVORTAC"; break;
    case  ILS     : WpType = "\e[01;31mILS";    break;
    case  ILSDME  : WpType = "\e[01;31mILSDME"; break;
    case  MILTAC  : WpType = "\e[01;31mMILTAC"; break;
    case  MLSDME  : WpType = "\e[01;31mMLSDME"; break;
    default: break;
    }
    WpType += "\e[m";
    TmpFreq = RenderFrequency(element.Freq);
    sprintf(TmpString,"|%-8s|%-8s|%s|%c%7.3f|%c%7.3f|%3i - %c |\n",
            element.ICAO,
            WpType.c_str(),
            TmpFreq.c_str(),
            (element.Lat>=0) ? 'N':'S',
            element.Lat,
            (element.Lon>=0) ? 'E': 'W',
            element.Lon,
            element.Channel,
            (element.ChMode == Y)? 'Y':'X');
    output += TmpString;
  }
  output += "+--------+--------+----------+--------+--------+--------+\n";
  return output;
}
