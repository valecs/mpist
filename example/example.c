#include "../lib/mpigrid.h"

#include <stdio.h>
void usage(void);

const char token='f';

int main(int argc, char ** argv){
  if (1 == argc){usage(); return 0;}
  
  /* Tasks will be passed to the program as -f n-m */
  int task = getTaskID(token, &argc, argv);
  
  printf("Hello from task %d. My other args are:\n", task);

  for (int i = 0; i < argc; i++){
    printf("%02d: %s\n", i, argv[i]);
  }

  //doTheWork(task, argc, argv);
  
  return 0;
}

void usage(void){
  printf("grid-example [-%c n-m] [OTHER ARGS ...]\n"
	 , token);
}
