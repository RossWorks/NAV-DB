#include "./MainWindow.hpp"

MainWindow::MainWindow(){
  set_title("NAV DB GEN");

  set_child(MainGrid);
  
  DbInfoIcon.set_from_resource("./res/DbInfo.png");

  CmdLoad.set_label("_LOAD DB");
  CmdLoad.set_use_underline(true);
  CmdLoad.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::LoadDb_F));

  CmdDbInfo.set_label("DB INFO");
  //CmdDbInfo.set_child(DbInfoIcon);
  CmdDbInfo.set_sensitive(false);
  CmdDbInfo.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::GetDbInfo));

  CmdSearch.set_label("SEARCH");
  CmdSearch.set_sensitive(false);
  CmdSearch.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::SearchDb_F));

  CmdBuildDb.set_label("BUILD DB (LE)");
  CmdBuildDb.set_sensitive(false);
  CmdBuildDb.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::BuildDb_F));

  CmdDetResults1.set_label("DET INFO");
  CmdDetResults2.set_label("DET INFO");
  CmdDetResults3.set_label("DET INFO");
  CmdDetResults4.set_label("DET INFO");
  CmdDetResults5.set_label("DET INFO");

  CmdDetResults1.hide();
  CmdDetResults2.hide();
  CmdDetResults3.hide();
  CmdDetResults4.hide();
  CmdDetResults5.hide();

  CmdDetResults1.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::DetRes1));
  CmdDetResults2.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::DetRes2));
  CmdDetResults3.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::DetRes3));
  CmdDetResults4.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::DetRes4));
  CmdDetResults5.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::DetRes5));

  MainGrid.attach(ResultsGrid,0,0,3,1);
  MainGrid.attach(TxtSearchKey,0,1,1,1);
  MainGrid.attach(CmdLoad,1,1,1,1);
  MainGrid.attach(CmdSearch,2,1,1,1);
  MainGrid.attach(CmdDbInfo, 1,3,1,1);
  MainGrid.attach(CmdBuildDb, 2,3,1,1);
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

  ResultsGrid.attach(CmdDetResults1,3,0,1,1);
  ResultsGrid.attach(CmdDetResults2,3,1,1,1);
  ResultsGrid.attach(CmdDetResults3,3,2,1,1);
  ResultsGrid.attach(CmdDetResults4,3,3,1,1);
  ResultsGrid.attach(CmdDetResults5,3,4,1,1);
  
  for (int Index = 0; Index < 5; Index++){
    SetWidgetFont(IcaoArray[Index], StandardFont);
    SetWidgetFont(TypeArray[Index], StandardFont);
    SetWidgetFont(CountryArray[Index], StandardFont);
  }
}

void MainWindow::LoadDb_F(){
  std::thread DbSorterThread(&MainWindow::LoadDb_Imp,this);
  TxtSearchKey.set_text("LOAD DB");
  DbSorterThread.detach();
}

void MainWindow::LoadDb_Imp(){
  this->MyStdDb.StdDbInitialization(true, "./Data/");
  CmdSearch.set_sensitive(true);
  CmdDbInfo.set_sensitive(true);
  CmdBuildDb.set_sensitive(true);
}

void MainWindow::SearchDb_F(){
  unsigned int Index = 0;
  E_LIST_TYPE ListType = VHF_LIST;
  std::string OrderFromName = "", SearchKey = "";
  std::string UsrCommand = this->TxtSearchKey.get_text();
  HMI_State DummyState = HMI_START;
  this->SearchResults.clear();
  DummyState = ParseCommand(UsrCommand, &SearchKey, &Index, &ListType, &OrderFromName);
  switch (DummyState){
    case HMI_LIST:
      this->SearchResults = MyStdDb.GetList(ListType, Index, 5, OrderFromName);
      break;
    case HMI_SEARCH:
      this->SearchResults= MyStdDb.Search(SearchKey);
      break;
    default:
      return;
      break;
  }
  if (this->SearchResults.size()<1){TxtSearchKey.set_text("LIST EMPTY");}
  for (Index = 0; Index < 5; Index++){
    if (Index < SearchResults.size()){
      IcaoArray[Index]->set_text(SearchResults.at(Index).ICAO);
      DetResArray[Index]->show();
      CountryArray[Index]->set_text(SearchResults.at(Index).CountryCode);
      TypeArray[Index]->set_text(PrintClass(SearchResults.at(Index).Class,false));
    }
    else{
      IcaoArray[Index]->set_text("");
      DetResArray[Index]->hide();
      CountryArray[Index]->set_text("");
      TypeArray[Index]->set_text("");
    }
  }
  return;
}

void MainWindow::GetDbInfo(){
  std::map<std::string, uint32_t> DbStats;
  DbStats = this->MyStdDb.getStatistics();
}

void MainWindow::BuildDb_F(){
  this->MyStdDb.BuildStdDB("./DbGeneration/");
}


/*these callbacks suck, but for now I just want to call it a day and be done*/
void MainWindow::DetRes1(){
  this->PopUpWindow.PopulateResult(this->SearchResults.at(0));
  this->PopUpWindow.show();
}

void MainWindow::DetRes2(){
  this->PopUpWindow.PopulateResult(this->SearchResults.at(1));
  this->PopUpWindow.show();
}

void MainWindow::DetRes3(){
  this->PopUpWindow.PopulateResult(this->SearchResults.at(2));
  this->PopUpWindow.show();
}

void MainWindow::DetRes4(){
  this->PopUpWindow.PopulateResult(this->SearchResults.at(3));
  this->PopUpWindow.show();
}

void MainWindow::DetRes5(){
  this->PopUpWindow.PopulateResult(this->SearchResults.at(4));
  this->PopUpWindow.show();
}