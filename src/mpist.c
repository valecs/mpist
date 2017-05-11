#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /*execv*/


void help(char * pname){
  printf("%s: An MPI wrapper to allow the execution of many instances of a\n
         single-threaded program.\n", pname);
}

int main(int argc, char ** argv){
  MPI_Init(NULL, NULL);
  help(argv[0]);
  MPI_Finalize();
  return EXIT_SUCCESS
}
