#include "./MainWindow.hpp"

MainWindow::MainWindow(){
  set_title("NAV DB GEN");

  set_child(MainGrid);

  CmdLoad.set_label("LOAD DB");
  CmdLoad.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::LoadDb_F));

  CmdSearch.set_label("SEARCH");
  CmdSearch.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::SearchDb_F));

  MainGrid.attach(ResultsGrid,0,0,1,1);
  MainGrid.attach(TxtSearchKey,0,1,1,1);
  MainGrid.attach(CmdLoad,1,1,1,1);
  MainGrid.attach(CmdSearch,2,1,1,1);
  ResultsGrid.attach(LblIcao1,0,0,1,1);
  ResultsGrid.attach(LblIcao2,0,1,1,1);
  ResultsGrid.attach(LblIcao3,0,2,1,1);
  ResultsGrid.attach(LblIcao4,0,3,1,1);
  ResultsGrid.attach(LblIcao5,0,4,1,1);
  ResultsGrid.attach(LblCountry1,1,0,1,1);
  ResultsGrid.attach(LblCountry2,1,1,1,1);
  ResultsGrid.attach(LblCountry3,1,2,1,1);
  ResultsGrid.attach(LblCountry4,1,3,1,1);
  ResultsGrid.attach(LblCountry5,1,4,1,1);
  ResultsGrid.attach(LblType1,2,0,1,1);
  ResultsGrid.attach(LblType2,2,1,1,1);
  ResultsGrid.attach(LblType3,2,2,1,1);
  ResultsGrid.attach(LblType4,2,3,1,1);
  ResultsGrid.attach(LblType5,2,4,1,1);
  
}

void MainWindow::LoadDb_F(){
  std::cout << "LOAD DB\n";
  this->MyStdDb.StdDbInitialization(true, "./Data/");
}

void MainWindow::SearchDb_F(){
  std::string Searchkey = this->TxtSearchKey.get_text();

  this->SearchResults.clear();
  this->SearchResults= MyStdDb.Search(Searchkey);
  if (this->SearchResults.size()<1){return;}
  try{
    this->LblIcao1.set_text(SearchResults.at(0).ICAO);
    this->LblIcao2.set_text(SearchResults.at(1).ICAO);
    this->LblIcao3.set_text(SearchResults.at(2).ICAO);
    this->LblIcao4.set_text(SearchResults.at(3).ICAO);
    this->LblIcao5.set_text(SearchResults.at(4).ICAO);
  }
  catch (std::exception &e){}

  try{
    this->LblCountry1.set_text(SearchResults.at(0).CountryCode);
    this->LblCountry2.set_text(SearchResults.at(1).CountryCode);
    this->LblCountry3.set_text(SearchResults.at(2).CountryCode);
    this->LblCountry4.set_text(SearchResults.at(3).CountryCode);
    this->LblCountry5.set_text(SearchResults.at(4).CountryCode);
  }
  catch (std::exception &e){}

  try{
    this->LblType1.set_text(PrintClass(SearchResults.at(0).Class,false));
    this->LblType2.set_text(PrintClass(SearchResults.at(1).Class,false));
    this->LblType3.set_text(PrintClass(SearchResults.at(2).Class,false));
    this->LblType4.set_text(PrintClass(SearchResults.at(3).Class,false));
    this->LblType5.set_text(PrintClass(SearchResults.at(4).Class,false));
  }
  catch (std::exception &e){}
}