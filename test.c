#include <check.h>
#include <stdlib.h>

#include "calc_algorithm/calc_alg.h"

START_TEST(arithmetic_expr) {
  double res;

  char expr_1[] = "2 + 2";
  int err = eval_expr(expr_1, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, 2 + 2, 1e-7);

  char expr_2[] = "2 * (-2)";
  err = eval_expr(expr_2, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, 2 * (-2), 1e-7);

  char expr_3[] = "2 ^ 2";
  err = eval_expr(expr_3, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, pow(2, 2), 1e-7);

  char expr_4[] = "2 / 2";
  err = eval_expr(expr_4, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, 2 / 2, 1e-7);

  char expr_5[] = "(2 * 3) / 1";
  err = eval_expr(expr_5, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, (2 * 3) / 1, 1e-7);

  char expr_6[] = "5 * (+2)";
  err = eval_expr(expr_6, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, 5 * (+2), 1e-7);

  char expr_7[] = "7 mod 3";
  err = eval_expr(expr_7, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, 7 % 3, 1e-7);

  char expr_8[] = "7 / 0";
  err = eval_expr(expr_8, &res);

  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(expr_with_func) {
  double res;

  char expr_1[] = "cos(3)";
  int err = eval_expr(expr_1, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, cos(3), 1e-7);

  char expr_2[] = "sin(1)";
  err = eval_expr(expr_2, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, sin(1), 1e-7);

  char expr_3[] = "tan(8)";
  err = eval_expr(expr_3, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, tan(8), 1e-7);

  char expr_4[] = "ln(4)";
  err = eval_expr(expr_4, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, log(4), 1e-7);

  char expr_5[] = "log(100)";
  err = eval_expr(expr_5, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, log10(100), 1e-7);

  char expr_6[] = "sqrt(1000000)";
  err = eval_expr(expr_6, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, sqrt(1000000), 1e-7);

  char expr_7[] = "atan(2)";
  err = eval_expr(expr_7, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, atan(2), 1e-7);

  char expr_8[] = "asin(0.04)";
  err = eval_expr(expr_8, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, asin(0.04), 1e-7);

  char expr_9[] = "acos(-0.03)";
  err = eval_expr(expr_9, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, acos(-0.03), 1e-7);
}
END_TEST

START_TEST(misspelled_expr) {
  double res;

  char expr_1[] = "3*****2";
  int err = eval_expr(expr_1, &res);

  ck_assert_int_eq(err, 1);

  char expr_2[] = "sin(12 + 3";
  err = eval_expr(expr_2, &res);

  ck_assert_int_eq(err, 1);

  char expr_3[] = "abcd";
  err = eval_expr(expr_3, &res);

  ck_assert_int_eq(err, 1);

  char expr_4[] = "5 + ;";
  err = eval_expr(expr_4, &res);

  ck_assert_int_eq(err, 1);

  char expr_5[] = "(((9))";
  err = eval_expr(expr_5, &res);

  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(mixed_expr) {
  double res;

  char expr_1[] = "sqrt(3^2 + 8^2)";
  int err = eval_expr(expr_1, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, sqrt(pow(3, 2) + pow(8, 2)), 1e-7);

  char expr_2[] = "(8998 - (125 * 15 + 112 * 37 + 147 * 5)) / 132";
  err = eval_expr(expr_2, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, (8998 - (125 * 15 + 112 * 37 + 147 * 5)) / 132,
                          1e-7);

  char expr_3[] = "log(sqrt(ln(15) * 6))";
  err = eval_expr(expr_3, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, log10(sqrt(log(15) * 6)), 1e-7);

  char expr_4[] = "sin(9) * cos(3) * atan(1) * sin(4) * cos(8)";
  err = eval_expr(expr_4, &res);

  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(res, sin(9) * cos(3) * atan(1) * sin(4) * cos(8),
                          1e-7);
}
END_TEST

Suite *suite_test_smart_calc(void) {
  Suite *s = suite_create("Test smart calc");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, arithmetic_expr);
  tcase_add_test(tc, expr_with_func);
  tcase_add_test(tc, misspelled_expr);
  tcase_add_test(tc, mixed_expr);

  suite_add_tcase(s, tc);
  return s;
}

int main() {
  int number_failed = 0;
  SRunner *sr;

  sr = srunner_create(NULL);

  srunner_add_suite(sr, suite_test_smart_calc());
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);

  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
