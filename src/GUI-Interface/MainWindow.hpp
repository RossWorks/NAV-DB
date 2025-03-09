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
  Gtk::Button CmdBuildDb;
};
