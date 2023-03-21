#include "./PresentationUtilities.hpp"

std::map <E_TermColors, std::string> ForeColors ={
  {CLEAR,  "\e[m"},
  {MAGENTA,"\e[38;5;13m"},
  {RED,    "\e[38;5;9m"},
  {ORANGE, "\e[38;5;208m"},
  {AMBER,  "\e[38;5;202m"},
  {YELLOW, "\e[38;5;3m"},
  {GREEN,  "\e[38;5;10m"},
  {CYAN,   "\e[38;5;14m"},
  {BLUE,   "\e[38;5;4m"},
  {INDIGO, "\e[38;5;12m"},
  {WHITE,  "\e[38;5;15m"},
  {BLACK,  "\e[38;5;m"}
};

std::string RenderFrequency(uint32_t Frequency){
  std::string output;
  if (Frequency >= 1e6){
    output = std::to_string((float)Frequency/1e6).substr(0,6) + " MHz";
  }
  else if (Frequency >= 1e3){
    output = std::to_string((float)Frequency/1e3).substr(0,6) + " kHz";
  }  
  return output;
}

std::string RenderCoord(double Coord, bool IsLat){
  std::string output;
  int degrees = static_cast<int>(Coord);
  float minutes = 0.0;
  minutes = std::abs(Coord - (float)degrees) * 60;
  //minutes = (static_cast<int>(minutes*100))/100;
  output  = std::to_string(abs(degrees)) + "° " + PrintFloat(minutes,2) + "'";
  if (IsLat){
    (degrees<0 ? output = "S"+output : output = "N"+output);
  }
  else{
    (degrees<0 ? output = "W"+output : output = "E"+output);
  }
  return output;  
}

std::string PrintFloat(float number, int decimals){
  std::string output = std::to_string(number);
  uint64_t IndexOfDot = std::string::npos;
  IndexOfDot = output.find('.');
  output = output.substr(0,IndexOfDot+decimals+1);
  return output;
}

std::string PrintClass(E_NavAidClass Class){
  std::string output;
  switch (Class){
    case  UNKNOWN : output = ForeColors[WHITE]  +"UNK   "; break;
    case  APT     : output = ForeColors[CYAN]   +"APT   "; break;
    case  NDB     : output = ForeColors[ORANGE] +"NDB   "; break;
    case  VOR     : output = ForeColors[GREEN]  +"VOR   "; break;
    case  DME     : output = ForeColors[GREEN]  +"DME   "; break;
    case  VORDME  : output = ForeColors[GREEN]  +"VORDME"; break;
    case  TACAN   : output = ForeColors[GREEN]  +"TACAN "; break;
    case  VORTAC  : output = ForeColors[GREEN]  +"VORTAC"; break;
    case  ILS     : output = ForeColors[YELLOW] +"ILS   "; break;
    case  ILSDME  : output = ForeColors[YELLOW] +"ILSDME"; break;
    case  MILTAC  : output = ForeColors[RED]    +"MILTAC"; break;
    case  MLSDME  : output = ForeColors[YELLOW] +"MLSDME"; break;
    case  WAYPOINT: output = ForeColors[MAGENTA]+"WPT   "; break;
    default       : output = ForeColors[WHITE]  +"UNK   "; break;
    }
    output += ForeColors[CLEAR];
    return output;
}