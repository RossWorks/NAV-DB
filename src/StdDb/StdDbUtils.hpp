#ifndef STDDBUTILS_HPP
#define STDDBUTILS_HPP

#include <string>
#include <math.h>
#include <chrono>
#include <iostream>
#include "./DbCommonTypes.hpp"

Validated_Double ReadLat(std::string FileRecord, char StartIndex);

Validated_Double ReadLon(std::string FileRecord, char StartIndex);

Validated_Float ReadMagVar(std::string FileRecord, char StartIndex);

Validated_Integer ReadElev(std::string FileRecord, char StartIndex);

Validated_Integer ReadInteger(std::string Filerecord, const char StartIndex,
                              const char FieldLength, const char Resolution);

Validated_Float ReadFloat(std::string FileRecord, char StartIndex, uint8_t Length,
                          uint8_t DecimalDigits);

std::string ReadIcaoCode(std::string FileRecord, char StartIndex, char MaxLen);

std::string GenerateDbName(int DbVersion, int DbCycle, bool isLittleEndian);

void WriteNdbRecordToBuffer(const DbRecord_t NdbRecord, unsigned char* FileBuffer, bool isLittleEndian);

void WriteWptRecordToBuffer(const DbRecord_t WptRecord, unsigned char* FileBuffer, bool isLittleEndian);

void WriteVhfRecordToBuffer(const DbRecord_t VhfRecord, unsigned char* FileBufferStart, bool isLittleEndian);

void WriteAptRecordToBuffer(const DbRecord_t AptRecord, unsigned char* FileBuffer, bool isLittleEndian);

Validated_Float ReadRwyBearing(std::string FileRecord, char StartIndex);

/**
 * @brief Tells if the first record comes before the second one
 * @param Record1 The first record
 * @param Record2 The second record
 * @return true  if Record1 comes before Record2
 */
bool SortTwoRecords(const DbRecord_t Record1, const DbRecord_t Record2);

template<typename T> void WriteBigEndian(T Data, unsigned char* DestionationStart);

template<typename T> void WriteLittleEndian(T Data, unsigned char* DestionationStart);

#include "./StdDbUtilsTemplates.inc"
#endif