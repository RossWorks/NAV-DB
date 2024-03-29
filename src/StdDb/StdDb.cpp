#include "StdDb.hpp"

StdDb::StdDb(){
  this->DbIsSorted = false;
  this->DbIsValid  = false;
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
  clock_t StartTime = 0, EndTime = 0, ElapTime = 0;
  uint32_t NewDBSize = this->GetSourceFilesSize(A424Folder);
  if(this->DbIsValid){this->DestroyDb();}
  try{
    this->AptStorage.reserve(NewDBSize);
    this->NdbStorage.reserve(NewDBSize);
    this->VhfStorage.reserve(NewDBSize);
    this->WpStorage.reserve(NewDBSize);
  }
  catch (const std::length_error& StdDbAllocationFailure){
    std::cout << "Not enough memory to allocate all Database\n";
    this->DbIsSorted = false;
    this->DbIsValid  = false;
    return OUT_OF_MEMORY;
  }
  this->Statistics.APT_size = 0;
  this->Statistics.Enroute_size = 0;
  this->Statistics.GlobalSize = 0;
  this->Statistics.NDB_size = 0;
  this->Statistics.VHF_size = 0;
  StartTime = clock();
  output = this->AcquireArinc424Files(A424Folder);
  EndTime = clock();
  ElapTime = EndTime - StartTime;
  Write2Log("Arinc files acquired in " + std::to_string(ElapTime*1e3/CLOCKS_PER_SEC) + " ns");
  if (output != NO_ERROR){
    this->DbIsSorted = false;
    this->DbIsValid  = false;
    return output;
  }
  if (SortDb){
    StartTime = clock();
    std::cout << "Sorting Db...\n";
    std::thread AptSorter(&StdDb::SortDatabase, this, &(this->AptStorage));
    std::thread VhfSorter(&StdDb::SortDatabase, this, &(this->VhfStorage));
    std::thread NdbSorter(&StdDb::SortDatabase, this, &(this->NdbStorage));
    std::thread WpSorter(&StdDb::SortDatabase, this, &(this->WpStorage));
    AptSorter.join();
    VhfSorter.join();
    NdbSorter.join();
    WpSorter.join();
    std::cout << "DONE\n";
    EndTime = clock();
    ElapTime = EndTime - StartTime;
    Write2Log("Database ordered in " + std::to_string(ElapTime*1e6/CLOCKS_PER_SEC) + " us");
    this->DbIsSorted = true;
  }
  this->DbIsValid  = true;
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
  RwyRecord_t TmpRwyRecord;
  E_DbError ErrorCode = RECORD_MALFORMED;
  FileRecord.reserve(135);
  this->Statistics.APT_size = 0;
  this->Statistics.NDB_size = 0;
  this->Statistics.VHF_size = 0;
  this->Statistics.Enroute_size = 0;
  for (const auto FileName: SearchingPath){
    A424File.open(FileName.path(),std::ios_base::in);
    while (getline(A424File,FileRecord)){
      ClearDbRecord(&TmpDbRecord);
      if ((FileRecord[C_CONT_INDEX] - '0') > 1){continue;}
      switch(FileRecord[C_SECTION_CODE]){
        case 'D':
          switch(FileRecord[C_SECTION_CODE+1]){
            case C_BLANK_CHAR:
              TmpDbRecord = this->AcquireVhfRecord(FileRecord,&ErrorCode);
              TmpDbRecord.ID = this->Statistics.VHF_size;
              this->VhfStorage.push_back(TmpDbRecord);
              break;
            case 'B':
              TmpDbRecord = this->AcquireNdbRecord(FileRecord,&ErrorCode);
              TmpDbRecord.ID = this->Statistics.NDB_size;
              this->NdbStorage.push_back(TmpDbRecord);
              break;
          }
          break;
        case 'E':
          switch (FileRecord[C_SECTION_CODE+1]){
          case 'A':
            TmpDbRecord = this->AcquireEnrRecord(FileRecord,&ErrorCode);
            TmpDbRecord.ID = this->Statistics.Enroute_size;;
            this->WpStorage.push_back(TmpDbRecord);
            this->Statistics.Enroute_size++;
            break;
          default:
            break;
          }
        case 'P':
          switch (FileRecord[C_APT_SUBSECTION]){
            case 'A':
              if (FileRecord[21] != '0'){break;}
              TmpDbRecord = this->AcquireAptRecord(FileRecord,&ErrorCode);
              TmpDbRecord.ID = this->Statistics.APT_size;
              this->AptStorage.push_back(TmpDbRecord);
              break;
            case 'G':
              if (FileRecord[21] != '0'){break;}
              TmpRwyRecord = this->AcquireRwyRecord(FileRecord,&ErrorCode);
              TmpRwyRecord.ID = this->Statistics.APT_size;
              this->RwyStorage.push_back(TmpDbRecord);
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
  this->Statistics.APT_size = this->AptStorage.size();
  this->Statistics.NDB_size = this->NdbStorage.size();
  this->Statistics.VHF_size = this->VhfStorage.size();
  this->Statistics.Enroute_size = this->WpStorage.size();
  return NO_ERROR;
}

/**
 * This function reads an Arinc 424 record identified as an NDB navaid.
 */
DbRecord_t StdDb::AcquireNdbRecord(std::string FileRecord, E_DbError* ReturnCode){
  DbRecord_t output;
  std::string SpareString;
  output.ListType = NDB_LIST;
  output.ICAO = ReadIcaoCode(FileRecord, C_ICAO_IDENT, 3);
  output.CountryCode = ReadIcaoCode(FileRecord, C_COUNTRY_CODE, 2);
  output.Freq.Value = 0;
  output.Freq.Status = true;
  for (uint8_t i=0; i<5; i++){
    output.Freq.Value += (FileRecord[C_FREQ+i]-48)*pow(10,6-i);
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
  output.ICAO = ReadIcaoCode(FileRecord, C_ICAO_IDENT, 3);
  output.CountryCode = ReadIcaoCode(FileRecord, C_COUNTRY_CODE, 2);
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
  output.Freq.Status = true;
  output.Freq.Value  = 0;
  for (uint8_t i=0; i<5; i++){
    output.Freq.Value += (FileRecord[C_FREQ+i]-48)*pow(10,8-i);
  }
  Freq2Channel(output.Freq.Value, &output.Channel, &output.ChMode);
  output.Lat = ReadLat(FileRecord, C_NAVAID_LAT);
  output.Lon = ReadLon(FileRecord, C_NAVAID_LON);
  output.DmeLat = ReadLat(FileRecord, C_DME_LAT);
  output.DmeLon = ReadLon(FileRecord, C_DME_LON);
  output.DmeElev = ReadElev(FileRecord, C_DME_ELEV);
  if (output.Lat.Value == output.DmeLat.Value && output.Lon.Value == output.DmeLon.Value){
    output.DMEisCollocated = true;
  }
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
  clock_t StartTime = 0, EndTime = 0, ElapTime = 0;
  StartTime = clock();
  if (false/*this->DbIsSorted*/){}
  else{
    output = this->LinearSearch(Searchkey);
  }
  EndTime = clock();
  ElapTime = EndTime - StartTime;
  Write2Log("Found " + std::to_string(output.size()) + " occurencies of " + Searchkey + " in " + std::to_string(ElapTime*1e6 / CLOCKS_PER_SEC) + " us");
  return output;
}

std::vector<DbRecord_t> StdDb::LinearSearch(std::string Searchkey){
  std::vector<DbRecord_t> output;
  std::string TmpString;
  Write2Log("Searching in APT");
  for (int i = 1; i< this->Statistics.APT_size; i++){
    TmpString = this->AptStorage.at(i).ICAO;
    if (TmpString == Searchkey){
      output.push_back(this->AptStorage.at(i));
    }
  }
  Write2Log("Searching in NDB");
  for (int i = 1; i< this->Statistics.NDB_size; i++){
    TmpString = this->NdbStorage.at(i).ICAO;
    if (TmpString == Searchkey){
      output.push_back(this->NdbStorage.at(i));
    }
  }
  Write2Log("Searching in VHF");
  for (int i = 1; i< this->Statistics.VHF_size; i++){
    TmpString = this->VhfStorage.at(i).ICAO;
    if (TmpString == Searchkey){
      output.push_back(this->VhfStorage.at(i));
    }
  }
  Write2Log("Searching in WPT");
  for (int i = 1; i< this->Statistics.Enroute_size; i++){
    TmpString = this->WpStorage.at(i).ICAO;
    if (TmpString == Searchkey){
      output.push_back(this->WpStorage.at(i));
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
  Record->Freq.Value  = 0;
  Record->Freq.Status = false;
  for (i=0; i<5; i++){Record->ICAO[i] = '\0';}
  Record->ID = 0;
  Record->Lat = {0.0, false};
  Record->Lon = {0.0, false};
  Record->MagVar = {0.0, false};
  Record->DMEisCollocated = false;
}

/**
 * This function reads an Arinc 424 record identified as an enroute waypoint.
 */
DbRecord_t StdDb::AcquireEnrRecord(std::string FileRecord, E_DbError* ReturnCode){
  DbRecord_t output;
  output.Class = WAYPOINT;
  output.ListType = WP_LIST;
  output.ICAO = ReadIcaoCode(FileRecord, C_ICAO_IDENT, 5);
  output.CountryCode = ReadIcaoCode(FileRecord, C_COUNTRY_CODE, 2);
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
  output.CountryCode = ReadIcaoCode(FileRecord, 10, 2);
  output.ICAO = ReadIcaoCode(FileRecord, C_APT_ICAO_IDENT, 4);
  output.LongName = ReadIcaoCode(FileRecord, C_APT_LONG_NAME, C_LONG_NAME_LEN);
  output.Lat = ReadLat(FileRecord, C_NAVAID_LAT);
  output.Lon = ReadLon(FileRecord, C_NAVAID_LON);
  output.Elev= ReadElev(FileRecord, C_APT_ELEV);
  output.MagVar = ReadMagVar(FileRecord, C_APT_MAGVAR);
  output.IATA = ReadIcaoCode(FileRecord, C_APT_IATA_IDENT, 3);
  output.LongestRWYlength = 0;
  for (i=0; i<3; i++){
    if (FileRecord[C_APT_RWY_LONG+i]!= ' '){
      output.LongestRWYlength += (FileRecord[C_APT_RWY_LONG+i]-48)*pow(10,4-i);
    }
  }
  output.AptIfrCapable = (FileRecord[C_APT_IS_IFR] == 'Y');
  switch (FileRecord[C_APT_RWY_SURFACE]){
    case 'H': output.LongRwySurfType = HARD_SURFACE;  break;
    case 'S': output.LongRwySurfType = SOFT_SURFACE;  break;
    case 'W': output.LongRwySurfType = WATER_SURFACE; break;
    case 'U': output.LongRwySurfType = UNDEF_SURFACE; break;
    default : output.LongRwySurfType = UNDEF_SURFACE; break;
  }
  switch (FileRecord[C_APT_USE]){
    case 'C': output.AptUsage = CIVIL;    break;
    case 'M': output.AptUsage = MILITARY; break;
    case 'P': output.AptUsage = PRIVATE;  break;
    case 'J': output.AptUsage = JOINT;    break;
    default : output.AptUsage = PRIVATE;  break;
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
  output.RecommendedNavaid = ReadIcaoCode(FileRecord, 64, 4);
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

std::vector<DbRecord_t> StdDb::GetList(E_LIST_TYPE ListType, uint32_t StartNumber,
                                       uint32_t RequiredElements, std::string OrderFromKey){
  std::vector<DbRecord_t> output;
  uint64_t I = 0;
  StartNumber = StartNumber - 1;
  if (!this->DbIsSorted){return output;}
  if (OrderFromKey != ""){
    StartNumber = this->GetClosestMatch(OrderFromKey, ListType);
  }
  output.reserve(RequiredElements);
  switch (ListType){
  case VHF_LIST:
    if (StartNumber > this->Statistics.VHF_size){return output;}
    for (I = 0; I < RequiredElements; I++){
      output.push_back(this->VhfStorage.at(I+StartNumber));
    }
    break;
  case NDB_LIST:
    if (StartNumber > this->Statistics.NDB_size){return output;}
    for (I = 0; I < RequiredElements; I++){
      output.push_back(this->NdbStorage.at(I+StartNumber));
    }
    break;
  case APT_LIST:
    if (StartNumber > this->Statistics.APT_size){return output;}
    for (I = 0; I < RequiredElements; I++){
      output.push_back(this->AptStorage.at(I+StartNumber));
    }
    break;
  case WP_LIST:
    if (StartNumber > this->Statistics.Enroute_size){return output;}
    for (I = 0; I < RequiredElements; I++){
      output.push_back(this->WpStorage.at(I+StartNumber));
    }
    break;
  default:
    break;
  }
  return output;
}

uint32_t StdDb::GetClosestMatch(std::string OrderFromKey, E_LIST_TYPE ListType){
  uint32_t Index = 0;
  if (!this->DbIsSorted){return 0;}
  switch (ListType) {
    case APT_LIST:
      for (DbRecord_t Element: this->AptStorage){
        if (OrderFromKey <= std::string(Element.ICAO)){return Index;}
        Index++;
      }
      break;
    case NDB_LIST:
      for (DbRecord_t Element: this->NdbStorage){
        if (OrderFromKey <= std::string(Element.ICAO)){return Index;}
        Index++;
      }
      break;
    case VHF_LIST:
      for (DbRecord_t Element: this->VhfStorage){
        if (OrderFromKey <= std::string(Element.ICAO)){return Index;}
        Index++;
      }
      break;
    case WP_LIST:
      for (DbRecord_t Element: this->WpStorage){
        if (OrderFromKey <= std::string(Element.ICAO)){return Index;}
        Index++;
      }
      break; 
  }
  return Index;
}

E_DbError StdDb::SortDatabase(std::vector<DbRecord_t>* MyStorage){
  clock_t StartTime = 0, EndTime = 0, ElapTime = 0;
  bool Swapped = false;
  int i = 0, start = 0, end = MyStorage->size()-1;
  DbRecord_t AUX;
  std::string DbType = "";
  switch (MyStorage->at(0).ListType){
    case VHF_LIST: DbType = "VHF"; break;
    case APT_LIST: DbType = "APT"; break;
    case NDB_LIST: DbType = "NDB"; break;
    case WP_LIST:  DbType = "WPT"; break;
    default: break;
  }
  std::sort(MyStorage->begin(), MyStorage->end(), SortTwoRecords);
  EndTime = clock();
  ElapTime = EndTime - StartTime;
  Write2Log(std::to_string(MyStorage->size()) + " "+ DbType+" records sorted in " + std::to_string(ElapTime*1e6/CLOCKS_PER_SEC) + " us");
  return NO_ERROR;
}

E_DbError StdDb::BuildStdDB(std::string Path, bool isLittleEndian){
  std::string Filename = GenerateDbName(1, 0, isLittleEndian);
  const char C_DBNAME[12] = "NAV-DB_TEST";
  std::ofstream OuputFile(Path+Filename,std::ios_base::out);
  int CRC32 = 0, Index = 0, C = 0, SemiCircleCoord = 0;
  uint32_t* PositionPtr = nullptr;
  uint8_t VhfRowBuffer[VHF_TABLE_ROW_SIZE_IN_BYTES] = {0};
  uint8_t AptRowBuffer[APT_TABLE_ROW_SIZE_IN_BYTES] = {0};
  uint8_t NdbRowBuffer[NDB_TABLE_ROW_SIZE_IN_BYTES] = {0};
  uint8_t WptRowBuffer[WPT_TABLE_ROW_SIZE_IN_BYTES] = {0};
  std::vector<uint8_t> VhfTable, AptTable, NdbTable, WptTable;
  unsigned char VhfHeader[TABLE_HEADER_SIZE_IN_BYTES] = {0};
  unsigned char AptHeader[TABLE_HEADER_SIZE_IN_BYTES] = {0};
  unsigned char NdbHeader[TABLE_HEADER_SIZE_IN_BYTES] = {0};
  unsigned char WptHeader[TABLE_HEADER_SIZE_IN_BYTES] = {0};
  unsigned char DbHeader[DB_HEADER_SIZE_IN_BYTES]     = {0};
  VhfTable.reserve(VHF_TABLE_ROW_SIZE_IN_BYTES*this->Statistics.VHF_size);
  AptTable.reserve(APT_TABLE_ROW_SIZE_IN_BYTES*this->Statistics.APT_size);
  NdbTable.reserve(NDB_TABLE_ROW_SIZE_IN_BYTES*this->Statistics.NDB_size);
  WptTable.reserve(WPT_TABLE_ROW_SIZE_IN_BYTES*this->Statistics.Enroute_size);

  /*Airport records preparation*/
  for (DbRecord_t element: this->AptStorage){
    WriteAptRecordToBuffer(element, AptRowBuffer, isLittleEndian);
    for (Index = 0; Index < APT_TABLE_ROW_SIZE_IN_BYTES; Index++){
      AptTable.push_back(AptRowBuffer[Index]);
    }
  }
  if (isLittleEndian){
    WriteLittleEndian<int>(1, AptHeader);
    WriteLittleEndian<int>(this->Statistics.APT_size, AptHeader+4);
  }else{
    WriteBigEndian<int>(1, AptHeader);
    WriteBigEndian<int>(this->Statistics.APT_size, AptHeader+4);
  }

  /*VHF navaid records preparation*/
  for (DbRecord_t element: this->VhfStorage){
    WriteVhfRecordToBuffer(element, VhfRowBuffer, isLittleEndian);
    for (Index = 0; Index < VHF_TABLE_ROW_SIZE_IN_BYTES; Index++){
      VhfTable.push_back(VhfRowBuffer[Index]);
    }
  }
  if (isLittleEndian){
    WriteLittleEndian<int>(2, VhfHeader);
    WriteLittleEndian<int>(this->Statistics.VHF_size, VhfHeader+4);
  }else{
    WriteBigEndian<int>(2, VhfHeader);
    WriteBigEndian<int>(this->Statistics.VHF_size, VhfHeader+4);
  }

  /*NDB records preparation*/
  for (DbRecord_t element: this->NdbStorage){
    WriteNdbRecordToBuffer(element, NdbRowBuffer, isLittleEndian);
    for (Index = 0; Index < NDB_TABLE_ROW_SIZE_IN_BYTES; Index++){
      NdbTable.push_back(NdbRowBuffer[Index]);
    }
  }
  if (isLittleEndian){
    WriteLittleEndian<int>(3, NdbHeader);
    WriteLittleEndian<int>(this->Statistics.NDB_size, NdbHeader+4);
  }else{
    WriteBigEndian<int>(3, NdbHeader);
    WriteBigEndian<int>(this->Statistics.NDB_size, NdbHeader+4);
  }

  /*WPT records preparation*/
  for (DbRecord_t element: this->WpStorage){
    WriteNdbRecordToBuffer(element, WptRowBuffer, isLittleEndian);
    for (Index = 0; Index < WPT_TABLE_ROW_SIZE_IN_BYTES; Index++){
      WptTable.push_back(WptRowBuffer[Index]);
    }
  }
  if (isLittleEndian){
    WriteLittleEndian<int>(4, WptHeader);
    WriteLittleEndian<int>(this->Statistics.Enroute_size, WptHeader+4);
  }else{
    WriteBigEndian<int>(4, WptHeader);
    WriteBigEndian<int>(this->Statistics.Enroute_size, WptHeader+4);
  }
  
  /*Saving DB name*/
  for (C=0; C<12; C++){
    DbHeader[C] = C_DBNAME[C];
  }

  /*Database CRC saving*/
  if (isLittleEndian){
    WriteLittleEndian<int>(830666, DbHeader+20);
  }else{
    WriteBigEndian<int>(830666, DbHeader+20);
  }

  /*Writing DB header to file*/
  for (C=0; C<DB_HEADER_SIZE_IN_BYTES; C++){
    OuputFile.put(DbHeader[C]);
  }
  
  /*Writing Airports header & storage to file*/
  for (C=0; C<TABLE_HEADER_SIZE_IN_BYTES; C++){
    OuputFile.put(AptHeader[C]);
  }
  for (C=0; C<AptTable.size(); C++){
    OuputFile.put(AptTable[C]);
  }

  /*Writing VHF navaids header & storage to file*/
  for (C=0; C<TABLE_HEADER_SIZE_IN_BYTES; C++){
    OuputFile.put(VhfHeader[C]);
  }
  for (C=0; C<VhfTable.size(); C++){
    OuputFile.put(VhfTable[C]);
  }

  /*Writing NDB header & storage to file*/
  for (C=0; C<TABLE_HEADER_SIZE_IN_BYTES; C++){
    OuputFile.put(NdbHeader[C]);
  }
  for (C=0; C<NdbTable.size(); C++){
    OuputFile.put(NdbTable[C]);
  }

  /*Writing WPT header & storage to file*/
  for (C=0; C<TABLE_HEADER_SIZE_IN_BYTES; C++){
    OuputFile.put(WptHeader[C]);
  }
  for (C=0; C<WptTable.size(); C++){
    OuputFile.put(WptTable[C]);
  }

  /*Closing file*/
  OuputFile.close();
  return NO_ERROR;
}

RwyRecord_t StdDb::AcquireRwyRecord(std::string FileRecord, E_DbError* ReturnCode){
  RwyRecord_t output;
  output.AssAirport  = ReadIcaoCode(FileRecord,C_APT_ICAO_IDENT,4);
  output.Lat         = ReadLat(FileRecord, C_RWY_LATITUDE);
  output.Lon         = ReadLon(FileRecord, C_RWY_LONGITUDE);
  output.MagBearing  = ReadRwyBearing(FileRecord,27);
  output.RwyIdent    = ReadIcaoCode(FileRecord,C_RWY_IDENTIFIER,5);
  output.Length      = ReadInteger(FileRecord, C_RWY_LENGTH, 5, 1);
  output.Gradient    = ReadFloat(FileRecord, 51, 5,  2);
  output.Elev        = ReadInteger(FileRecord, C_RWY_ELEV, 6, 1);
  output.Width       = ReadInteger(FileRecord, C_RWY_WIDTH, 3, 1);
  return output; 
}

E_DbError StdDb::DestroyDb(){
  this->DbIsSorted = false;
  this->DbIsValid  = false;
  this->AptStorage.clear();
  this->NdbStorage.clear();
  this->VhfStorage.clear();
  this->WpStorage.clear();
  this->RwyStorage.clear();
  return NO_ERROR;
}