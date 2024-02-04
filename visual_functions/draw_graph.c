#include "visual_func.h"

void draw_grid(cairo_t *cr, int num_grid_lines_x, int num_grid_lines_y,
               int width, int height) {
  cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
  cairo_set_line_width(cr, 0.5);

  for (int i = 1; i <= num_grid_lines_x; ++i) {
    double x_pos = width / 2 + i * (width / (2 * num_grid_lines_x)) -
                   (num_grid_lines_x * width / (2 * num_grid_lines_x));
    double x_neg = width / 2 - i * (width / (2 * num_grid_lines_x)) +
                   (num_grid_lines_x * width / (2 * num_grid_lines_x));

    cairo_move_to(cr, x_pos, 0);
    cairo_line_to(cr, x_pos, height);
    cairo_move_to(cr, x_neg, 0);
    cairo_line_to(cr, x_neg, height);
  }

  for (int i = 1; i <= num_grid_lines_y; ++i) {
    double y_pos = height / 2 - i * (height / (2 * num_grid_lines_y)) +
                   (num_grid_lines_y * height / (2 * num_grid_lines_y));
    double y_neg = height / 2 + i * (height / (2 * num_grid_lines_y)) -
                   (num_grid_lines_y * height / (2 * num_grid_lines_y));

    cairo_move_to(cr, 0, y_pos);
    cairo_line_to(cr, width, y_pos);
    cairo_move_to(cr, 0, y_neg);
    cairo_line_to(cr, width, y_neg);
  }

  cairo_stroke(cr);
}
void draw_axis(cairo_t *cr, int width, int height) {
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);

  cairo_move_to(cr, 0, height / 2);
  cairo_line_to(cr, width, height / 2);

  cairo_move_to(cr, width - 10, height / 2 - 5);
  cairo_line_to(cr, width, height / 2);
  cairo_line_to(cr, width - 10, height / 2 + 5);
  cairo_stroke(cr);

  cairo_move_to(cr, width / 2, 0);
  cairo_line_to(cr, width / 2, height);

  cairo_move_to(cr, width / 2 - 5, 10);
  cairo_line_to(cr, width / 2, 0);
  cairo_line_to(cr, width / 2 + 5, 10);
  cairo_stroke(cr);

  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 12);
  cairo_move_to(cr, width - 20, height / 2 - 10);
  cairo_show_text(cr, "x");

  cairo_move_to(cr, width / 2 + 10, 20);
  cairo_show_text(cr, "y");

  cairo_stroke(cr);
}

void draw_domain_text_and_scale(cairo_t *cr, double x_min, double x_max,
                                double y_min, double y_max, int width,
                                int height) {
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 12);
  cairo_set_source_rgb(cr, 0, 0, 0);

  gchar *domain_text = g_strdup_printf("x ∈ [%.2f, %.2f], y ∈ [%.2f, %.2f]",
                                       x_min, x_max, y_min, y_max);
  cairo_move_to(cr, 1, 20);
  cairo_show_text(cr, domain_text);
  g_free(domain_text);

  double x_scale = (x_max - x_min) / width;
  double y_scale = (y_max - y_min) / height;

  gchar *scale_text =
      g_strdup_printf("Scale: %.4f (x), %.4f (y)", x_scale, y_scale);
  cairo_move_to(cr, 1, 40);
  cairo_show_text(cr, scale_text);
  g_free(scale_text);

  cairo_stroke(cr);
}

void draw_func(cairo_t *cr, gchar *expr, double x_min, double x_max,
               double y_min, double y_max, int width, int height) {
  cairo_set_source_rgb(cr, 1, 0, 0);
  cairo_set_line_width(cr, 2);

  double x_step = (x_max - x_min) / 5000;

  double prev_y;

  for (double x = x_min + x_step; x <= x_max; x += x_step) {
    double y;
    gchar *x_val = g_strdup_printf("%lf", x);
    gchar *expr_with_x_value = replace_substring(expr, "x", x_val);
    g_free(x_val);

    if (eval_expr(expr_with_x_value, &y) == 0 && isfinite(y)) {
      double x_pixel = width / 2 + x * width / (x_max - x_min);
      double y_pixel = height / 2 - y * height / (y_max - y_min);

      if (isnan(prev_y) || !isfinite(prev_y))
        cairo_move_to(cr, x_pixel, y_pixel);
      else
        cairo_line_to(cr, x_pixel, y_pixel);

      prev_y = y;
    } else
      prev_y = NAN;

    g_free(expr_with_x_value);
  }

  cairo_stroke(cr);
}

