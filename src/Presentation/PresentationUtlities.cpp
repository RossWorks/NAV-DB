#include "./PresentationUtilities.hpp"

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