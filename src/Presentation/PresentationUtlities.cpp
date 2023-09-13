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

std::string RenderFrequency(Validated_Integer Frequency, bool ColorizeText){
  std::string output;
  if (!Frequency.Status){
    output = InvalidText(6,ColorizeText);
    return output;
  }
  if (Frequency.Value >= 1e6){
    output = std::to_string((float)Frequency.Value/1e6).substr(0,6) + " MHz";
  }
  else if (Frequency.Value >= 1e3){
    output = std::to_string((float)Frequency.Value/1e3).substr(0,6) + " kHz";
  }  
  return output;
}

std::string RenderCoord(Validated_Double Coord, bool IsLat){
  std::string output;
  int degrees = int(Coord.Value);
  float minutes = 0.0;
  minutes = std::abs(Coord.Value - (float)degrees) * 60;
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

std::string PrintClass(E_NavAidClass Class, bool ColorizeText){
  std::string output;
  switch (Class){
    case  UNKNOWN : output = (ColorizeText ? ForeColors[WHITE]   : "")+"UNK   "; break;
    case  APT     : output = (ColorizeText ? ForeColors[CYAN]    : "")+"APT   "; break;
    case  NDB     : output = (ColorizeText ? ForeColors[ORANGE]  : "")+"NDB   "; break;
    case  VOR     : output = (ColorizeText ? ForeColors[GREEN]   : "")+"VOR   "; break;
    case  DME     : output = (ColorizeText ? ForeColors[GREEN]   : "")+"DME   "; break;
    case  VORDME  : output = (ColorizeText ? ForeColors[GREEN]   : "")+"VORDME"; break;
    case  TACAN   : output = (ColorizeText ? ForeColors[GREEN]   : "")+"TACAN "; break;
    case  VORTAC  : output = (ColorizeText ? ForeColors[GREEN]   : "")+"VORTAC"; break;
    case  ILS     : output = (ColorizeText ? ForeColors[YELLOW]  : "")+"ILS   "; break;
    case  ILSDME  : output = (ColorizeText ? ForeColors[YELLOW]  : "")+"ILSDME"; break;
    case  MILTAC  : output = (ColorizeText ? ForeColors[RED]     : "")+"MILTAC"; break;
    case  MLSDME  : output = (ColorizeText ? ForeColors[YELLOW]  : "")+"MLSDME"; break;
    case  WAYPOINT: output = (ColorizeText ? ForeColors[MAGENTA] : "")+"WPT   "; break;
    default       : output = (ColorizeText ? ForeColors[WHITE]   : "")+"UNK   "; break;
    }
    output += (ColorizeText ? ForeColors[CLEAR] : "");
    return output;
}

std::string PrintMagVar(Validated_Float MagVar, bool ColorizeText){
  std::string output("");
  if (!MagVar.Status){output = InvalidText(5,ColorizeText); return output;}
  output = (MagVar.Value >= 0 ? "E " : "W ") + PrintFloat(abs(MagVar.Value),2)+ "°";
  return output;
}

std::string InvalidText(uint32_t Size, bool ColorizeText){
  std::string output("");
  int C = 0;
  if (ColorizeText){output =  ForeColors[AMBER];}
  for (C=0; C<Size; C++){
    output += "-";
  }
  if (ColorizeText){output += ForeColors[CLEAR];}
  return output; 
}

std::string PrintValidatedInteger(Validated_Integer Var, bool ColorizeText){
  std::string output = "";
  if (Var.Status){
    output = std::to_string(Var.Value);
  }
  else{
    output = InvalidText(5,ColorizeText);
  }
  return output;
}