#include "StdDb.hpp"

uint32_t StdDb::GetSourceFilesSize(){
  std::filesystem::directory_iterator SearchingPath(C_DataFolder);
  uint32_t output = 0;
  for (const auto FileName: SearchingPath){
    if (FileName.is_regular_file()){
      output += FileName.file_size();
    }
  }
  output /= 133;
  return output;
}

E_DbError StdDb::StdDbInitialization(){
  uint32_t NewDBSize = this->GetSourceFilesSize();
  if (NewDBSize > this->Storage.max_size()){
    return OUT_OF_MEMORY;
  }
  this->Storage.reserve(NewDBSize);
  this->AcquireArinc424Files();
  return NO_ERROR;
}

E_DbError StdDb::AcquireArinc424Files(){
  std::filesystem::directory_iterator SearchingPath(C_DataFolder);
  std::ifstream A424File;
  std::string FileRecord;
  DbRecord_t TmpDbRecord;
  E_DbError ErrorCode = RECORD_MALFORMED;
  uint32_t CurrentID = 1;
  FileRecord.reserve(135);
  for (const auto FileName: SearchingPath){
    A424File.open(FileName.path(),std::ios_base::in);
    while (getline(A424File,FileRecord))
    {
      switch(FileRecord[C_SECTION_CODE]){
        case 'D':
          switch(FileRecord[C_SECTION_CODE+1]){
            case C_BLANK_CHAR:
              TmpDbRecord = this->AcquireVhfRecord(FileRecord,&ErrorCode);
              TmpDbRecord.ID = CurrentID;
              this->Storage[CurrentID] = TmpDbRecord;
              CurrentID++;
              this->Statistics.VHF_size++;
            break;
            case 'B':
              TmpDbRecord = this->AcquireNdbRecord(FileRecord,&ErrorCode);
              TmpDbRecord.ID = CurrentID;
              this->Storage[CurrentID] = TmpDbRecord;
              CurrentID++;
              this->Statistics.NDB_size++;
            break;
          }
        default:
          break;
      }
    }
    A424File.close();
  }
  return NO_ERROR;
}

DbRecord_t StdDb::AcquireNdbRecord(std::string FileRecord, E_DbError* ReturnCode){
  DbRecord_t output;
  for (uint8_t i=0; i<4; i++){
    output.ICAO[i] = FileRecord[C_ICAO_IDENT+i];
    if (output.ICAO[i] == ' '){output.ICAO[i] = '\0';}
  }
  output.ICAO[4] = '\0';
  for (uint8_t i=0; i<2; i++){
    output.CountryCode[i] = FileRecord[C_COUNTRY_CODE+i];
  }
  output.CountryCode[2] = '\0';
  output.Freq = 0;
  for (uint8_t i=0; i<5; i++){
    output.Freq += (FileRecord[C_FREQ+i]-48)*pow(10,6-i);
  }
  switch (FileRecord[C_NAVAID_CLASS]){
    case 'H':
      output.Class = NDB;
      break;
    default:
      break;
  }
  output.Lat = 0;
  output.Lat += (FileRecord[C_NAVAID_LAT+1]-48)*10 + (FileRecord[C_NAVAID_LAT+2]-48);
  output.Lon = 0;
  output.Lat += (FileRecord[C_NAVAID_LON+1]-48)*10 + (FileRecord[C_NAVAID_LON+2]-48);

  return output;
}


DbRecord_t StdDb::AcquireVhfRecord(std::string FileRecord, E_DbError* ReturnCode){
  DbRecord_t output;
  std::string SpareString;
  SpareString.reserve(20);
  char SpareChar = 0;
  for (uint8_t i=0; i<4; i++){
    output.ICAO[i] = FileRecord[C_ICAO_IDENT+i];
    if (output.ICAO[i] == ' '){output.ICAO[i] = '\0';}
  }
  output.ICAO[4] = '\0';
  for (uint8_t i=0; i<2; i++){
    output.CountryCode[i] = FileRecord[C_COUNTRY_CODE+i];
  }
  output.CountryCode[2] = '\0';
  switch (FileRecord[C_NAVAID_CLASS]){
    case 'V':
      switch(FileRecord[C_NAVAID_CLASS+1]){
      case 'D': output.Class = VORDME; break;
      case 'T': output.Class = VORTAC; break;
      case 'M': output.Class = MILTAC; break;
      case 'I': output.Class = ILSDME; break;
      case 'N': output.Class = MLSDME; break;
      case 'P': output.Class = MLSDME; break;
      case ' ': output.Class = VOR;    break;
      default : *ReturnCode = RECORD_MALFORMED; break;
      }
      break;
    case ' ':
      switch(FileRecord[C_NAVAID_CLASS+1]){
      case 'D': output.Class = DME; break;
      case 'T': output.Class = TACAN; break;
      case 'M': output.Class = MILTAC; break;
      case 'I': output.Class = ILSDME; break;
      case 'N': output.Class = MLSDME; break;
      case 'P': output.Class = MLSDME; break;
      case ' ': *ReturnCode  = RECORD_MALFORMED; break; 
      default : *ReturnCode  = RECORD_MALFORMED; break;
    }
    break;
  }
  output.Freq = 0;
  for (uint8_t i=0; i<5; i++){
    output.Freq += (FileRecord[C_FREQ+i]-48)*pow(10,8-i);
  }
  output.Lat = 0;
  output.Lat += (FileRecord[C_NAVAID_LAT+1]-48)*10 + (FileRecord[C_NAVAID_LAT+2]-48);
  output.Lon = 0;
  output.Lat += (FileRecord[C_NAVAID_LON+1]-48)*10 + (FileRecord[C_NAVAID_LON+2]-48);
  return output;
}


std::vector<DbRecord_t> StdDb::Search(std::string Searchkey){
  std::vector<DbRecord_t> output;
  std::string TmpString;
  for (int i = 1; i< this->Statistics.VHF_size; i++){
    TmpString = this->Storage[i].ICAO;
    if (TmpString == Searchkey){
      output.push_back(this->Storage[i]);
    }
  }
  return output;
}
