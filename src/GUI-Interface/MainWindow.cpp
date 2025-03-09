#include "./MainWindow.hpp"

MainWindow::MainWindow(){
  set_title(GuiTitle);

  set_child(MainGrid);

  CmdBuildDb.set_label("BUILD DB (LE)");
}
