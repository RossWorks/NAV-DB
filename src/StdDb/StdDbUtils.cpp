#include "./StdDbUtils.hpp"

double ReadLat(std::string FileRecord, char StartIndex){
    double output = 0.0;
    output = (FileRecord[StartIndex+1]-48)*10 +
             (FileRecord[StartIndex+2]-48)+
             (float)(FileRecord[StartIndex+3]-48)/6+
             (float)(FileRecord[StartIndex+4]-48)/60;
    if (FileRecord[StartIndex] == 'S'){
      output *= -1;
    }
    return output;
}

double ReadLon(std::string FileRecord, char StartIndex){
    double output = 0.0;
    output = (FileRecord[StartIndex+1]-48)*100 +
             (FileRecord[StartIndex+2]-48)*10+
             (FileRecord[StartIndex+3]-48)*1+
             (float)(FileRecord[StartIndex+4]-48)/6+
             (float)(FileRecord[StartIndex+5]-48)/60;
    if (FileRecord[StartIndex] == 'W'){
      output *= -1;
    }
    return output;
}

Validated_Float ReadMagVar(std::string FileRecord, char StartIndex){
  Validated_Float output = {0.0, false};
  for (uint8_t i = 0; i < 4; i++){
    if(FileRecord[StartIndex+1+i] != ' '){
      output.Value += (FileRecord[StartIndex+1+i]-48) * pow(10,2-i);
      output.Status = true;
    }
  }
  if (FileRecord[StartIndex] == 'W'){
    output.Value *= -1;
  }
  return output;
}

Validated_Integer ReadElev(std::string FileRecord, char StartIndex){
  Validated_Integer output = {0, false};
  for (uint8_t i = 0; i<4; i++){
    if (FileRecord[StartIndex+1+i] == ' '){continue;}
    output.Value += (FileRecord[StartIndex+1+i]-48) * pow(10,3-i);
    output.Status = true;
    }
  if (FileRecord[StartIndex] == '-'){output.Value *= -1;}
  return output;
}

void ReadIcaoCode(char* output, std::string FileRecord, char StartIndex, char MaxLen){
  int C = 0;
  for(C=0; C<MaxLen; C++){
    if (output[C] != ' '){
      output[C] = FileRecord[StartIndex+C];
    }
    else{output[C] = '\0';}
  }
  output[MaxLen] = '\0';
}