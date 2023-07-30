#include "./MainWindow.hpp"

MainWindow::MainWindow(){
  set_title("NAV DB GEN");

  set_child(MainGrid);

  CmdLoad.set_label("LOAD DB");
  CmdLoad.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::LoadDb_F));

  CmdSearch.set_label("SEARCH");
  CmdSearch.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::SearchDb_F));

  MainGrid.attach(LblOutput,0,0,1,1);
  MainGrid.attach(TxtSearchKey,0,1,1,1);
  MainGrid.attach(CmdLoad,1,1,1,1);
  MainGrid.attach(CmdSearch,2,1,1,1);
}

void MainWindow::LoadDb_F(){
  std::cout << "LOAD DB\n";
  this->MyStdDb.StdDbInitialization(true, "./Data/");
  this->LblOutput.set_text("DONE");
}

void MainWindow::SearchDb_F(){
  std::string SearchTextOutput = "";
  std::vector<DbRecord_t> SearchResults;
  std::string Searchkey = this->TxtSearchKey.get_text();

  this->LblOutput.set_text("");
  SearchResults= MyStdDb.Search(Searchkey);
  SearchTextOutput.clear();
  SearchTextOutput = PresentSearchResult(SearchResults);
  this->LblOutput.set_text(SearchTextOutput);
}