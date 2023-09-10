#include "./GuiDecorations.hpp"

void SetWidgetFont(Gtk::Label* LabelWidget, SimpleFontDescription FontDescription){
  auto context = LabelWidget->get_pango_context();
  auto font = context->get_font_description();
  font.set_family(std::get<0>(FontDescription));
  font.set_size(std::get<1>(FontDescription));
  context->set_font_description(font);
}