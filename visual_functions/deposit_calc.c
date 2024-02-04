#include "visual_func.h"

void free_str(gchar *dpst_amount_str, gchar *period_str, gchar *intrst_rate_str,
              gchar *tax_rate_str, gchar *freq_str) {
  g_free(dpst_amount_str);
  g_free(period_str);
  g_free(intrst_rate_str);
  g_free(tax_rate_str);
  g_free(freq_str);
}

void deposit_handler(GtkTextBuffer *buffer, const gchar *dpst_amount_str,
                     const gchar *period_str, const gchar *intrst_rate_str,
                     const gchar *tax_rate_str, const gchar *freq_str,
                     const gchar *cap_str, gchar *repl_str, gchar *wthdr_str) {
  gchar *dpst_cpy = g_strdup(dpst_amount_str);
  gchar *period_cpy = g_strdup(period_str);
  gchar *intrst_cpy = g_strdup(intrst_rate_str);
  gchar *tax_cpy = g_strdup(tax_rate_str);
  gchar *freq_cpy = g_strdup(freq_str);

  double dpst = 0, period = 0, intrst = 0, tax_rate = 0, frqncy = 0;

  int err1 = eval_expr(dpst_cpy, &dpst);
  int err2 = eval_expr(period_cpy, &period);
  int err3 = eval_expr(intrst_cpy, &intrst);
  int err4 = eval_expr(tax_cpy, &tax_rate);
  int err5 = eval_expr(freq_cpy, &frqncy);

  int err = 0;

  if (!err1 & !err2 && !err3 & !err4 && !err5 && dpst > 0 && period > 0 &&
      intrst > 0 && tax_rate > 0 && frqncy > 0 &&
      (!g_strcmp0(cap_str, "1") || !g_strcmp0(cap_str, "0"))) {
    double dpsts = 0.0;
    gchar *token = strtok(repl_str, " ");
    while (token != NULL) {
      double num = 0;
      if (eval_expr(token, &num)) {
        err = 1;
        break;
      }
      dpsts += num;
      token = strtok(NULL, " ");
    }

    double wthdrwls = 0.0;
    token = strtok(wthdr_str, " ");
    while (token != NULL) {
      double num = 0;
      if (eval_expr(token, &num)) {
        err = 1;
        break;
      }
      wthdrwls += num;
      token = strtok(NULL, " ");
    }

    if (!err) {
      double intrst_accum = 0.0, tax_amount = 0.0;
      double dpst_acc = 0.0, dpsts_acc = 0.0;

      if (!g_strcmp0(cap_str, "0")) {
        dpst += dpsts - wthdrwls;
        intrst_accum = dpst * intrst * period / frqncy / 100;
        tax_amount = dpst * tax_rate / 100;
        dpst += intrst_accum - tax_amount;
      } else {
        dpst_acc = dpst * pow(1 + (intrst / frqncy / 1200), frqncy * period);
        dpst_acc -= dpst;

        dpsts_acc = dpsts * pow(1 + (intrst / frqncy / 1200), frqncy * period);
        dpsts_acc -= dpsts;

        intrst_accum = dpst_acc + dpsts_acc;
        tax_amount = intrst_accum * tax_rate / 100;
        dpst += dpsts + intrst_accum - tax_amount - wthdrwls;
      }

      gchar *dpst_inf = g_strdup_printf(
          "Interest charges: %.2lf RUB, Tax amount: %.2lf RUB, Amount on "
          "deposit at the end of the term: "
          "%.2lf RUB",
          intrst_accum, tax_amount, dpst);
      gtk_text_buffer_set_text(buffer, dpst_inf, -1);
      g_free(dpst_inf);
    } else
      gtk_text_buffer_set_text(buffer, "Error in deposit information", -1);
  } else
    gtk_text_buffer_set_text(buffer, "Error in deposit information", -1);

  free_str(dpst_cpy, period_cpy, intrst_cpy, tax_cpy, freq_cpy);
}

void setup_scrolled_windows(GtkWidget *grid, GtkWidget *scroll_window1,
                            GtkWidget *scroll_window2, GtkWidget *text_view1,
                            GtkWidget *text_view2) {
  gtk_container_add(GTK_CONTAINER(scroll_window1), text_view1);
  gtk_container_add(GTK_CONTAINER(scroll_window2), text_view2);

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window1),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window2),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_grid_attach(GTK_GRID(grid), scroll_window1, 1, 6, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), scroll_window2, 1, 7, 1, 1);

  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view1), GTK_WRAP_WORD_CHAR);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view2), GTK_WRAP_WORD_CHAR);
}

