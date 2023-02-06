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
  if (VHF.Class == VOR || VHF.Class == VORDME ||VHF.Class == TACAN ||
      VHF.Class == VORTAC){
    output += "LATITUDE:\t" + RenderCoord(VHF.Lat, true) + "\n";
    output += "LONGITUDE:\t" + RenderCoord(VHF.Lon, false) + "\n";
  }
  output += "FREQUENCY:\t" + RenderFrequency(VHF.Freq) + "\n";
  output += "CHANNEL:\t" + std::to_string(VHF.Channel) + " " + (VHF.ChMode == X ? "X" : "Y") + "\n";
  if (VHF.Class == VORDME || VHF.Class == DME || VHF.Class == TACAN || 
      VHF.Class == VORTAC){
    output += "DME LAT:\t" + RenderCoord(VHF.DmeLat, true) + "\n";
    output += "DME LON:\t" + RenderCoord(VHF.DmeLon, false) + "\n";
    output += "DME ELEV:\t" + std::to_string(VHF.DmeElev) + "\n";
  }
  if (VHF.Class != DME){
    output += "MAG VAR:\t" + std::string(VHF.MagVar >= 0 ? "E" : "W") + std::to_string(abs(VHF.MagVar)) + "\n";
  }
  output += "RANGE:\t";
  switch (VHF.VhfRange){
    case TERMINAL:
      output += "TERMINAL (25 NM)\n";
      break;
    case LOW_ALT:
      output += "LOW ALTITUDE (40 NM)\n";
      break;
    case HI_ALT:
      output += "HIGH ALTITUDE (130 NM)\n";
      break;
    case EXT_HI_ALT:
      output += "VERY HIGH ALTITUDE (> 130 NM)\n";
      break;
    case VHF_IS_MIL:
      output += "STATION IS MILITARY\n";
      break;
    case OUT_OF_SERVICE:
      output += "STATION OUT OF SERVICE\n";
      break;
    default:
      output += "UNDEFINED\n";
      break;
  }
  return output;
}

std::string PresentAPT(DbRecord_t APT){
  std::string output = "";
  output += "NAME:    \t" + std::string(APT.ICAO) + "\n";
  output += "LONG NAME:\t" + std::string(APT.LongName) + "\n";
  output += "IATA CODE:\t" + std::string(APT.IATA) + "\n";
  output += "REGION:  \t" + std::string(APT.CountryCode) + "\n";
  output += "LATITUDE:\t" + RenderCoord(APT.Lat, true) + "\n";
  output += "LONGITUDE:\t" + RenderCoord(APT.Lon, false) + "\n";
  output += "ELEVEATION:\t" + std::to_string(APT.Elev) + "\n";
  output += "FREQUENCY:\t" + RenderFrequency(APT.Freq) + "\n";
  output += "MAG VAR:\t" + std::string(APT.MagVar >= 0 ? "E" : "W") + std::to_string(abs(APT.MagVar)) + "\n";
  output += "USAGE:\t\t";
  switch (APT.AptUsage){
  case CIVIL:    output += "CIVIL"; break;
  case MILITARY: output += "MILITARY"; break;
  case PRIVATE:  output += "PRIVATE"; break;
  case JOINT:    output += "JOINT"; break;
  }
  output += " - ";
  APT.AptIfrCapable ? output += "IFR" : output += "VFR";
  output += "\n";
  output +="LONGEST RWY:\t" + std::to_string(APT.LongestRWYlength) + " ft - ";
  switch (APT.LongRwySurfType){
  case SOFT_SURFACE:  output += "SOFT"; break;
  case HARD_SURFACE:  output += "HARD"; break;
  case WATER_SURFACE: output += "WATER"; break;
  case UNDEF_SURFACE: output += "UNDEFINED"; break;
  }
  output += "\n";
  output += "TIME ZONE:\t" + std::to_string(APT.TimeZoneOffset/60) + ":" +  std::to_string(APT.TimeZoneOffset%60) + "\n";
  return output;
}

std::string PresentNDB(DbRecord_t NDB){
  std::string output = "";
  output += "NAME:    \t" + std::string(NDB.ICAO) + "\n";
  output += "REGION:  \t" + std::string(NDB.CountryCode) + "\n";
  output += "LATITUDE:\t" + RenderCoord(NDB.Lat, true) + "\n";
  output += "LONGITUDE:\t" + RenderCoord(NDB.Lon,false) + "\n";
  output += "FREQUENCY:\t" + RenderFrequency(NDB.Freq) + "\n";
  output += "MAG VAR:\t" + std::string(NDB.MagVar >= 0 ? "E" : "W") + std::to_string(abs(NDB.MagVar)) + "\n";
  return output;
}

std::string PresentList(std::vector<DbRecord_t> List){
  std::string output("");
  return output;
}