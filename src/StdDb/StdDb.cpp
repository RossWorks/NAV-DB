#include "StdDb.hpp"

StdDb::StdDb(){
  this->DbIsSorted = false;
}

/**
 * This function computes how many records are stored in the Arinc 424 files
 * inside the C_DataFolder path. The sum of the physical sizes of every file is 
 * then divided by 133 (132 char + newline) to obtain the total number of lines.
 * This method is inefficient at best, as it does not take in account:
 * 1) unused/skipped records
 * 2) continuation records
 * The function assumes that every line of every file holds an independent
 * reference point.
 */
uint32_t StdDb::GetSourceFilesSize(std::string A424Dir){
  if (!std::filesystem::is_directory(A424Dir)){
    return IO_ERROR;
  };
  std::filesystem::directory_iterator SearchingPath(A424Dir);
  uint32_t output = 0;
  for (const auto FileName: SearchingPath){
    if (FileName.is_regular_file()){
      output += FileName.file_size();
    }
  }
  output /= 133;
  return output;
}

/**
 * This function initializes the Std-DB by loading the info from the A424 files
 * into the RAM. First it deduces how many records the `Storage` vector should
 * hold, by counting how many lines the A424 files hold (it assumes that 1 line 
 * = 1 Reference point), then it reserves the needed memory to avoid a continous
 * reallocation. Sorting of the read info is optional, since it can take quite
 * time. Sorting is mandatory to build a usable DB for NavSys.
 */
E_DbError StdDb::StdDbInitialization(bool SortDb, std::string A424Folder){
  E_DbError output = NO_ERROR;
  uint32_t NewDBSize = this->GetSourceFilesSize(A424Folder);
  if (NewDBSize > this->Storage.max_size()){
    output = OUT_OF_MEMORY;
    return output;
  }
  this->Storage.reserve(NewDBSize);
  this->Statistics.APT_size = 0;
  this->Statistics.Enroute_size = 0;
  this->Statistics.GlobalSize = 0;
  this->Statistics.NDB_size = 0;
  this->Statistics.VHF_size = 0;
  output = this->AcquireArinc424Files(A424Folder);
  if (output != NO_ERROR){
    return output;
  }
  if (SortDb){
    std::cout << "Sorting Db...\n";
    this->SortDatabase();
    std::cout << "DONE\n";
    this->DbIsSorted = true;
  }
  return output;
}

/**
 * This function iterates trough the files inside `C_DataFolder'. For each file,
 * every line is read. According to Section and Subsection chars, the current
 * line is passed to the appropriate function able to read that specific type of
 * record (VHF, APT, NDB, etc...). Each line is sotred inside a 135 chars long
 * string to save time by allocating memory only once. Each line read inceases
 * a counter, used in statistics, to keep track of how many records have been
 * acquired and of what type.
 */
