#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/textview.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/dropdown.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <giomm/listmodel.h>

#include <iostream>
#include <list>

#include "../StdDb/StdDb.hpp"
#include "../Common/CommonSettings.hpp"
#include "./GuiDecorations.hpp"
#include "GuiConstants.hpp"
#include "GenerationJob.hpp"

class MainWindow : public Gtk::Window{
public:
  MainWindow();
private:
  
  std::list<GenerationJob> JobList;

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
  Gtk::Frame FrameJobName;
  Gtk::Frame FrameDbName;
  Gtk::Frame FrameIcdVers;
  Gtk::Frame FrameCountryList;
  Gtk::Frame FrameSrcDir;
  Gtk::Frame FrameAiracCycle;
  Gtk::Frame FrameEndiannes;

  //std::map <std::string, *Gtk::Label> JobLabeles;

  Gtk::Entry TxtJobName;
  Gtk::Entry TxtDbName;
  Gtk::DropDown ListIcdVers;
  Gtk::Entry TxtCountryList;
  Gtk::Entry TxtSrcDir;
  Gtk::Entry TxtAiracCycle;
  Gtk::DropDown ListEndianness;

  Gtk::ScrolledWindow ScrollerJobList;
  Gtk::TextView ListJobList;
  Gtk::Button CmdAddJob;
  Gtk::Button CmdDeleteJob;
  Gtk::Button CmdEditJob;
  /*Job definition widgets END*/

  /* start of callback declarations*/
  void AddJob2List();

  void RemoveJobFromList();

  
};
