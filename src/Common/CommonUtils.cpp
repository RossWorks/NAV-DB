#include "CommonUtils.hpp"

/**
 * this function splits a string according to a programmer-defined char
 */
std::vector<std::string> SplitString(std::string InString, char delimiter){
  uint TokenNumber = 0, N = 0;
  std::string token;
  std::vector <std::string> output;
  const char* Cstring = InString.c_str();
  do{
    if (Cstring[N] != delimiter){
      token += Cstring[N];
    }
    else{
      output.push_back(token);
      TokenNumber += 1;
      token.clear();
    }
    N++;
  }while(Cstring[N] != '\0');
  return output;
}

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
