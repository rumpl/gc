#ifndef _VM_H
#define _VM_H

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

baby_vm *new_vm();
void free_vm(baby_vm *);

void push(baby_vm *, baby_object *);
baby_object *pop(baby_vm *);

baby_object *new_object(baby_vm *, baby_object_type);

void push_int(baby_vm *, int);
void push_pair(baby_vm *);

#endif
