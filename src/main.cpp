// #include <gtkmm-4.0/gtkmm.h>
#include "./StdDb/StdDb.hpp"
#include "./Common/CommonUtils.hpp"
#include "./Presentation/Presentation.hpp"
#include "./Windows/MainWindow.hpp"

MainWindow MyWin;

int MainSetup(StdDb* DBMngPtr){
  DBMngPtr->StdDbInitialization();
  //MyWin.Show();
  return 0;  
}

int main(int argc, char* argv[])
{
  StdDb MyDb;
  std::string SearchKey;
  std::string CLIout;
  std::vector<DbRecord_t> SearchResult;
  MainSetup(&MyDb);
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
