#ifndef VISUAL_FUNC_H
#define VISUAL_FUNC_H

#include <cairo.h>
#include <ctype.h>
#include <gtk/gtk.h>
#include <string.h>

#include "../calc_algorithm/calc_alg.h"

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 30

typedef struct {
  GtkWidget *text_view;
  double x_min;
  double x_max;
  double y_min;
  double y_max;
} GraphData;

void on_window_closed(GtkWidget *widget);
gchar *replace_substring(const gchar *str, const gchar *old_str,
                         const gchar *new_str);
void insert_button_text(gpointer data, const char *button_text);
void setup_window(GtkWidget *window, gint width, gint height);
void setup_button(GtkWidget *grid, GtkWidget *entry, GtkWidget *button,
                  void (*foo)(GtkWidget *, gpointer), int but_width,
                  int but_height, int col, int row, int width, int height);

void create_buttons(GtkWidget *grid, GtkWidget *entry);

void on_credit_button_clicked(GtkWidget *widget, gpointer data);
void on_fx_button_clicked(GtkWidget *widget, gpointer data);
void on_equal_button_clicked(GtkWidget *widget, gpointer data);
void on_clear_button_clicked(GtkWidget *widget, gpointer data);
void on_digit_button_clicked(GtkWidget *widget, gpointer data);
void on_operator_button_clicked(GtkWidget *widget, gpointer data);
void on_deposit_button_clicked(GtkWidget *widget, gpointer data);

#endif /* VISUAL_FUNC_H */
