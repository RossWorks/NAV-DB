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
  minutes = (static_cast<int>(minutes*100))/100;
  output  = std::to_string(abs(degrees)) + "° " + std::to_string(minutes) + "'";
  if (IsLat){
    (degrees<0 ? output = "S"+output : output = "N"+output);
  }
  else{
    (degrees<0 ? output = "W"+output : output = "E"+output);
  }
  return output;  
}