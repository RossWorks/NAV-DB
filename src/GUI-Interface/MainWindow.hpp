#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/dropdown.h>
#include <gtkmm/liststore.h>
#include <iostream>

#include "../StdDb/StdDb.hpp"
#include "../Common/CommonSettings.hpp"
#include "./GuiDecorations.hpp"
#include "GuiConstants.hpp"
#include "GenerationJob.hpp"

class MainWindow : public Gtk::Window{
public:
  MainWindow();
private:
  
  GenerationJob JobList;

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
  
  /*Databse build execution widgets START*/
  Gtk::Button CmdBuildDb;
  /*Databse build execution widgets END*/

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
  Gtk::Entry TxtDbName;
  Gtk::DropDown ListIcdVers;
  Gtk::Entry TxtCountryList;
  Gtk::Entry TxtSrcDir;
  Gtk::Entry TxtAiracCycle;
  Gtk::DropDown ListEndianness;

  //Gtk::ListStore ListJobList;
  Gtk::Button CmdAddJob;
  Gtk::Button CmdDeleteJob;
  Gtk::Button CmdEditJob;
  /*Job definition widgets END*/
};
