#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <iostream>
#include <exception>
#include <thread>

#include "../StdDb/StdDb.hpp"
#include "../Common/CommonSettings.hpp"
#include "./GuiDecorations.hpp"
#include "GuiConstants.hpp"

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

  /*Window widgets*/
  Gtk::Grid MainGrid;

  Gtk::Frame JobsFrame;
  Gtk::Grid JobsGrid;

  Gtk::Frame JobsDefineFrame;
  Gtk::Grid  JobsDefineGrid;

  Gtk::Button CmdBuildDb;

  /*Job definition widgets START*/
  Gtk::Label LblJobName;
  Gtk::Label LblDbName;
  Gtk::Label LblIcdVers;
  Gtk::Label LblCountryList;
  Gtk::Label LblSrcDir;
  Gtk::Label LblAiracCycle;
  Gtk::Label LblEndiannes;

  //std::map <std::string, *Gtk::Label> JobLabeles;

  Gtk::Entry TxtJobName;

  /*Job definition widgets END*/
};
