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
    WpType = PrintClass(element.Class, true);
    sprintf(TmpString,"%2i -> |%-8s|%-8s|%s|\n",
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
  output += "CLASS:   \t" + PrintClass(VHF.Class, true) + "\n";
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
    output += "DME ELEV:\t" + PrintValidatedInteger(VHF.DmeElev) + "\n";
  }
  if (VHF.Class != DME){
    output += "MAG VAR:\t" + PrintMagVar(VHF.MagVar) + "\n";
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
  output += "ICAO CODE:\t" + std::string(APT.ICAO) + "\n";
  output += "IATA CODE:\t" + std::string(APT.IATA) + "\n";
  output += "LONG NAME:\t" + std::string(APT.LongName) + "\n";
  output += "REGION:  \t" + std::string(APT.CountryCode) + "\n";
  output += "LATITUDE:\t" + RenderCoord(APT.Lat, true) + "\n";
  output += "LONGITUDE:\t" + RenderCoord(APT.Lon, false) + "\n";
  output += "ELEVEATION:\t" + PrintValidatedInteger(APT.Elev) + "\n";
  output += "REC NAVAID:\t" + std::string(APT.RecommendedNavaid) + "\n";
  output += "MAG VAR:\t" + PrintMagVar(APT.MagVar) + "\n";
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
  output += "MAG VAR:\t" + PrintMagVar(NDB.MagVar) + "\n";
  return output;
}

std::string PresentWPT(DbRecord_t WPT){
  std::string output = "";
  output += "NAME:    \t" + std::string(WPT.ICAO) + "\n";
  output += "REGION:  \t" + std::string(WPT.CountryCode) + "\n";
  output += "LATITUDE:\t" + RenderCoord(WPT.Lat, true) + "\n";
  output += "LONGITUDE:\t" + RenderCoord(WPT.Lon,false) + "\n";
  output += "MAG VAR:\t" + PrintMagVar(WPT.MagVar) + "\n";
  return output;
}

std::string PresentList(std::vector<DbRecord_t> List){
  std::string output("");
  return output;
}