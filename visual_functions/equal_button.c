#include "visual_func.h"

void on_equal_button_clicked(GtkWidget *widget, gpointer data) {
  if (!widget) return;

  GtkWidget *text_view = GTK_WIDGET(data);
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  GtkTextIter start, end;
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  gchar *expr = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

  if (g_strrstr(expr, "x") != NULL) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Input x value", GTK_WINDOW(gtk_widget_get_toplevel(text_view)),
        GTK_DIALOG_MODAL, "OK", GTK_RESPONSE_OK, NULL);

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(entry), 255);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_add(GTK_CONTAINER(content_area), entry);

    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
      const gchar *x_value = gtk_entry_get_text(GTK_ENTRY(entry));

      gchar *expr_with_x_value = replace_substring(expr, "x", x_value);

      g_free(expr);
      expr = g_strdup(expr_with_x_value);
      g_free(expr_with_x_value);

      gtk_text_buffer_set_text(buffer, expr, -1);
    }
    gtk_widget_destroy(dialog);
  }

  double result;
  if (eval_expr(expr, &result) == 0) {
    gchar *result_text = g_strdup_printf("%.2f", result);

    gtk_text_buffer_set_text(buffer, result_text, -1);

    g_free(result_text);
  } else
    gtk_text_buffer_set_text(buffer, "Error", -1);

  g_free(expr);
}
