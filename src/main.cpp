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
  std::map<std::string,uint32_t> Stats;
  std::string SearchKey;
  std::string CLIout;
  std::vector<DbRecord_t> SearchResult;

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
  }
  return 0;
}
