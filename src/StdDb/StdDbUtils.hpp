#ifndef STDDBUTILS_HPP
#define STDDBUTILS_HPP

#include <string>
#include <math.h>

double ReadLat(std::string FileRecord, char StartIndex);

double ReadLon(std::string FileRecord, char StartIndex);

float ReadMagVar(std::string FileRecord, char StartIndex);

int ReadElev(std::string FileRecord, char StartIndex);

#endif