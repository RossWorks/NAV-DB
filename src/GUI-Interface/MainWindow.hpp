#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <iostream>
#include <exception>
#include <thread>

#include "../StdDb/StdDb.hpp"
#include "../Common/CommonSettings.hpp"
#include "../Presentation/Presentation.hpp"

class MainWindow : public Gtk::Window{
public:
  MainWindow();
private:
  std::vector<DbRecord_t> SearchResults;
  Settings MySettings;
  StdDb MyStdDb;
  StdDb* StdDbPtr = &MyStdDb;

  /*Accesories widgets*/
  Gtk::Image DbInfoIcon;

  /*Window widgets*/
  Gtk::Grid MainGrid;
  Gtk::Grid ResultsGrid;
  Gtk::Button CmdLoad;
  Gtk::Button CmdDbInfo;
  Gtk::Label LblDbInfoText;
  Gtk::Button CmdSearch;
  Gtk::Entry TxtSearchKey;
  
  /*Start of result presentation widgets*/
  Gtk::Label LblIcao1, LblCountry1, LblType1, LblLongName1;
  Gtk::Label LblIcao2, LblCountry2, LblType2, LblLongName2;
  Gtk::Label LblIcao3, LblCountry3, LblType3, LblLongName3;
  Gtk::Label LblIcao4, LblCountry4, LblType4, LblLongName4;
  Gtk::Label LblIcao5, LblCountry5, LblType5, LblLongName5;
  /*End   of result presentation widgets*/

  void LoadDb_F();
  void SearchDb_F();
  void GetDbInfo();
  void LoadDb_Imp();
};