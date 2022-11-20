#ifndef STDDB_HPP
#define STDDB_HPP

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <math.h>

#include "DbCommonTypes.hpp"
#include "../Common/CommonUtils.hpp"

const std::string C_DataFolder = "./Data/";

typedef struct Record
{
  uint32_t ID;
  char ICAO[5];
  char CountryCode[3];
  E_NavAidClass Class;
  double Lat, Lon;
  float MagVar;
  uint32_t Freq;
  uint8_t Channel;
  E_ChannelMode ChMode;
}DbRecord_t;

typedef struct StdDbStatistics{
  uint16_t VHF_size, NDB_size;
} StdDbStat_t;

class StdDb{
private:
  std::vector<DbRecord_t> Storage;
  StdDbStat_t Statistics;
  uint32_t SetDbSize();
  uint32_t GetSourceFilesSize();
  E_DbError AcquireArinc424Files();
  DbRecord_t AcquireVhfRecord(std::string FileRecord, E_DbError* ReturnCode);
  DbRecord_t AcquireNdbRecord(std::string FileRecord, E_DbError* ReturnCode);
  void Freq2Channel(uint32_t Freq, uint8_t* Channel, E_ChannelMode* Mode);
  void ClearDbRecord(DbRecord_t* Record);
public:
  StdDb(/* args */) {}
  ~StdDb() {}
  E_DbError StdDbInitialization();
  std::vector<DbRecord_t> Search(std::string Searchkey);
};

#endif 
