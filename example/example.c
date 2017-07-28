#include "../lib/mpigrid.h"

#include <stdio.h>

int main(int argc, char ** argv){
  /* Tasks will be passed to the program as -f n-m */
  int task = getTaskID('f', &argc, argv);

  printf("Hello from task %d. My other args are:\n", task);

  for (int i = 0; i < argc; i++){
    printf("%02d: %s\n", i, argv[i]);
  }

  //doTheWork(task, argc, argv);
  
  return 0;
}
