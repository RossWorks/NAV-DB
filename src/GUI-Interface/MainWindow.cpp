#include "./MainWindow.hpp"

MainWindow::MainWindow(){
  set_title("NAV DB GEN");

  set_child(MainGrid);
  
  DbInfoIcon.set_from_resource("./res/DbInfo.png");

  CmdLoad.set_label("_LOAD DB");
  CmdLoad.set_use_underline(true);
  CmdLoad.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::LoadDb_F));

  CmdDbInfo.set_label("DB INFO");
  CmdDbInfo.set_sensitive(false);
  //CmdDbInfo.add(DbInfoIcon);
  CmdDbInfo.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::GetDbInfo));

  CmdSearch.set_label("SEARCH");
  CmdSearch.set_sensitive(false);
  CmdSearch.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::SearchDb_F));

  MainGrid.attach(ResultsGrid,0,0,2,1);
  MainGrid.attach(TxtSearchKey,0,1,1,1);
  MainGrid.attach(CmdLoad,1,1,1,1);
  MainGrid.attach(CmdSearch,2,1,1,1);
  MainGrid.attach(CmdDbInfo, 1,3,1,1);
  ResultsGrid.set_column_homogeneous();
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
  CmdSearch.set_sensitive(true);
  CmdDbInfo.set_sensitive(true);
}

void MainWindow::SearchDb_F(){
  std::string Searchkey = this->TxtSearchKey.get_text();

  this->SearchResults.clear();
  this->SearchResults= MyStdDb.Search(Searchkey);
  if (this->SearchResults.size()<1){return;}

  try{
    this->LblIcao1.set_text(SearchResults.at(0).ICAO);
    this->LblCountry1.set_text(SearchResults.at(0).CountryCode);
    this->LblType1.set_text(PrintClass(SearchResults.at(0).Class,false));
  }
  catch (std::exception &e){
    this->LblIcao1.set_text("");
    this->LblCountry1.set_text("");
    this->LblType1.set_text("");
  }

  try{
    this->LblIcao2.set_text(SearchResults.at(1).ICAO);
    this->LblCountry2.set_text(SearchResults.at(1).CountryCode);
    this->LblType2.set_text(PrintClass(SearchResults.at(1).Class,false));
  }
  catch (std::exception &e){
    this->LblIcao2.set_text("");
    this->LblCountry2.set_text("");
    this->LblType2.set_text("");
  }

  try{
    this->LblIcao3.set_text(SearchResults.at(2).ICAO);
    this->LblCountry3.set_text(SearchResults.at(2).CountryCode);
    this->LblType3.set_text(PrintClass(SearchResults.at(2).Class,false));
  }
  catch (std::exception &e){
    this->LblIcao3.set_text("");
    this->LblCountry3.set_text("");
    this->LblType3.set_text("");
  }

  try{
    this->LblIcao4.set_text(SearchResults.at(3).ICAO);
    this->LblCountry4.set_text(SearchResults.at(3).CountryCode);
    this->LblType4.set_text(PrintClass(SearchResults.at(3).Class,false));
  }
  catch (std::exception &e){
    this->LblIcao4.set_text("");
    this->LblCountry4.set_text("");
    this->LblType4.set_text("");
  }

  try{
    this->LblIcao5.set_text(SearchResults.at(4).ICAO);
    this->LblCountry5.set_text(SearchResults.at(4).CountryCode);
    this->LblType5.set_text(PrintClass(SearchResults.at(4).Class,false));
  }
  catch (std::exception &e){
    this->LblIcao5.set_text("");
    this->LblCountry5.set_text("");
    this->LblType5.set_text("");
  }
}

void MainWindow::GetDbInfo(){
  std::map<std::string, uint32_t> DbStats;
  DbStats = this->MyStdDb.getStatistics();
}