#ifndef STDDBUTILS_HPP
#define STDDBUTILS_HPP

#include <string>
#include <math.h>
#include "./DbCommonTypes.hpp"

double ReadLat(std::string FileRecord, char StartIndex);

double ReadLon(std::string FileRecord, char StartIndex);

Validated_Float ReadMagVar(std::string FileRecord, char StartIndex);

Validated_Integer ReadElev(std::string FileRecord, char StartIndex);

void ReadIcaoCode(char* output, std::string FileRecord, char StartIndex, char MaxLen);

#endif