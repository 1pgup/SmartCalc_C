#include "stack.h"

void init_stack(Stack *stack) { stack->top = NULL; }

bool is_empty(Stack *stack) { return stack->top == NULL; }

void push_number(Stack *stack, double value) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  if (new_node) {
    new_node->num = value;
    new_node->next = stack->top;
    stack->top = new_node;
  }
}

void push_oper(Stack *stack, char op) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  if (new_node) {
    new_node->oper = op;
    new_node->next = stack->top;
    stack->top = new_node;
  }
}

Node pop(Stack *stack) {
  Node *top_node = stack->top;
  Node popped_elem = *top_node;
  stack->top = top_node->next;
  free(top_node);

  return popped_elem;
}

Node peek(Stack *stack) { return *(stack->top); }

void free_stack(Stack *stack) {
  while (!is_empty(stack)) pop(stack);
}