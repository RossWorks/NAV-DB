#include "./StdDb/StdDb.hpp"
#include "./Common/CommonUtils.hpp"
#include "./Presentation/Presentation.hpp"


int MainSetup(StdDb* DBMngPtr){
  DBMngPtr->StdDbInitialization();
  return 0;  
}

int main(int argc, char* argv[])
{
  StdDb MyDb;
  uint IndexOfSpace = 0, argument = 0;
  std::map<std::string,uint32_t> Stats;
  std::string SearchKey, command;
  std::string CLIout;
  std::vector<DbRecord_t> SearchResult;
  SearchResult.reserve(20);

  system("clear");
  MainSetup(&MyDb);
  Stats = MyDb.getStatistics();
  for (std::pair<std::string,uint32_t> element: Stats){
    std::cout << element.first << ": " << element.second << '\n';
  }
  std::cout << "Enter search key: ";
  std::cin >> SearchKey;
  while (SearchKey != "QUITNOW"){
    SearchResult = MyDb.Search(SearchKey);
    CLIout = PresentSearchResult(SearchResult);
    std::cout << CLIout;
    std::cout << "Enter search key: ";
    std::cin >> SearchKey;
    system("clear");
    if (SearchKey.length() >= 7){
      command  = SearchKey.substr(0,7);
      argument = std::stoi(SearchKey.substr(7, SearchKey.length()-7));
      if (command == "DETAILS" && argument > 0 && argument <= SearchResult.size()){
        switch (SearchResult[argument-1].Class){
          case VOR:
          case VORDME:
          case DME:
            CLIout = PresentVHF(SearchResult[argument-1]);
            break;
          case APT:
            CLIout = PresentAPT(SearchResult[argument-1]);
            break;
          case NDB:
            CLIout = PresentNDB(SearchResult[argument-1]);
            break;
          default:
            CLIout = PresentVHF(SearchResult[argument-1]);
            break;
        }
        std::cout << CLIout;
        std::cout << "Enter search key: ";
        std::cin >> SearchKey;
        system("clear");
      }
    }
  }
  return 0;
}
