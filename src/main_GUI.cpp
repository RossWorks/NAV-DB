#include <gtkmm.h>
#include "./GUI-Interface/MainWindow.hpp"
#include "./StdDb/StdDb.hpp"
#include "./Common/CommonSettings.hpp"
int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.gtkmm.examples.base");
  //MyStdDb.StdDbInitialization(true,"./Data/");
  return app->make_window_and_run<MainWindow>(argc, argv);
}