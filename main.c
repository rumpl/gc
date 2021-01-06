#include <stdio.h>

#include "gc_mark_sweep.h"
#include "utils.h"
#include "vm.h"

int main() {
  baby_vm *vm = new_vm();


  push_int(vm, 1);
  push_int(vm, 2);
  push_int(vm, 3);
  push_int(vm, 4);
  push_int(vm, 5);
  push_int(vm, 6);
  push_pair(vm);
  push_int(vm, 7);
  push_pair(vm);

  printf("Number of objects before %d\n", vm->num_objects);

  print_stack(vm);

  pop(vm);
  pop(vm);

  gc(vm);

  print_stack(vm);

  printf("Resulting number of objects %d\n", vm->num_objects);

  free_vm(vm);
}
