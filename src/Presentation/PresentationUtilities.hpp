#ifndef PRESENTATION_UTILITIES_HPP
#define PRESENTATION_UTILITIES_HPP

#include <string>
#include <stdint.h>

#include "../Common/CommonUtils.hpp"

enum E_CoordPrecision{
    DECIMAL_DEGREES,
    DEGREES_MINUTES,
    DEGREES_MINUTES_SECONDS
};

std::string RenderFrequency(uint32_t Frequency);

std::string RenderCoord(double Coord, bool IsLat);

#endif