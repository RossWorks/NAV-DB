#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <gtkmm-4.0/gtkmm.h>

class MainWindow
{
private:
  uint16_t Width, Height;
public:
  MainWindow();
  ~MainWindow();
  void Show();
};

#endif