void setup_labels(GtkWidget *grid) {
  GtkWidget *label_amount = gtk_label_new("Deposit amount, RUB:");
  GtkWidget *label_period = gtk_label_new("Placement period, months:");
  GtkWidget *label_intrst_rate = gtk_label_new("Interest rate, \%:");
  GtkWidget *label_tax_rate = gtk_label_new("Tax rate, \%:");
  GtkWidget *label_freq = gtk_label_new("Frequency of payments:");
  GtkWidget *label_cap =
      gtk_label_new("Monthly capitalization: (1 - yes, 0 - no)");
  GtkWidget *label_replenishment = gtk_label_new(
      "List of replenishments, RUB: (enter separated by a space)");
  GtkWidget *label_withdrawal =
      gtk_label_new("List of withdrawals, RUB: (enter separated by a space)");

  gtk_grid_attach(GTK_GRID(grid), label_amount, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label_period, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label_intrst_rate, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label_tax_rate, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label_freq, 0, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label_cap, 0, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label_replenishment, 0, 6, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label_withdrawal, 0, 7, 1, 1);
}

void on_deposit_button_clicked(GtkWidget *widget, gpointer data) {
  if (!widget) return;

  GtkWidget *text_view = GTK_WIDGET(data);
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  GtkWidget *dialog = gtk_dialog_new_with_buttons(
      "Enter Deposit Information",
      GTK_WINDOW(gtk_widget_get_toplevel(text_view)), GTK_DIALOG_MODAL, "OK",
      GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);

  GtkWidget *dpst_amount = gtk_entry_new(), *plcmnt_period = gtk_entry_new();
  GtkWidget *intrst_rate = gtk_entry_new(), *tax_rate = gtk_entry_new();
  GtkWidget *freq = gtk_entry_new(), *capitalization = gtk_entry_new();

  setup_labels(grid);

  gtk_grid_attach(GTK_GRID(grid), dpst_amount, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), plcmnt_period, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), intrst_rate, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), tax_rate, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), freq, 1, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), capitalization, 1, 5, 1, 1);

  GtkWidget *repl = gtk_scrolled_window_new(NULL, NULL);
  GtkWidget *wthdr = gtk_scrolled_window_new(NULL, NULL);
  GtkWidget *repl_entry = gtk_text_view_new();
  GtkWidget *wthdr_entry = gtk_text_view_new();

  setup_scrolled_windows(grid, repl, wthdr, repl_entry, wthdr_entry);

  GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  gtk_container_add(GTK_CONTAINER(content_area), grid);
  gtk_widget_show_all(dialog);

  gint response = gtk_dialog_run(GTK_DIALOG(dialog));
  if (response == GTK_RESPONSE_OK) {
    const gchar *dpst_amount_str = gtk_entry_get_text(GTK_ENTRY(dpst_amount));
    const gchar *period_str = gtk_entry_get_text(GTK_ENTRY(plcmnt_period));
    const gchar *intrst_rate_str = gtk_entry_get_text(GTK_ENTRY(intrst_rate));
    const gchar *tax_rate_str = gtk_entry_get_text(GTK_ENTRY(tax_rate));
    const gchar *freq_str = gtk_entry_get_text(GTK_ENTRY(freq));
    const gchar *cap_str = gtk_entry_get_text(GTK_ENTRY(capitalization));

    GtkTextBuffer *buf_tmp =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(repl_entry));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buf_tmp, &start, &end);
    gchar *repl_str = gtk_text_buffer_get_text(buf_tmp, &start, &end, FALSE);

    buf_tmp = gtk_text_view_get_buffer(GTK_TEXT_VIEW(wthdr_entry));
    gtk_text_buffer_get_bounds(buf_tmp, &start, &end);
    gchar *wthdr_str = gtk_text_buffer_get_text(buf_tmp, &start, &end, FALSE);

    deposit_handler(buffer, dpst_amount_str, period_str, intrst_rate_str,
                    tax_rate_str, freq_str, cap_str, repl_str, wthdr_str);

    g_free(repl_str);
    g_free(wthdr_str);
  }
  gtk_widget_destroy(dialog);
}
