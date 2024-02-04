#include "visual_func.h"

void on_clear_button_clicked(GtkWidget *widget, gpointer data) {
  if (!widget) return;

  GtkWidget *text_view = GTK_WIDGET(data);
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  gtk_text_buffer_set_text(buffer, "", -1);
}
