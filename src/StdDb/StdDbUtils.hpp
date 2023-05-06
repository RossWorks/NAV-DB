#ifndef STDDBUTILS_HPP
#define STDDBUTILS_HPP

#include <string>
#include <math.h>
#include <chrono>
#include <iostream>
#include "./DbCommonTypes.hpp"

double ReadLat(std::string FileRecord, char StartIndex);

double ReadLon(std::string FileRecord, char StartIndex);

Validated_Float ReadMagVar(std::string FileRecord, char StartIndex);

Validated_Integer ReadElev(std::string FileRecord, char StartIndex);

void ReadIcaoCode(char* output, std::string FileRecord, char StartIndex, char MaxLen);

std::string GenerateDbName(int DbVersion = 0, int DbCycle = 0);

template<typename T> void WriteBigEndian(T Data, unsigned char* DestionationStart);

template<typename T> void WriteLittleEndian(T Data, unsigned char* DestionationStart);

#include "./StdDbUtilsTemplates.inc"
#endif