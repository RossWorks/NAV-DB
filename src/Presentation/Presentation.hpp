#ifndef PRESENTATION_HPP
#define PRESENTATION_HPP

#include <string>
#include <vector>

#include "../Common/CommonUtils.hpp"
#include "../StdDb/StdDb.hpp"
#include "./PresentationUtilities.hpp"

std::string PresentSearchResult(std::vector<DbRecord_t> Result);

std::string PresentVHF(DbRecord_t VHF);

std::string PresentAPT(DbRecord_t APT);

std::string PresentNDB(DbRecord_t NDB);
#endif
