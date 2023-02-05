#ifndef STDDB_HPP
#define STDDB_HPP

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <math.h>

#include "./DbCommonTypes.hpp"
#include "../Common/CommonUtils.hpp"
#include "./StdDbUtils.hpp"

typedef struct Record
{
  uint32_t ID;
  char ICAO[6];
  char LongName[C_LONG_NAME_LEN+1];
  char CountryCode[3];
  char DMEident[5];
  char IATA[4];
  E_NavAidClass Class;
  double Lat, Lon;
  double DmeLat, DmeLon;
  int Elev, DmeElev;
  float MagVar;
  E_LIST_TYPE ListType;
  uint32_t Freq;
  uint8_t Channel;
  E_ChannelMode ChMode;
  E_VhfRange VhfRange;
  E_Surf_Type LongRwySurfType;
  uint32_t LongestRWYlength;
  E_AptPubMil AptUsage;
  bool AptIfrCapable;
  int TimeZoneOffset;
}DbRecord_t;

typedef struct StdDbStatistics{
  uint32_t VHF_size, NDB_size, Enroute_size, GlobalSize,APT_size;
} StdDbStat_t;

class StdDb{
private:
  std::vector<DbRecord_t> Storage; ///vector storing the info read form the A424 files
  StdDbStat_t Statistics; ///struct storing some stats of the Storage vector
  bool DbIsSorted;

  /**
   * @brief Get the Source Files Size
   * @return the bytes size to be allocated in RAM for the DB 
   */
  uint32_t GetSourceFilesSize(std::string A424Dir);

  /**
   * @brief Reads data from the A424 files
   * @return E_DbError 
   */
  E_DbError AcquireArinc424Files(std::string A424Dir);

  /**
   * @brief Reads a Arinc 424 record containg a VHF navaid
   * @param FileRecord the record to be read
   * @param ReturnCode outcome of the operation
   * @return DbRecord_t The acquired information
   */
  DbRecord_t AcquireVhfRecord(std::string FileRecord, E_DbError* ReturnCode);

  /**
   * @brief Reads a Arinc 424 record containg a NDB navaid
   * @param FileRecord the record to be read
   * @param ReturnCode outcome of the operation
   * @return DbRecord_t The acquired information
   */
  DbRecord_t AcquireNdbRecord(std::string FileRecord, E_DbError* ReturnCode);

  /**
   * @brief Reads a Arinc 424 record containg an enroute waypoint
   * @param FileRecord the record to be read
   * @param ReturnCode outcome of the operation
   * @return DbRecord_t The acquired information
   */
  DbRecord_t AcquireEnrRecord(std::string FileRecord, E_DbError* ReturnCode);

  /**
   * @brief Reads a Arinc 424 record containg an airport
   * @param FileRecord the record to be read
   * @param ReturnCode outcome of the operation
   * @return DbRecord_t The acquired information
   */
  DbRecord_t AcquireAptRecord(std::string FileRecord, E_DbError* ReturnCode);

  /**
   * @brief Computes DME channel from a Mhz frequency 
   * @param Freq The navaid frequency
   * @param Channel The computed channel
   * @param Mode The computed channel mode
   */
  void Freq2Channel(uint32_t Freq, uint8_t* Channel, E_ChannelMode* Mode);

  /**
   * @brief Resets a Db Record to zero values
   * @param Record The record to be cleared
   */
  void ClearDbRecord(DbRecord_t* Record);

  /**
   * @brief sorts the Database witha cocktail sort 
   */
  void SortDatabase();

  /**
   * @brief Tells if the first record comes after the second one
   * @param Record1 The first record
   * @param Record2 The second record
   * @return true  if Record1 comes before Record2
   * @return false if Record2 comes before Record1
   */
  bool SortTwoRecords(const DbRecord_t Record1, const DbRecord_t Record2);
public:
  StdDb();
  ~StdDb(){}

 /**
  * @brief Initializes the Db from the stored A424 files
  * @param SortDb tells if the Db is to be sorted
  * @return E_DbError NO_ERROR if all good
  */
  E_DbError StdDbInitialization(bool SortDb, std::string A424Folder);

  /**
   * @brief Get the Statistics for the Db
   * @return A map of the various stats
   */
  std::map<std::string, uint32_t> getStatistics();

  /**
   * @brief Search for SearcKey in the Std-Db
   * @param Searchkey ICAO Code of the point being searched
   * @return A vector of all points having SearchKey as ICAO code
   */
  std::vector<DbRecord_t> Search(std::string Searchkey);

  /**
   * @brief Returns a list of all Points of type ListType 
   * @param ListType 
   * @return std::vector<DbRecord_t> 
   */
  std::vector<DbRecord_t> List(E_LIST_TYPE ListType);
};

#endif 
