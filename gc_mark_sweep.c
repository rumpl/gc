#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

void mark(baby_object *object) {
  if (object->marked == 1) {
    return;
  }

  object->marked = 1;

  if (object->type == OBJ_PAIR) {
    mark(object->head);
    mark(object->tail);
  }
}

void sweep(baby_vm *vm) {
  baby_object **object = &vm->first_object;

  while (*object) {
    if (!(*object)->marked) {
      baby_object *unreached = *object;

      *object = unreached->next;
      free(unreached);

      vm->num_objects--;
    } else {
      (*object)->marked = 0;
      object = &(*object)->next;
    }
  }
}

void mark_all(baby_vm *vm) {
  for (int i = 0; i < vm->stack_size; i++) {
    mark(vm->stack[i]);
  }
}

void gc(baby_vm *vm) {
  int num_objects = vm->num_objects;

  mark_all(vm);
  sweep(vm);

  vm->max_objects = vm->num_objects * 2;
  printf("Collected %d objects, %d remaining.\n", num_objects - vm->num_objects,
         vm->num_objects);
}
