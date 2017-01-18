#include <stdlib.h>

#include "vm.h"
#include "gc_mark_sweep.h"

baby_vm *new_vm()
{
  baby_vm *vm = malloc(sizeof(baby_vm));
  vm->stack_size = 0;
  vm->num_objects = 0;
  vm->max_objects = INITIAL_GC_THRESHHOLD;
  return vm;
}

void push(baby_vm *vm, baby_object *object)
{
  vm->stack[vm->stack_size++] = object;
}

baby_object *pop(baby_vm *vm)
{
  return vm->stack[--vm->stack_size];
}

baby_object *new_object(baby_vm *vm, baby_object_type type)
{
  if (vm->num_objects > vm->max_objects)
  {
    gc(vm);
  }

  baby_object *object = malloc(sizeof(baby_object));
  object->type = type;
  object->next = vm->first_object;
  object->marked = 0;

  vm->first_object = object;
  vm->num_objects++;

  return object;
}

void push_int(baby_vm *vm, int value)
{
  baby_object *object = new_object(vm, OBJ_INT);
  object->value = value;
  push(vm, object);
}

void push_pair(baby_vm *vm)
{
  baby_object *object = new_object(vm, OBJ_PAIR);
  object->tail = pop(vm);
  object->head = pop(vm);

  push(vm, object);
}

void free_vm(baby_vm *vm)
{
  vm->stack_size = 0;
  gc(vm);
  free(vm);
}
