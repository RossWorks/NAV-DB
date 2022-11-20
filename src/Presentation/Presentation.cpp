#include "Presentation.hpp"

std::string PresentSearchResult(std::vector<DbRecord_t> List){
  std::string output = "+--------+--------+----------+--------+--------+--------+\n";
  char TmpString[100];
  std::string TmpFreq;
  uint8_t NameLen = 0;
  std::string WpType;
  for (DbRecord_t element: List){
    switch (element.Class){
    case  0: WpType = "UNK";    break;
    case  1: WpType = "NDB";    break;
    case  2: WpType = "VOR";    break;
    case  3: WpType = "DME";    break;
    case  4: WpType = "VORDME"; break;
    case  5: WpType = "TACAN";  break;
    case  6: WpType = "VORTAC"; break;
    case  7: WpType = "ILS";    break;
    case  8: WpType = "ILSDME"; break;
    case  9: WpType = "MILTAC"; break;
    case 11: WpType = "MLSDME"; break;
    default: break;
    }
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
