#ifndef GUI_CONSTANTS_HPP
#define GUI_CONSTANTS_HPP

#include <pango/pango-font.h>

const char DefaultFontFamily[] = "B612";
const int  DefaultFontSize     = 14 * PANGO_SCALE;

typedef std::tuple<std::string, int> SimpleFontDescription;

const SimpleFontDescription StandardFont = {"B612", 14 * PANGO_SCALE};

#endif