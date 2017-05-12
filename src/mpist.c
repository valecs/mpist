#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

void help(char * pname);
void status(void);
bool processOptions(int argc, char ** argv);

#define BUFLEN 128

struct param_t{
  char * path;         // executable
  char ** args;        // its arguments
  char envvar[BUFLEN]; // environmental variable of interest
  int n;               // low range
  int m;               // high range
};

/*
  Global struct holding parameters from the command line.
*/
static struct param_t param = {
  .path = NULL,
  .args = NULL,
  .envvar = {0},
  .n = 0,
  .m = 0,
};

int main(int argc, char ** argv){
  bool go = processOptions(argc, argv);
  status();

  if (!go){
    return EXIT_SUCCESS;
  }
  
  MPI_Init(NULL, NULL);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  { // Set the environmental var to rank + n
    char eval[BUFLEN];
    snprintf(eval, (size_t) BUFLEN, "%d", param.n + rank);
    setenv(param.envvar, eval, true);
  }

  // if we're already past m, no need to run
  if (param.n + rank <= param.m){
    fprintf(stderr, "Launching from rank %d.\n", rank);
    int wstatus;
    pid_t pid = vfork();
    if (0 == pid){// Must make the execv immediately since using vfork.
      execv(param.path, param.args);
    }
    else{
      waitpid(pid, &wstatus, 0);
    }
  }
  
  MPI_Finalize();
  return EXIT_SUCCESS;
}


void help(char * pname){
  printf("%s: An MPI wrapper to allow the execution of many instances of a\n"
         "  single-threaded program.\n", pname);
  printf("Usage: %s [-h] [-G] -C ENVVAR -t n-m EXE [ARGS]\n", pname);
  printf("  Executes EXE [ARGS] with the environmental variable specified by\n"
	 "  ENVVAR set to integers in the range [n,m] inclusive. If there are\n"
	 "  insufficient MPI ranks available, the top of the range is truncated.\n");
  printf("  -h displays this message.\n");
  printf("  -G prevents actually exec-ing and just displays status.\n");
  printf("The environmental variable, ENVVAR must be shorter than %d characters.\n", BUFLEN - 1);
}

bool processOptions(int argc, char ** argv){
  if (1 == argc){
    help(argv[0]);
    exit(0);
  }

  bool go = true;
  
  int c;
  opterr = 0;  //since we do our own error-handling
  const char optstring[]="+hGC:t:";
  while (-1 != (c = getopt(argc,argv, optstring))){
    switch (c){
    case 'C':
      /*
	== case has insufficient space because of the NULL ('\0') char.
      */
      if (strlen(optarg) < BUFLEN){
	strncpy(param.envvar, optarg, (size_t) BUFLEN);
      } else {
	fprintf(stderr, "ENVVAR must be shorter than %d. Exiting!\n", BUFLEN - 1);
	exit(-2);
      }
      break;
    case 't':
      /*
	Short-circuit evaluation ensures n & m are both set prior to
	comparison.
      */
      if (2 != sscanf(optarg, "%d-%d", &param.n, &param.m) ||
	  param.n > param.m){
	fprintf(stderr, "Bad range specification. Exiting!\n");
	exit(-3);
      }
      break;
    case 'h':
      help(argv[0]);
      exit(0);
      break;
    case 'G':
      go = false;
      break;
    case '?':
      fprintf(stderr, "Option, -%c, is invalid. Exiting!\n", optopt);
      exit(-1);
    default:
      fprintf(stderr, "Internal error. Exiting!\n");
      exit(-1);
      break;
    }
  }

  /*
    After the POSIXLY-CORRECT run of getopt, the argument at optind is
    the executable and those that follow are its arguments.
  */
  param.path = argv[optind];
  param.args = argv + optind + 1;
  return go;
}

/*
  Displays the contents of param on stderr.
*/
void status(void){
  fprintf(stderr,"%s=[%d,%d] ", param.envvar, param.n, param.m);
  fprintf(stderr,"%s", param.path);
  for (size_t i = 0; param.args[i]; i++){
    fprintf(stderr," %s", param.args[i]);
  }
  fprintf(stderr, "\n");
}
