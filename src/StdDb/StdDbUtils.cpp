#include "./StdDbUtils.hpp"

Validated_Double ReadLat(std::string FileRecord, char StartIndex){
    Validated_Double output = {0.0, false};
    output.Value = (FileRecord[StartIndex+1]-48)*10 +
                   (FileRecord[StartIndex+2]-48)+
            (float)(FileRecord[StartIndex+3]-48)/6+
            (float)(FileRecord[StartIndex+4]-48)/60;
    if (FileRecord[StartIndex] == 'S'){
      output.Value *= -1;
    }
    output.Status = true;
    return output;
}

Validated_Double ReadLon(std::string FileRecord, char StartIndex){
    Validated_Double output = {0.0, false};
    output.Value = (FileRecord[StartIndex+1]-48)*100 +
                   (FileRecord[StartIndex+2]-48)*10+
                   (FileRecord[StartIndex+3]-48)*1+
            (float)(FileRecord[StartIndex+4]-48)/6+
            (float)(FileRecord[StartIndex+5]-48)/60;
    if (FileRecord[StartIndex] == 'W'){
      output.Value *= -1;
    }
    output.Status = true;
    return output;
}

Validated_Float ReadFloat(std::string FileRecord, char StartIndex, uint8_t Length,
                          uint8_t DecimalDigits){
  Validated_Float output = {0.0, false};
  int RawNumber = 0;
  try{
    RawNumber = std::stoi(FileRecord.substr(StartIndex, Length));
  }
  catch (const std::invalid_argument &e){
    return output;
  }
  output.Value = (float)RawNumber/pow(10,DecimalDigits);
  output.Status = true;
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

Validated_Integer ReadInteger(std::string Filerecord, const char StartIndex,
                              const char FieldLength, const char Resolution){
  Validated_Integer output = {0, false};
  try{
    output.Value += std::stoi(Filerecord.substr(StartIndex, FieldLength));
    output.Status = true;
  }
  catch(std::invalid_argument &e){
    output = {0, false};
  }
  return output;
}

Validated_Integer ReadRwyLength(std::string FileRecord, char StartIndex){
  Validated_Integer output = {0, false};
  output.Value = std::stoi(FileRecord.substr(StartIndex, 5));
  output.Status = true;
  return output;
}

std::string ReadIcaoCode(std::string FileRecord, char StartIndex, char MaxLen){
  int C = 0;
  std::string output = "";
  output = FileRecord.substr(StartIndex, MaxLen);
  for (C=0; C < output.size(); C++){
    if (output.at(C) == ' '){
      output = output.erase(C);
      C--;
    }
  }
  return output;
}

std::string GenerateDbName(int DbVersion, int DbCycle, bool isLittleEndian){
  std::string output;
  std::string strDbCycle;
  int Year = 0, Month = 0, Day = 0;
  using Clock = std::chrono::system_clock;
  std::chrono::time_point<Clock> Now = std::chrono::system_clock::now();
  std::chrono::year_month_day MyDate = std::chrono::floor<std::chrono::days>(Now);
  DbCycle == 0 ? strDbCycle = "XXXX" : strDbCycle = std::to_string(DbCycle);
  Year  = static_cast<int>(MyDate.year());
  Month = static_cast<unsigned>(MyDate.month());
  Day   = static_cast<unsigned>(MyDate.day());
  output = "STDDB." + std::to_string(DbVersion) + "." + strDbCycle + "_" +
           std::to_string(Year)+"_"+std::to_string(Month)+"_"+std::to_string(Day);
  output += (isLittleEndian ? "_LE" : "_BE");
  output += '\0';
  return output;
}

void WriteVhfRecordToBuffer(const DbRecord_t VhfRecord, unsigned char* FileBuffer, bool isLittleEndian){
  int C = 0, SemiCircleCoord = 0, int_MagVar = 0;
  for (C=C_ICD_VHF_OBJECT_ID_BYTEPOS; C<(C_ICD_VHF_OBJECT_ID_BYTEPOS+6); C++){
    FileBuffer[C] = VhfRecord.ICAO[C-C_ICD_VHF_OBJECT_ID_BYTEPOS];
  }
  for (C=C_ICD_VHF_COUNTRY_CODE_BYTEPOS; C<(C_ICD_VHF_COUNTRY_CODE_BYTEPOS+4); C++){
    FileBuffer[C] = VhfRecord.CountryCode[C-C_ICD_VHF_COUNTRY_CODE_BYTEPOS];
  }
  switch (VhfRecord.Class){
    case UNKNOWN:  FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 0; break;
    case APT:      FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 0; break;
    case DME:      FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 8; break;
    case ILS:      FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 9; break;
    case ILSDME:   FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 7; break;
    case MILTAC:   FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 6; break;
    case MLSDME:   FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 0; break;
    case NDB:      FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 0; break;
    case TACAN:    FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 4; break;
    case VOR:      FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 1; break;
    case VORDME:   FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 3; break;
    case VORTAC:   FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 5; break;
    case WAYPOINT: FileBuffer[C_ICD_VHF_OBJECT_TYPE_BYTEPOS] = 0; break;
  }
  if (VhfRecord.DMEisCollocated){
    FileBuffer[C_ICD_VHF_DME_COL_BYTEPOS] = 1;
  }
  else{
    FileBuffer[C_ICD_VHF_DME_COL_BYTEPOS] = 0;
  }
  SemiCircleCoord = (int)(VhfRecord.Lat.Value*(pow(2,31))/180.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_VHF_LAT_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_VHF_LAT_BYTEPOS);
  }
  SemiCircleCoord = (int)(VhfRecord.Lon.Value*(pow(2,31))/180.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_VHF_LON_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_VHF_LON_BYTEPOS);
  }
  int_MagVar = (int)(VhfRecord.MagVar.Value*100.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(int_MagVar,FileBuffer+C_ICD_VHF_MAGVAR_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(int_MagVar,FileBuffer+C_ICD_VHF_MAGVAR_BYTEPOS);
  }
  if (isLittleEndian){
    WriteLittleEndian<int>(VhfRecord.Freq.Value,FileBuffer+C_ICD_VHF_FREQ_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(VhfRecord.Freq.Value,FileBuffer+C_ICD_VHF_FREQ_BYTEPOS);
  }
  SemiCircleCoord = (int)(VhfRecord.DmeLat.Value*(pow(2,31))/180.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_VHF_DME_LAT_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_VHF_DME_LAT_BYTEPOS);
  }
  SemiCircleCoord = (int)(VhfRecord.DmeLon.Value*(pow(2,31))/180.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_VHF_DME_LON_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_VHF_DME_LON_BYTEPOS);
  }
}

