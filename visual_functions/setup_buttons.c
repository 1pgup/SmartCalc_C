#include "visual_func.h"

void create_buttons(GtkWidget *grid, GtkWidget *entry) {
  for (int i = 0; i < 10; i++) {
    gchar *tmp = g_strdup_printf("%d", i);
    GtkWidget *button = gtk_button_new_with_label(tmp);
    setup_button(grid, entry, button, on_digit_button_clicked, BUTTON_WIDTH,
                 BUTTON_HEIGHT, i % 5 + 1, i / 5 + 1, 1, 1);

    g_free(tmp);
  }

  const gchar *operators[] = {
      "+",   "-",   "/",    "*",    "^",    "mod",  ".",  "(",   ")", "cos",
      "sin", "tan", "acos", "asin", "atan", "sqrt", "ln", "log", "x"};
  for (int i = 0; i < 19; i++) {
    GtkWidget *operator_button = gtk_button_new_with_label(operators[i]);
    void (*action)(GtkWidget *, gpointer) = on_digit_button_clicked;
    if (i <= 5) action = on_operator_button_clicked;

    setup_button(grid, entry, operator_button, action, BUTTON_WIDTH,
                 BUTTON_HEIGHT, i % 5 + 1, i / 5 + 3, 1, 1);
  }

  GtkWidget *clear_button = gtk_button_new_with_label("Clear");
  setup_button(grid, entry, clear_button, on_clear_button_clicked, BUTTON_WIDTH,
               BUTTON_HEIGHT, 0, 1, 1, 1);

  GtkWidget *equal_button = gtk_button_new_with_label("=");
  setup_button(grid, entry, equal_button, on_equal_button_clicked, BUTTON_WIDTH,
               BUTTON_HEIGHT, 0, 2, 1, 1);

  GtkWidget *func_button = gtk_button_new_with_label("f(x)");
  setup_button(grid, entry, func_button, on_fx_button_clicked, BUTTON_WIDTH,
               BUTTON_HEIGHT, 0, 3, 1, 1);

  GtkWidget *credit_button = gtk_button_new_with_label("Credit");
  setup_button(grid, entry, credit_button, on_credit_button_clicked,
               BUTTON_WIDTH, BUTTON_HEIGHT, 0, 4, 1, 1);

  GtkWidget *deposit_button = gtk_button_new_with_label("Deposit");
  setup_button(grid, entry, deposit_button, on_deposit_button_clicked,
               BUTTON_WIDTH, BUTTON_HEIGHT, 0, 5, 1, 1);
}