E_DbError StdDb::AcquireArinc424Files(std::string A424Dir){
  if (!std::filesystem::is_directory(A424Dir)){
    return IO_ERROR;
  };
  std::filesystem::directory_iterator SearchingPath(A424Dir);
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
          switch (FileRecord[C_APT_SUBSECTION]){
          case 'A':
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

/**
 * This function reads an Arinc 424 record identified as an NDB navaid.
 */
DbRecord_t StdDb::AcquireNdbRecord(std::string FileRecord, E_DbError* ReturnCode){
  DbRecord_t output;
  std::string SpareString;
  output.ListType = NDB_LIST;
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

/**
 * This function reads an Arinc 424 record identified as a VHF navaid.
 */
DbRecord_t StdDb::AcquireVhfRecord(std::string FileRecord, E_DbError* ReturnCode){
  DbRecord_t output;
  output.ListType = VHF_LIST;
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
  switch (FileRecord[C_VHF_FIGURE_MERIT]){
    case TERMINAL       : output.VhfRange = TERMINAL; break;
    case LOW_ALT        : output.VhfRange = LOW_ALT; break;
    case HI_ALT         : output.VhfRange = HI_ALT; break;
    case EXT_HI_ALT     : output.VhfRange = EXT_HI_ALT; break;
    case VHF_IS_MIL     : output.VhfRange = VHF_IS_MIL; break;
    case OUT_OF_SERVICE : output.VhfRange = OUT_OF_SERVICE; break;
    default : break;
  }
  return output;
}

/**
 * this function searchs the Database for all the points having `SearchKey` as
 * ICAO code. The search is simple, but inefficient, sequential search suitable
 * for unordered Databases
 */
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

/**
 * This function clears a DbRecord by setting all its values to zero
 */
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

/**
 * This function reads an Arinc 424 record identified as an enroute waypoint.
 */
DbRecord_t StdDb::AcquireEnrRecord(std::string FileRecord, E_DbError* ReturnCode){
  DbRecord_t output;
  output.ListType = WP_LIST;
  for (int i = 0; i < 6; i++){
    output.ICAO[i] = FileRecord[C_ICAO_IDENT+i];
  }
  output.ICAO[5] = '\0';
  output.Lat = ReadLat(FileRecord, C_NAVAID_LAT);
  output.Lon = ReadLon(FileRecord, C_NAVAID_LON);
  return output;
}

/**
 * This function reads an Arinc 424 record identified as an airport.
 */
DbRecord_t StdDb::AcquireAptRecord(std::string FileRecord, E_DbError* ReturnCode){
  DbRecord_t output;
  int i = 0;
  output.Class = APT;
  output.ListType = APT_LIST;
  ReadIcaoCode(output.CountryCode, FileRecord, 10, 2);
  ReadIcaoCode(output.ICAO, FileRecord, C_APT_ICAO_IDENT, 4);
  for (i = 0; i < C_LONG_NAME_LEN; i++){
    output.LongName[i] = FileRecord[C_APT_LONG_NAME+i];
  }
  output.LongName[C_LONG_NAME_LEN] = '\0';
  output.Lat = ReadLat(FileRecord, C_NAVAID_LAT);
  output.Lon = ReadLon(FileRecord, C_NAVAID_LON);
  output.Elev= ReadElev(FileRecord, C_APT_ELEV);
  output.MagVar = ReadMagVar(FileRecord, C_APT_MAGVAR);
  for (i=0; i<3; i++){
    output.IATA[i] = FileRecord[C_APT_IATA_IDENT+i];
  }
  output.IATA[3] = '\0';
  output.LongestRWYlength = 0;
  for (i=0; i<3; i++){
    if (FileRecord[C_APT_RWY_LONG+i]!= ' '){
      output.LongestRWYlength += (FileRecord[C_APT_RWY_LONG+i]-48)*pow(10,4-i);
    }
  }
  output.AptIfrCapable = (FileRecord[C_APT_IS_IFR] == 'Y');
  switch (FileRecord[C_APT_RWY_SURFACE]){
  case 'H': output.LongRwySurfType = HARD_SURFACE;break;
  case 'S': output.LongRwySurfType = SOFT_SURFACE;break;
  case 'W': output.LongRwySurfType = WATER_SURFACE;break;
  case 'U': output.LongRwySurfType = UNDEF_SURFACE;break;
  default : output.LongRwySurfType = UNDEF_SURFACE;break;
  }
  switch (FileRecord[C_APT_USE]){
  case 'C': output.AptUsage = CIVIL;break;
  case 'M': output.AptUsage = MILITARY;break;
  case 'P': output.AptUsage = PRIVATE;break;
  case 'J': output.AptUsage = JOINT;break;
  default : output.AptUsage = PRIVATE;break;
  }
  output.TimeZoneOffset = 0;
  if (FileRecord[C_APT_TIMEZONE] == ' '){}
  else if (FileRecord[C_APT_TIMEZONE] == 'Z'){
    output.TimeZoneOffset += 0*60;
  }
  else if (FileRecord[C_APT_TIMEZONE] <= 'M'){
    output.TimeZoneOffset += -1*(FileRecord[C_APT_TIMEZONE]-'A'+1)*60;
    output.TimeZoneOffset += -1*(FileRecord[C_APT_TIMEZONE+1]-'0'*10);
    output.TimeZoneOffset += -1*(FileRecord[C_APT_TIMEZONE+2]-'0');
  }
  else if (FileRecord[C_APT_TIMEZONE] >= 'N'){
    output.TimeZoneOffset += (FileRecord[C_APT_TIMEZONE]-'N'+1)*60;
    output.TimeZoneOffset += FileRecord[C_APT_TIMEZONE+1]-'0'*10;
    output.TimeZoneOffset += FileRecord[C_APT_TIMEZONE+2]-'0';
  }
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

std::vector<DbRecord_t> StdDb::List(E_LIST_TYPE ListType){
  int i = 0, C = 0;
  std::vector<DbRecord_t> output;
  switch (ListType){
    case VHF_LIST:
      output.reserve(this->Statistics.VHF_size);
      for (i = 0; i < this->Statistics.GlobalSize; i++){
        if (this->Storage[i].Class == VOR || this->Storage[i].Class == VORDME ||
            this->Storage[i].Class == DME || this->Storage[i].Class == VORTAC ||
            this->Storage[i].Class == ILS || this->Storage[i].Class == ILSDME ||
            this->Storage[i].Class == TACAN){
          output[C] = this->Storage[i];
          C++;
        }
      }
      break;
    default:
      break;
  }
  return output;
}

bool StdDb::SortTwoRecords(const DbRecord_t Record1,const DbRecord_t Record2){
  std::string Name1(Record1.ICAO), Name2(Record2.ICAO);
  if (Record1.ListType < Record2.ListType){return true;}
  if (Record1.ListType > Record2.ListType){return false;}
  if (Record1.ListType == Record2.ListType){
    if (Name1 <= Name2){return true;}
    if (Name1 > Name2){return false;}
  }
  return false;
}

void StdDb::SortDatabase(){
  bool Swapped = false;
  uint64_t Iteration = 0;
  float progress = 0, old_progress = 0;
  int i = 0, start = 0, end = this->Statistics.GlobalSize-1;
  DbRecord_t AUX;
  do{
    progress = (float)Iteration/(this->Statistics.GlobalSize/2)*100;
    if ((progress - old_progress) > 0.1){
      printf("\r %.2f %", progress);
      old_progress = progress;
    }
    Iteration++;
    for (i = start; i <= end; i++){
      if (this->SortTwoRecords(this->Storage[i], this->Storage[i+1])){
        AUX = this->Storage[i];
        this->Storage[i] = this->Storage[i+1];
        this->Storage[i+1] = AUX;
        Swapped = true;
      }
    }
    if (!Swapped){break;}
    Swapped = false;
    end--;
    for (i = end; i > start; i--){
      if (this->SortTwoRecords(this->Storage[i], this->Storage[i-1])){
        AUX = this->Storage[i];
        this->Storage[i] = this->Storage[i-1];
        this->Storage[i-1] = AUX;
        Swapped = true;
      }
    }
    start++;
  } while (Swapped);
    
}