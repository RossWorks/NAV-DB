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
#include "./DetailedResultWindow.hpp"

class MainWindow : public Gtk::Window{
public:
  MainWindow();
private:
  Glib::RefPtr<Pango::Context> WidgetContext;
  Pango::FontDescription TextFontDescriptor;

  std::vector<DbRecord_t> SearchResults;
  Settings MySettings;
  StdDb MyStdDb;
  StdDb* StdDbPtr = &MyStdDb;
  
  /*Auxiliary windows*/
  DetailedResultWindow PopUpWindow;

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
  Gtk::Button CmdBuildDb;
  
  /*Start of result presentation widgets*/
  Gtk::Label LblIcao1, LblCountry1, LblType1; Gtk::Button CmdDetResults1;
  Gtk::Label LblIcao2, LblCountry2, LblType2; Gtk::Button CmdDetResults2;
  Gtk::Label LblIcao3, LblCountry3, LblType3; Gtk::Button CmdDetResults3;
  Gtk::Label LblIcao4, LblCountry4, LblType4; Gtk::Button CmdDetResults4;
  Gtk::Label LblIcao5, LblCountry5, LblType5; Gtk::Button CmdDetResults5;
  /*End   of result presentation widgets*/

  void LoadDb_F();
  void SearchDb_F();
  void GetDbInfo();
  void LoadDb_Imp();
  void BuildDb_F();

  void DetRes1();
  void DetRes2();
  void DetRes3();
  void DetRes4();
  void DetRes5();
};