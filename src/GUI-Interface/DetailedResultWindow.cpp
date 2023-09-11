#include "./DetailedResultWindow.hpp"

DetailedResultWindow::DetailedResultWindow(){

  auto context = TxtSearchDetails.get_pango_context();
  auto font = context->get_font_description();
  font.set_family("B612");
  font.set_size(14*PANGO_SCALE);
  context->set_font_description(font);

  set_child(MainGrid);
  set_modal();
  set_hide_on_close(true);
  set_resizable(false);
  //set_decorated(false);

  CmdExit.set_label("RETURN");
  CmdExit.signal_clicked().connect(sigc::mem_fun(*this, &DetailedResultWindow::hide));

  MainGrid.attach(TxtSearchDetails,0,0,1,1);
  MainGrid.attach(CmdExit,0,1,1,1);

}

DetailedResultWindow::~DetailedResultWindow(){

}


void DetailedResultWindow::PopulateResult(DbRecord_t SearchResult){
  std::string output = "";
  switch (SearchResult.ListType){
    case APT_LIST: output = PresentAPT(SearchResult, false); break;
    case VHF_LIST: output = PresentVHF(SearchResult, false); break;
    case NDB_LIST: output = PresentNDB(SearchResult, false); break;
    case WP_LIST:  output = PresentWPT(SearchResult, false); break;
    default:       output = "ERROR IN PRESENTATION";  break;
  }
  TxtSearchDetails.set_text(Glib::ustring(output));
}