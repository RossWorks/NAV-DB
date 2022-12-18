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

std::string RenderCoord(double Coord){
  std::string output;
  int degrees = static_cast<int>(Coord);
  float minutes = 0.0;
  minutes = (Coord - (float)degrees) * 60;
  minutes = (static_cast<int>(minutes*100))/100;
  output  = std::to_string(degrees) + "° " + std::to_string(minutes) + "'";
  return output;  
}