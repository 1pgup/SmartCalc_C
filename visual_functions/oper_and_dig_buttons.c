#include "visual_func.h"

void on_digit_button_clicked(GtkWidget *widget, gpointer data) {
  const char *button_text = gtk_button_get_label(GTK_BUTTON(widget));
  insert_button_text(data, button_text);
}

void on_operator_button_clicked(GtkWidget *widget, gpointer data) {
  const char *operator_text = gtk_button_get_label(GTK_BUTTON(widget));
  gchar *new_text = g_strconcat(" ", operator_text, " ", NULL);

  insert_button_text(data, new_text);
  g_free(new_text);
}