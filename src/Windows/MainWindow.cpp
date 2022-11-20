#include "MainWindow.hpp"

MainWindow::MainWindow(){}
MainWindow::~MainWindow(){}

void MainWindow::Show(){
  auto builder =  Gtk::Builder::create_from_file("./res/Interface.glade");
}
