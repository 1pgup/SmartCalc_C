#include <ctype.h>
#include <gtk/gtk.h>
#include <string.h>

#include "visual_functions/visual_func.h"

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  setup_window(window, 500, 300);

  GtkWidget *grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  GtkWidget *text_view = gtk_text_view_new();
  gtk_widget_set_size_request(text_view, 500, 100);

  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
  gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 0, 10, 1);

  create_buttons(grid, text_view);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
