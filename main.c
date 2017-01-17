#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX 256
#define INITIAL_GC_THRESHHOLD 12

typedef enum {
  OBJ_INT,
  OBJ_PAIR
} baby_object_type;

typedef struct s_baby_object
{
  unsigned char marked;
  struct s_baby_object *next;

  baby_object_type type;

  union {
    int value;

    struct
    {
      struct s_baby_object *head;
      struct s_baby_object *tail;
    };
  };
} baby_object;

typedef struct
{
  int num_objects;
  int max_objects;
  baby_object *first_object;
  baby_object *stack[STACK_MAX];
  int stack_size;
} baby_vm;

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

void mark(baby_object *object)
{
  if (object->marked == 1)
  {
    return;
  }

  object->marked = 1;

  if (object->type == OBJ_PAIR)
  {
    mark(object->head);
    mark(object->tail);
  }
}

void sweep(baby_vm *vm)
{
  baby_object **object = &vm->first_object;

  while (*object)
  {
    if (!(*object)->marked)
    {
      baby_object *unreached = *object;

      *object = unreached->next;
      free(unreached);

      vm->num_objects--;
    }
    else
    {
      (*object)->marked = 0;
      object = &(*object)->next;
    }
  }
}

void markAll(baby_vm *vm)
{
  for (int i = 0; i < vm->stack_size; i++)
  {
    mark(vm->stack[i]);
  }
}

void gc(baby_vm *vm)
{
  int num_objects = vm->num_objects;

  markAll(vm);
  sweep(vm);

  vm->max_objects = vm->num_objects * 2;
  printf("Collected %d objects, %d remaining.\n", num_objects - vm->num_objects, vm->num_objects);
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
  baby_object *object = new_object(vm, OBJ_INT);
  object->tail = pop(vm);
  object->head = pop(vm);

  push(vm, object);
}

void free_vm(baby_vm* vm) 
{
  vm->stack_size = 0;
  gc(vm);
  free(vm);
}

int main()
{
  baby_vm* vm = new_vm();

  push_int(vm, 1);
  push_int(vm, 2);
  push_int(vm, 3);
  push_int(vm, 4);
  push_int(vm, 5);
  push_int(vm, 6);
  push_int(vm, 7);
  push_int(vm, 8);

  printf("Number of objects before %d\n", vm->num_objects);
  
  pop(vm);
  pop(vm);

  gc(vm);

  printf("Resulting number of objects %d\n", vm->num_objects);

  free_vm(vm);
}
