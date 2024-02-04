#include "calc_alg.h"

#include <float.h>

int get_priority(char op) {
  if (op == 'u') return 5;
  if (op == 's' || op == 'c' || op == 't') return 4;
  if (op == 'l' || op == 'g') return 4;
  if (op == 'a' || op == 'i' || op == 'o') return 4;
  if (op == 'r') return 4;
  if (op == '^') return 3;
  if (op == '*' || op == '/') return 2;
  if (op == 'm') return 2;
  if (op == '+' || op == '-') return 1;

  return 0;
}

int perform_oper(double operand1, double operand2, char operator,
                 double * result) {
  int err = 0;
  switch (operator) {
    case '+':
      *result = operand1 + operand2;
      break;
    case '-':
      *result = operand1 - operand2;
      break;
    case '*':
      *result = operand1 * operand2;
      break;
    case '/':
      if (fabs(operand2) >= 1e-7)
        *result = operand1 / operand2;
      else
        err = 1;
      break;
    case '^':
      *result = pow(operand1, operand2);
      break;
    case 'u':
      *result = -operand2;
      break;
    case 'm':
      *result = fmod(operand1, operand2);
      break;
    case 's':
      *result = sin(operand2);
      break;
    case 'c':
      *result = cos(operand2);
      break;
    case 't':
      *result = tan(operand2);
      break;
    case 'l':
      *result = log(operand2);
      break;
    case 'g':
      *result = log10(operand2);
      break;
    case 'r':
      *result = sqrt(operand2);
      break;
    case 'a':
      *result = atan(operand2);
      break;
    case 'i':
      *result = asin(operand2);
      break;
    case 'o':
      *result = acos(operand2);
      break;
    default:
      err = 1;
  }

  return err;
}

int handle_func(char fun_name[], Stack *operator_stack) {
  int res = 0;

  if (strcmp(fun_name, "sin") == 0)
    push_oper(operator_stack, 's');
  else if (strcmp(fun_name, "cos") == 0)
    push_oper(operator_stack, 'c');
  else if (strcmp(fun_name, "tan") == 0)
    push_oper(operator_stack, 't');
  else if (strcmp(fun_name, "mod") == 0)
    push_oper(operator_stack, 'm');
  else if (strcmp(fun_name, "ln") == 0)
    push_oper(operator_stack, 'l');
  else if (strcmp(fun_name, "log") == 0)
    push_oper(operator_stack, 'g');
  else if (strcmp(fun_name, "sqrt") == 0)
    push_oper(operator_stack, 'r');
  else if (strcmp(fun_name, "atan") == 0)
    push_oper(operator_stack, 'a');
  else if (strcmp(fun_name, "asin") == 0)
    push_oper(operator_stack, 'i');
  else if (strcmp(fun_name, "acos") == 0)
    push_oper(operator_stack, 'o');
  else
    res = 1;

  return res;
}

int calc(Stack *operand_stack, Stack *operator_stack) {
  int err = 0;

  if (is_empty(operator_stack) || is_empty(operand_stack)) return 1;

  char op = pop(operator_stack).oper;
  double operand2 = pop(operand_stack).num;

  double result = 0;
  double operand1 = 0;
  if (!(op == 's' || op == 'c' || op == 't' || op == 'l' || op == 'g' ||
        op == 'r' || op == 'a' || op == 'i' || op == 'o' || op == 'u')) {
    if (!is_empty(operand_stack))
      operand1 = pop(operand_stack).num;
    else
      err = 1;
  }

  if (!err) {
    err = perform_oper(operand1, operand2, op, &result);
    if (!err) push_number(operand_stack, result);
  }

  return err;
}

void remove_spaces(char *str) {
  int len = strlen(str);
  int i = 0, j = 0;

  for (; i < len; i++) {
    if (!isspace((unsigned char)str[i])) str[j++] = str[i];
  }

  str[j] = '\0';
}

int eval_expr(char *expr, double *result) {
  if (!strcmp(expr, "") || !expr) return 1;

  remove_spaces(expr);
  int err = 0;
  size_t i = 0;
  Stack operand_stack, operator_stack;
  init_stack(&operand_stack);
  init_stack(&operator_stack);

  while (expr[i] != '\0' && !err) {
    if (isdigit(expr[i]) || (expr[i] == '.' && isdigit(expr[i + 1]))) {
      double num = strtod(&expr[i], NULL);
      push_number(&operand_stack, num);
      while (isdigit(expr[i]) || expr[i] == '.') i++;
    } else if (expr[i] == '(') {
      if (!(i < strlen(expr) && expr[i + 1] == ')')) {
        push_oper(&operator_stack, '(');
        i++;
      } else
        err = 1;
    } else if (expr[i] == ')') {
      while (!is_empty(&operator_stack) && peek(&operator_stack).oper != '(' &&
             !err)
        err = calc(&operand_stack, &operator_stack);
      if (!err) {
        pop(&operator_stack);
        i++;
      }
    } else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' ||
               expr[i] == '/' || expr[i] == '^') {
      if ((expr[i] == '-') &&
          (i == 0 || expr[i - 1] == '-' || expr[i - 1] == '+' ||
           expr[i - 1] == '*' || expr[i - 1] == '/' || expr[i - 1] == '^' ||
           expr[i - 1] == '(')) {
        push_oper(&operator_stack, 'u');
      } else if ((expr[i] == '+') &&
                 (i == 0 || expr[i - 1] == '-' || expr[i - 1] == '+' ||
                  expr[i - 1] == '*' || expr[i - 1] == '/' ||
                  expr[i - 1] == '^' || expr[i - 1] == '(')) {
      } else {
        while (
            !is_empty(&operator_stack) && peek(&operator_stack).oper != '(' &&
            get_priority(peek(&operator_stack).oper) >= get_priority(expr[i]) &&
            !err)
          err = calc(&operand_stack, &operator_stack);
        if (!err) push_oper(&operator_stack, expr[i]);
      }
      i++;
    } else if (isalpha(expr[i])) {
      char fun_name[5];
      int j = 0;
      while (isalpha(expr[i]) && j < 4) fun_name[j++] = expr[i++];
      fun_name[j] = '\0';

      err = handle_func(fun_name, &operator_stack);
    } else
      err = 1;
  }

  if (!err) {
    while (!is_empty(&operator_stack) && !err)
      err = calc(&operand_stack, &operator_stack);
    if (!err) *result = pop(&operand_stack).num;
  }

  if (!is_empty(&operator_stack) || !is_empty(&operand_stack)) {
    err = 1;
    free_stack(&operand_stack);
    free_stack(&operator_stack);
  }

  if (err) *result = NAN;

  return err;
}
