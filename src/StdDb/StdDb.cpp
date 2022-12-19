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
  this->Statistics.APT_size = 0;
  this->Statistics.Enroute_size = 0;
  this->Statistics.GlobalSize = 0;
  this->Statistics.NDB_size = 0;
  this->Statistics.VHF_size = 0;
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
      ClearDbRecord(&TmpDbRecord);
      //if (FileRecord[C_CONT_INDEX] != 0){continue;}
      switch(FileRecord[C_SECTION_CODE]){
        case 'D':
          switch(FileRecord[C_SECTION_CODE+1]){
            case C_BLANK_CHAR:
              TmpDbRecord = this->AcquireVhfRecord(FileRecord,&ErrorCode);
              TmpDbRecord.ID = CurrentID;
              this->Storage[CurrentID] = TmpDbRecord;
              CurrentID++;
              this->Statistics.VHF_size++;
              this->Statistics.GlobalSize++;
              break;
            case 'B':
              TmpDbRecord = this->AcquireNdbRecord(FileRecord,&ErrorCode);
              TmpDbRecord.ID = CurrentID;
              this->Storage[CurrentID] = TmpDbRecord;
              CurrentID++;
              this->Statistics.NDB_size++;
              this->Statistics.GlobalSize++;
              break;
          }
          break;
        case 'E':
          switch (FileRecord[C_SECTION_CODE+1]){
          case 'A':
            TmpDbRecord = this->AcquireEnrRecord(FileRecord,&ErrorCode);
            TmpDbRecord.ID = CurrentID;
            this->Storage[CurrentID] = TmpDbRecord;
            CurrentID++;
            this->Statistics.Enroute_size++;
            this->Statistics.GlobalSize++;
            break;
          default:
            break;
          }
        case 'P':
          switch (FileRecord[C_SECTION_CODE+1]){
          case 'A':
          case ' ':
            if (FileRecord[21] != '0'){break;}
            TmpDbRecord = this->AcquireAptRecord(FileRecord,&ErrorCode);
            TmpDbRecord.ID = CurrentID;
            this->Storage[CurrentID] = TmpDbRecord;
            CurrentID++;
            this->Statistics.APT_size++;
            this->Statistics.GlobalSize++;
            break;
          default:
            break;
          }
          break;
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
  std::string SpareString;
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
  output.Lat = ReadLat(FileRecord, C_NAVAID_LAT);
  output.Lon = ReadLon(FileRecord, C_NAVAID_LON);
  output.MagVar = ReadMagVar(FileRecord, C_NAVAID_MAGVAR);
  return output;
}


DbRecord_t StdDb::AcquireVhfRecord(std::string FileRecord, E_DbError* ReturnCode){
  DbRecord_t output;
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
  output.MagVar = ReadMagVar(FileRecord, C_NAVAID_MAGVAR);
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
  Freq2Channel(output.Freq, &output.Channel, &output.ChMode);
  output.Lat = ReadLat(FileRecord, C_NAVAID_LAT);
  output.Lon = ReadLon(FileRecord, C_NAVAID_LON);
  output.DmeLat = ReadLat(FileRecord, C_DME_LAT);
  output.DmeLon = ReadLon(FileRecord, C_DME_LON);
  output.DmeElev = ReadElev(FileRecord, C_DME_ELEV);
  return output;
}


std::vector<DbRecord_t> StdDb::Search(std::string Searchkey){
  std::vector<DbRecord_t> output;
  std::string TmpString;
  for (int i = 1; i< this->Statistics.GlobalSize; i++){
    TmpString = this->Storage[i].ICAO;
    if (TmpString == Searchkey){
      output.push_back(this->Storage[i]);
    }
  }
  return output;
}

void StdDb::Freq2Channel(uint32_t Freq, uint8_t* Channel, E_ChannelMode* Mode){
  uint32_t  L = 0, ENV = 0;
  float W = 0.0, MyFrequency = (float)Freq/1e6, pre_output = 0.0;
  if (MyFrequency >= 134.40 && MyFrequency <= 135.95){
    W = 134.40;
    L = 1;
  }
  if (MyFrequency >= 108.00 && MyFrequency <= 112.25){
    W = 108.00;
    L = 17;
  }
  if (MyFrequency >= 130.30 && MyFrequency <= 134.25){
    W = 130.30;
    L = 60;
  }
  if (MyFrequency >= 112.30 && MyFrequency <= 117.95){
    W = 112.30;
    L = 70;
  }
  if ((Freq % 100000 - Freq % 10000) == 0){ENV = 0; *Mode = X;}
  else{ENV = 1; *Mode = Y;}
  pre_output = (MyFrequency-(W+0.05*ENV))*10+L;
  *Channel = (uint8_t)pre_output;
}

void StdDb::ClearDbRecord( DbRecord_t *Record){
  int i = 0;
  Record->Channel     = 0;
  Record->ChMode      = X;
  Record->Class       = UNKNOWN;
  for (i=0; i<3; i++){Record->CountryCode[i] = '\0';}
  Record->Freq        = 0;
  for (i=0; i<5; i++){Record->ICAO[i] = '\0';}
  Record->ID = 0;
  Record->Lat = 0.0;
  Record->Lon = 0.0;
  Record->MagVar = 0.0;
}

DbRecord_t StdDb::AcquireEnrRecord(std::string FileRecord, E_DbError* ReturnCode){
  DbRecord_t output;
  for (int i = 0; i < 6; i++){
    output.ICAO[i] = FileRecord[C_ICAO_IDENT+i];
  }
  output.ICAO[5] = '\0';
  output.Lat = ReadLat(FileRecord, C_NAVAID_LAT);
  output.Lon = ReadLon(FileRecord, C_NAVAID_LON);
  return output;
}

DbRecord_t StdDb::AcquireAptRecord(std::string FileRecord, E_DbError* ReturnCode){
  DbRecord_t output;
  int i = 0;
  output.Class = APT;
  for (i = 0; i < 4; i++){
    output.ICAO[i] = FileRecord[6+i];
  }
  output.ICAO[4+1] = '\0';
  for (i = 0; i < 30; i++){
    output.LongName[i] = FileRecord[C_APT_LONG_NAME+i];
  }
  output.ICAO[C_APT_LONG_NAME+30] = '\0';
  output.Lat = ReadLat(FileRecord, C_NAVAID_LAT);
  output.Lon = ReadLon(FileRecord, C_NAVAID_LON);
  output.MagVar = ReadMagVar(FileRecord, C_APT_MAGVAR);

  return output;
}

std::map<std::string, uint32_t> StdDb::getStatistics(){
  std::map<std::string, uint32_t> output;
  output["TOTAL"] = this->Statistics.GlobalSize;
  output["VHF"] = this->Statistics.VHF_size;
  output["NDB"] = this->Statistics.NDB_size;
  output["APT"] = this->Statistics.APT_size;
  output["WPT"] = this->Statistics.Enroute_size;
  return output;
}