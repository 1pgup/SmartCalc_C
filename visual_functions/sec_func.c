#include "visual_func.h"

void on_window_closed(GtkWidget *widget) {
  if (!widget) return;
  gtk_main_quit();
}

gchar *replace_substring(const gchar *str, const gchar *old_str,
                         const gchar *new_str) {
  GString *result = g_string_new("");

  const gchar *ptr = str;
  while (*ptr) {
    const gchar *found = g_strstr_len(ptr, -1, old_str);
    if (found) {
      g_string_append_len(result, ptr, found - ptr);
      g_string_append(result, new_str);
      ptr = found + strlen(old_str);
    } else {
      g_string_append(result, ptr);
      break;
    }
  }

  return g_string_free(result, FALSE);
}

void insert_button_text(gpointer data, const char *button_text) {
  GtkWidget *text_view = GTK_WIDGET(data);
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_buffer_insert(buffer, &iter, button_text, -1);
}

void setup_window(GtkWidget *window, gint width, gint height) {
  gtk_window_set_title(GTK_WINDOW(window), "Calculator");
  gtk_widget_set_size_request(window, width, height);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), NULL);
}

void setup_button(GtkWidget *grid, GtkWidget *entry, GtkWidget *button,
                  void (*foo)(GtkWidget *, gpointer), int but_width,
                  int but_height, int col, int row, int width, int height) {
  gtk_widget_set_size_request(button, but_width, but_height);
  gtk_grid_attach(GTK_GRID(grid), button, col, row, width, height);

  g_signal_connect(button, "clicked", G_CALLBACK(foo), entry);
}