void WriteNdbRecordToBuffer(const DbRecord_t NdbRecord, unsigned char* FileBuffer, bool isLittleEndian){
  int C=0, SemiCircleCoord=0, int_MagVar=0;
  for (C=0; C<C_ICD_NDB_OBJECT_ID_BYTEPOS+6; C++){
    FileBuffer[C] = NdbRecord.ICAO[C-C_ICD_NDB_OBJECT_ID_BYTEPOS];
  }
  for (C=C_ICD_NDB_COUNTRY_CODE_BYTEPOS; C<(C_ICD_NDB_COUNTRY_CODE_BYTEPOS+4); C++){
    FileBuffer[C] = NdbRecord.CountryCode[C-C_ICD_NDB_COUNTRY_CODE_BYTEPOS];
  }
  SemiCircleCoord = (int)(NdbRecord.Lat.Value*(pow(2,31))/180.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_NDB_LAT_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_NDB_LAT_BYTEPOS);
  }
  SemiCircleCoord = (int)(NdbRecord.Lon.Value*(pow(2,31))/180.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_NDB_LON_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_NDB_LON_BYTEPOS);
  }
  int_MagVar = (int)(NdbRecord.MagVar.Value*100.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(int_MagVar,FileBuffer+C_ICD_NDB_MAGVAR_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(int_MagVar,FileBuffer+C_ICD_NDB_MAGVAR_BYTEPOS);
  }
  if (isLittleEndian){
    WriteLittleEndian<int>(NdbRecord.Freq.Value,FileBuffer+C_ICD_NDB_FREQ_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(NdbRecord.Freq.Value,FileBuffer+C_ICD_NDB_FREQ_BYTEPOS);
  }
}

void WriteWptRecordToBuffer(const DbRecord_t WptRecord, unsigned char* FileBuffer, bool isLittleEndian){
  int C=0, SemiCircleCoord=0;
  for (C=0; C<C_ICD_WPT_OBJECT_ID_BYTEPOS+6; C++){
    FileBuffer[C] = WptRecord.ICAO[C-C_ICD_WPT_OBJECT_ID_BYTEPOS];
  }
  SemiCircleCoord = (int)(WptRecord.Lat.Value*(pow(2,31))/180.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_WPT_LAT_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_WPT_LAT_BYTEPOS);
  }
  SemiCircleCoord = (int)(WptRecord.Lon.Value*(pow(2,31))/180.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_WPT_LON_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_WPT_LON_BYTEPOS);
  }
}

