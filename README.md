# mpist

Code for porting and running code from grid-based clusters in MPI environments. `mpist` allows unmodified binaries to be run on POSIX compliant clusters using MPI. `lib/mpigrid.h` is a thin shim that can add support for MPI-based parallelism to an already functioning program; see the example in `example`

## `mpist`
An MPI wrapper to allow the execution of many instances of a single-threaded program with a varrying envrionmental variable. Requires the presence of execv() and fork() syscalls and so will not function in Blue Gene (e.g. bg/q)  environments.

### Usage
`mpist [-h] [-G] -C ENVVAR -t n-m EXE [ARGS]`

Executes EXE [ARGS] with the environmental varaible specified by ENVVAR set to integers in the range [n,m] inclusive. If there are insufficient MPI ranks available, the top of the range is silently truncated.

-h displays this message.
-G prevents actually exec-ing and just displays status.

Notes:
* It is required that m > n
* The environmental variable, ENVVAR, must be shorter than 127 characters.

### Compiling
A simple call to make and will produce the binary. If xl compilers are available, they will be used.

### Example

One might submit a cobalt job with mpist as:
`% qsub -n 512 -t 20 mpist -C TASK_ID -t 1-8192 /path/to/my/single-thread/program [args...]`

## `mpigrid.h`

In environments where 

### Usage
`int getTaskID(const char f, int * argc, char ** argv)`

Parses command line arguments and returns a task ID based on the MPI rank. Expects an argument of the form -f n-m to sepcify tasks to be completed.  Inits MPI functions and arranges for MPI_Finalize to be called at exit.

If a '-f' option is present, getTaskID will shift the contents of argv and change the count in argc to hide the elements from future invocations of getopt(3).

If a '-f' option is present and the range cannot be parsed, the program will emit an error and terminate.

If there are more tasks than ranks, rank 0 will emit a warning. If there are more ranks than tasks, the superflous ranks will be killed silently.

### Example
`int task = getTaskID('f', &argc, argv);`

parses the command-line arguments, looking for a flag like -f n-m where n-m is the range to execute over. A unique int in [n,m] is returned for each rank.