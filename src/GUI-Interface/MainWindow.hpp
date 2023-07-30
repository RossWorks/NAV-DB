#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <iostream>
#include "../StdDb/StdDb.hpp"
#include "../Common/CommonSettings.hpp"
#include "../Presentation/Presentation.hpp"

class MainWindow : public Gtk::Window{
public:
  MainWindow();
private:
  Settings MySettings;
  StdDb MyStdDb;
  StdDb* StdDbPtr = &MyStdDb;
  Gtk::Grid MainGrid;
  Gtk::Button CmdLoad;
  Gtk::Button CmdSearch;
  Gtk::Entry TxtSearchKey;
  Gtk::Label LblOutput;

  void LoadDb_F();
  void SearchDb_F();
};