void WriteAptRecordToBuffer(const DbRecord_t AptRecord, unsigned char* FileBuffer, bool isLittleEndian){
  int C = 0, SemiCircleCoord = 0, int_MagVar = 0;
  for (C=C_ICD_APT_OBJECT_ID_BYTEPOS; C<(C_ICD_APT_OBJECT_ID_BYTEPOS+6); C++){
    FileBuffer[C] = AptRecord.ICAO[C-C_ICD_APT_OBJECT_ID_BYTEPOS];
  }
  for (C=C_ICD_APT_COUNTRY_CODE_BYTEPOS; C<(C_ICD_APT_COUNTRY_CODE_BYTEPOS+4); C++){
    FileBuffer[C] = AptRecord.CountryCode[C-C_ICD_APT_COUNTRY_CODE_BYTEPOS];
  }
  for (C=C_ICD_APT_IATA_CODE_BYTEPOS; C<(C_ICD_APT_IATA_CODE_BYTEPOS+4); C++){
    FileBuffer[C] = AptRecord.IATA[C-C_ICD_APT_IATA_CODE_BYTEPOS];
  }
  int_MagVar = (int)(AptRecord.MagVar.Value*100.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(int_MagVar,FileBuffer+C_ICD_APT_MAGVAR_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(int_MagVar,FileBuffer+C_ICD_APT_MAGVAR_BYTEPOS);
  }
  SemiCircleCoord = (int)(AptRecord.Lat.Value*(pow(2,31))/180.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_APT_LAT_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_APT_LAT_BYTEPOS);
  }
  SemiCircleCoord = (int)(AptRecord.Lon.Value*(pow(2,31))/180.0);
  if (isLittleEndian){
    WriteLittleEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_APT_LON_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(SemiCircleCoord,FileBuffer+C_ICD_APT_LON_BYTEPOS);
  }
  if (isLittleEndian){
    WriteLittleEndian<int>(AptRecord.Elev.Value,FileBuffer+C_ICD_APT_ELEVATION_BYTEPOS);
  }
  else{
    WriteBigEndian<int>(AptRecord.Elev.Value,FileBuffer+C_ICD_APT_ELEVATION_BYTEPOS);
  }
}

Validated_Float ReadRwyBearing(std::string FileRecord, char StartIndex){
  Validated_Float output = {0,false};
  int TmpInt = 0, length = 0;
  (FileRecord.at(StartIndex+3) == 'T' ? length = 3 : length = 4);
  try{
    TmpInt = std::stoi(FileRecord.substr(StartIndex, length));
  }
  catch (std::invalid_argument &InvalidBearing){
    return {0, false};
  }
  output.Value = (float)TmpInt;
  if (length == 3){
    output.Value *= -1;
  }
  output.Status = true;
  return output;
}

bool SortTwoRecords(const DbRecord_t Record1,const DbRecord_t Record2){
  return (Record1.ICAO <= Record2.ICAO);
}