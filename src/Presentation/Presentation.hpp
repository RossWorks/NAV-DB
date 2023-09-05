#ifndef PRESENTATION_HPP
#define PRESENTATION_HPP

#include <string>
#include <vector>

#include "../Common/CommonUtils.hpp"
#include "../StdDb/StdDb.hpp"
#include "./PresentationUtilities.hpp"

std::string PresentSearchResult(std::vector<DbRecord_t> Result);

std::string PresentVHF(DbRecord_t VHF, bool ColoorizeText);

std::string PresentAPT(DbRecord_t APT, bool ColoorizeText);

std::string PresentNDB(DbRecord_t NDB, bool ColoorizeText);

std::string PresentWPT(DbRecord_t WPT, bool ColoorizeText);

#endif