void draw_graph(cairo_t *cr, gchar *expr, double x_min, double x_max,
                double y_min, double y_max, int width, int height) {
  if ((x_min >= 0 && x_max >= 0) || (x_min <= 0 && x_max <= 0)) {
    double default_x_range = 10.0;
    x_min = -default_x_range / 2;
    x_max = default_x_range / 2;
  }
  if ((y_min >= 0 && y_max >= 0) || (y_min <= 0 && y_max <= 0)) {
    double default_y_range = 10.0;
    y_min = -default_y_range / 2;
    y_max = default_y_range / 2;
  }

  int num_grid_lines_x = 10;
  int num_grid_lines_y = 10;

  draw_grid(cr, num_grid_lines_x, num_grid_lines_y, width, height);
  draw_func(cr, expr, x_min, x_max, y_min, y_max, width, height);
  draw_axis(cr, width, height);
  draw_domain_text_and_scale(cr, x_min, x_max, y_min, y_max, width, height);
}

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
  GraphData *graph_data = (GraphData *)data;

  GtkWidget *text_view = graph_data->text_view;
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  GtkTextIter start, end;
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  gchar *expr = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

  double x_min = graph_data->x_min, x_max = graph_data->x_max;
  double y_min = graph_data->y_min, y_max = graph_data->y_max;

  int width, height;
  gtk_widget_get_size_request(widget, &width, &height);

  draw_graph(cr, expr, x_min, x_max, y_min, y_max, width, height);

  g_free(expr);
  return FALSE;
}

void on_fx_button_clicked(GtkWidget *widget, gpointer data) {
  if (!widget) return;

  GtkWidget *text_view = GTK_WIDGET(data);
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  GtkTextIter start, end;
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  gchar *expr = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

  if (g_utf8_strlen(expr, -1) > 0 && g_strcmp0(expr, "No expression entered")) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter Graph Parameters",
        GTK_WINDOW(gtk_widget_get_toplevel(text_view)), GTK_DIALOG_MODAL, "OK",
        GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);

    GtkWidget *entry_x_min = gtk_entry_new(), *entry_x_max = gtk_entry_new();
    GtkWidget *entry_y_min = gtk_entry_new(), *entry_y_max = gtk_entry_new();
    GtkWidget *label_x_min = gtk_label_new("x_min:");
    GtkWidget *label_x_max = gtk_label_new("x_max:");
    GtkWidget *label_y_min = gtk_label_new("y_min:");
    GtkWidget *label_y_max = gtk_label_new("y_max:");

    gtk_grid_attach(GTK_GRID(grid), label_x_min, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_x_min, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_x_max, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_x_max, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_y_min, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_y_min, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_y_max, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_y_max, 1, 3, 1, 1);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_OK) {
      const gchar *x_min_str = gtk_entry_get_text(GTK_ENTRY(entry_x_min));
      const gchar *x_max_str = gtk_entry_get_text(GTK_ENTRY(entry_x_max));
      const gchar *y_min_str = gtk_entry_get_text(GTK_ENTRY(entry_y_min));
      const gchar *y_max_str = gtk_entry_get_text(GTK_ENTRY(entry_y_max));

      double x_min = atof(x_min_str), x_max = atof(x_max_str);
      double y_min = atof(y_min_str), y_max = atof(y_max_str);

      if (x_min <= x_max && y_min <= y_max) {
        GtkWidget *graph_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(graph_window), "Graph f(x)");
        gtk_window_set_default_size(GTK_WINDOW(graph_window), 600, 600);
        gtk_window_set_resizable(GTK_WINDOW(graph_window), FALSE);

        GtkWidget *drawing_area = gtk_drawing_area_new();
        gtk_widget_set_size_request(drawing_area, 600, 600);

        GraphData *graph_data = g_new(GraphData, 1);
        graph_data->text_view = text_view;
        graph_data->x_min = x_min;
        graph_data->x_max = x_max;
        graph_data->y_min = y_min;
        graph_data->y_max = y_max;

        g_signal_connect(G_OBJECT(drawing_area), "draw",
                         G_CALLBACK(on_draw_event), graph_data);

        gtk_container_add(GTK_CONTAINER(graph_window), drawing_area);
        gtk_widget_show_all(graph_window);
      } else
        gtk_text_buffer_set_text(buffer, "Error: Invalid x or y range", -1);
    }
    gtk_widget_destroy(dialog);
  } else
    gtk_text_buffer_set_text(buffer, "No expression entered", -1);

  g_free(expr);
}
