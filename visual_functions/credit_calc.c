#include "visual_func.h"

double calculate_annuity_payment(double principal, double annual_interest_rate,
                                 int loan_term) {
  double monthly_interest_rate = annual_interest_rate / 12 / 100;
  double monthly_payment =
      principal *
      (monthly_interest_rate * pow(1 + monthly_interest_rate, loan_term)) /
      (pow(1 + monthly_interest_rate, loan_term) - 1);
  return monthly_payment;
}

double calculate_differentiated_payment(double principal,
                                        double annual_interest_rate,
                                        int loan_term, int month) {
  double monthly_interest_rate = annual_interest_rate / 12 / 100;
  double monthly_payment = (principal / loan_term) +
                           (principal - (month - 1) * (principal / loan_term)) *
                               monthly_interest_rate;
  return monthly_payment;
}

void credit_handler(GtkTextBuffer *buffer, const gchar *crdt_amount_str,
                    const gchar *crdt_term_str, const gchar *intrst_rate_str,
                    const gchar *crdt_type_str) {
  gchar *crdt_amount_str_cpy = g_strdup(crdt_amount_str);
  gchar *crdt_term_str_cpy = g_strdup(crdt_term_str);
  gchar *intrst_rate_str_cpy = g_strdup(intrst_rate_str);

  double credit_amount = 0, interest_rate = 0, credit_term = 0;

  int err1 = eval_expr(crdt_amount_str_cpy, &credit_amount);
  int err2 = eval_expr(crdt_term_str_cpy, &credit_term);
  int err3 = eval_expr(intrst_rate_str_cpy, &interest_rate);

  g_free(crdt_amount_str_cpy);
  g_free(crdt_term_str_cpy);
  g_free(intrst_rate_str_cpy);

  if (!err1 && !err2 && !err3 && credit_amount > 0 && credit_term > 0 &&
      interest_rate > 0 &&
      (!g_strcmp0(crdt_type_str, "1") || !g_strcmp0(crdt_type_str, "2"))) {
    if (!g_strcmp0(crdt_type_str, "1")) {
      double annuity_payment = calculate_annuity_payment(
          credit_amount, floor(interest_rate), credit_term);
      double total_payment_annuity = annuity_payment * credit_term;
      double overpayment_annuity = total_payment_annuity - credit_amount;

      gchar *crdt_inf = g_strdup_printf(
          "Monthly payment: %.2lf RUB, Total payout: %.2lf RUB, Overpayment: "
          "%.2lf RUB",
          annuity_payment, total_payment_annuity, overpayment_annuity);
      gtk_text_buffer_set_text(buffer, crdt_inf, -1);
      g_free(crdt_inf);
    } else {
      double total_payment_differentiated = 0;
      double overpayment_differentiated = 0;

      gchar *crdt_inf = g_strdup("Ежемесячные платежи: ");
      gchar *new_text = NULL;
      for (int i = 1; i <= credit_term; ++i) {
        double diff_payment = calculate_differentiated_payment(
            credit_amount, floor(interest_rate), credit_term, i);
        total_payment_differentiated += diff_payment;
        overpayment_differentiated +=
            diff_payment - credit_amount / credit_term;

        new_text = g_strdup_printf("Month %d: %.2lf RUB ", i, diff_payment);
        crdt_inf = g_strconcat(crdt_inf, new_text, NULL);
        g_free(new_text);
      }

      new_text = g_strdup_printf(
          ", Total payout: %.2lf RUB, Overpayment: %.2lf RUB",
          total_payment_differentiated, overpayment_differentiated);
      crdt_inf = g_strconcat(crdt_inf, new_text, NULL);
      g_free(new_text);

      gtk_text_buffer_set_text(buffer, crdt_inf, -1);
      g_free(crdt_inf);
    }
  } else
    gtk_text_buffer_set_text(buffer, "Error in credit information", -1);
}

void on_credit_button_clicked(GtkWidget *widget, gpointer data) {
  if (!widget) return;

  GtkWidget *text_view = GTK_WIDGET(data);
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  GtkWidget *dialog = gtk_dialog_new_with_buttons(
      "Enter Credit Information",
      GTK_WINDOW(gtk_widget_get_toplevel(text_view)), GTK_DIALOG_MODAL, "OK",
      GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, NULL);
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);

  GtkWidget *crdt_amount = gtk_entry_new(), *crdt_term = gtk_entry_new();
  GtkWidget *intrst_rate = gtk_entry_new(), *crdt_type = gtk_entry_new();

  GtkWidget *label_amount = gtk_label_new("Credit amount, RUB:");
  GtkWidget *label_term = gtk_label_new("Credit term, months:");
  GtkWidget *label_rate = gtk_label_new("Interest rate, \%:");
  GtkWidget *label_type =
      gtk_label_new("Type: annuity - 1, differentiated - 2:");

  gtk_grid_attach(GTK_GRID(grid), label_amount, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), crdt_amount, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label_term, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), crdt_term, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label_rate, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), intrst_rate, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), label_type, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), crdt_type, 1, 3, 1, 1);

  GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  gtk_container_add(GTK_CONTAINER(content_area), grid);

  gtk_widget_show_all(dialog);

  gint response = gtk_dialog_run(GTK_DIALOG(dialog));
  if (response == GTK_RESPONSE_OK) {
    const gchar *crdt_amount_str = gtk_entry_get_text(GTK_ENTRY(crdt_amount));
    const gchar *crdt_term_str = gtk_entry_get_text(GTK_ENTRY(crdt_term));
    const gchar *intrst_rate_str = gtk_entry_get_text(GTK_ENTRY(intrst_rate));
    const gchar *crdt_type_str = gtk_entry_get_text(GTK_ENTRY(crdt_type));

    credit_handler(buffer, crdt_amount_str, crdt_term_str, intrst_rate_str,
                   crdt_type_str);
  }

  gtk_widget_destroy(dialog);
}
