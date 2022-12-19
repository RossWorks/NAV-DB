#include "Presentation.hpp"

std::string PresentSearchResult(std::vector<DbRecord_t> List){
  std::string output = "";
  char TmpString[100];
  uint8_t NameLen = 0;
  std::string WpType;
  unsigned int I = 1;
  if (List.size() == 0){
    output += "LIST EMPTY\n";
  }
  for (DbRecord_t element: List){
    switch (element.Class){
    case  UNKNOWN : WpType = "\e[05;31mUNK   "; break;
    case  APT     : WpType = "\e[01;36mAPT   "; break;
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
    default       : WpType = "\e[05;31mUNK   "; break;
    }
    WpType += "\e[m";
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

std::string PresentVHF(DbRecord_t VHF){
  std::string output = "";
  output += "NAME:    \t" + std::string(VHF.ICAO) + "\n";
  output += "REGION:  \t" + std::string(VHF.CountryCode) + "\n";
  output += "LATITUDE:\t" + RenderCoord(VHF.Lat) + "\n";
  output += "LONGITUDE:\t" + RenderCoord(VHF.Lon) + "\n";
  output += "FREQUENCY:\t" + RenderFrequency(VHF.Freq) + "\n";
  output += "DME LAT:\t" + RenderCoord(VHF.DmeLat) + "\n";
  output += "DME LON:\t" + RenderCoord(VHF.DmeLon) + "\n";
  output += "DME ELEV:\t" + std::to_string(VHF.DmeElev) + "\n";
  output += "CHANNEL:\t" + std::to_string(VHF.Channel) + " " + (VHF.ChMode == X ? "X" : "Y") + "\n";
  output += "MAG VAR:\t" + std::string(VHF.MagVar >= 0 ? "E" : "W") + std::to_string(abs(VHF.MagVar)) + "\n";
  return output;
}

std::string PresentAPT(DbRecord_t APT){
  std::string output = "";
  output += "NAME:    \t" + std::string(APT.ICAO) + "\n";
  output += "LONG NAME:\t" + std::string(APT.LongName) + "\n";
  output += "REGION:  \t" + std::string(APT.CountryCode) + "\n";
  output += "LATITUDE:\t" + RenderCoord(APT.Lat) + "\n";
  output += "LONGITUDE:\t" + RenderCoord(APT.Lon) + "\n";
  output += "FREQUENCY:\t" + RenderFrequency(APT.Freq) + "\n";
  output += "MAG VAR:\t" + std::string(APT.MagVar >= 0 ? "E" : "W") + std::to_string(abs(APT.MagVar)) + "\n";
  return output;
}

std::string PresentNDB(DbRecord_t NDB){
  std::string output = "";
  output += "NAME:    \t" + std::string(NDB.ICAO) + "\n";
  output += "REGION:  \t" + std::string(NDB.CountryCode) + "\n";
  output += "LATITUDE:\t" + std::string(std::to_string(NDB.Lat)) + "\n";
  output += "LONGITUDE:\t" + std::string(std::to_string(NDB.Lon)) + "\n";
  output += "FREQUENCY:\t" + RenderFrequency(NDB.Freq) + "\n";
  output += "MAG VAR:\t" + std::string(NDB.MagVar >= 0 ? "E" : "W") + std::to_string(abs(NDB.MagVar)) + "\n";
  return output;
}