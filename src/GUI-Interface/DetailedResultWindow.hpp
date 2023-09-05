#ifndef DETAILED_SEARCH_RESULTS_WINDOW_HPP
#define DETAILED_SEARCH_RESULTS_WINDOW_HPP

#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/window.h>
#include <gtkmm/label.h>

#include <vector>

#include "../StdDb/DbCommonTypes.hpp"
#include "../Presentation/Presentation.hpp"

class DetailedResultWindow : public Gtk::Window{
public:
  DetailedResultWindow();
  ~DetailedResultWindow();
  void PopulateResult(DbRecord_t SearchResult);

private:
  Gtk::Grid MainGrid;
  Gtk::Label TxtSearchDetails;
  Gtk::Button CmdExit;

};

#endif //DETAILED_SEARCH_RESULTS_WINDOW_HPP