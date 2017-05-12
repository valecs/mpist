#ifndef MPIST_H
#define MPIST_H
#include <stdbool.h>

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

#endif
