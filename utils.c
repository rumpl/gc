#include "utils.h"

#include <stdio.h>

void print_object(baby_object *object) {
  if (object->type == OBJ_INT) {
    printf("%d", object->value);
  } else {
    printf("(");
    print_object(object->head);
    printf(" ");
    print_object(object->tail);
    printf(")");
  }
}

void print_stack(baby_vm *vm) {
  for (int i = 0; i < vm->stack_size; i++) {
    print_object(vm->stack[i]);
    printf(" ");
  }
  printf("\n");
}
