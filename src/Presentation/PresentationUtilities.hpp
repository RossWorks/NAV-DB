#ifndef PRESENTATION_UTILITIES_HPP
#define PRESENTATION_UTILITIES_HPP

#include <string>
#include <stdint.h>
#include <map>

#include "../Common/CommonUtils.hpp"
#include "../StdDb/DbCommonTypes.hpp"

enum E_CoordPrecision{
    DECIMAL_DEGREES,
    DEGREES_MINUTES,
    DEGREES_MINUTES_SECONDS
};

enum E_TermColors{
  CLEAR,
  BOLD,
  MAGENTA,
  RED,
  ORANGE,
  AMBER,
  YELLOW,
  GREEN,
  CYAN,
  BLUE,
  INDIGO,
  WHITE,
  BLACK
};

extern std::map <E_TermColors, std::string> ForeColors;

std::string InvalidText(uint32_t Size, bool ColorizeText);

//extern std::map <E_TermColors, std::string> BackColors;

std::string RenderFrequency(Validated_Integer Frequency, bool ColorizeText);

std::string RenderCoord(double Coord, bool IsLat);

std::string PrintFloat(float number, int decimals);

std::string PrintClass(E_NavAidClass Class, bool ColorizeText);

std::string PrintMagVar(Validated_Float MagVar, bool ColorizeText);

std::string PrintValidatedInteger(Validated_Integer Var, bool ColorizeText);
#endif