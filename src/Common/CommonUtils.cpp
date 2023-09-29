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

void Write2Log(std::string Message){
  std::ofstream LogFile("./Log.log",std::ios_base::app);
  time_t Now = time(NULL);
  std::string Time = asctime(localtime(&Now));
  LogFile << Time << "\t\t" << Message << "\n\n";
  LogFile.close();
}
