// #include <gtkmm-4.0/gtkmm.h>
#include "./StdDb/StdDb.hpp"
#include "./Common/CommonUtils.hpp"
int MainSetup(StdDb* DBMngPtr){
  DBMngPtr->StdDbInitialization();
  return 0;  
}

int main(int argc, char* argv[])
{
  StdDb MyDb;
  std::string SearchKey;
  std::vector<DbRecord_t> SearchResult;
  MainSetup(&MyDb);
  std::cout << "Enter search key: ";
  std::cin >> SearchKey;
  while (SearchKey != "QUITNOW"){
    SearchResult = MyDb.Search(SearchKey);
    std::cout << "Found " <<std::to_string(SearchResult.size()) << " elements\n";
    for (DbRecord_t element: SearchResult){
      std::cout << element.ICAO << " | " << element.Class << " | "<< RenderFrequency(element.Freq) <<"\n";
    }
    std::cout << "Enter search key: ";
    std::cin >> SearchKey;
  }
  return 0;
}
