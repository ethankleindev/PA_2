========================================================================
Project Name: Interprocess Communication (IPC) Process Timing
Course: Computer Science - Systems Programming
Members: Ethan Klein, Gerson Mancia, Xammy Yang, Brian Nguyen, Chris Jose
========================================================================

1. OVERVIEW
------------------------------------------------------------------------
This project measures the elapsed time required to execute a command from the 
command line using two different ICPs written in C

  time_pipe.c: Uses Unix pipes
  time_shm.c: Uses shared memory(mmap)

  Both of these fork a child process. The parent process is responsible for
exclusively reading while the child is only writing. We first check if
the arguments are valid, and if they are, we create a single pipe that
forks into two file descriptors. We use the child fd to write the start
time at the beginning of the argument it then sends that to the parent function
and it will execute whatever function it was in the beginning. Then it will 
close itself. While this is happening the parent receives the data and stores 
it as the start time. Then the parent waits until the child is done executing 
and then it finds out the end time. It then subtracts them together and gets
the total time. It prints them out at the end.

2. EVIDENCE
------------------------------------------------------------------------
Shared memory version (time_shm.c):

  $ ./time_shm ls -l | tee time_shm_output.txt
  total 32
  -rw-r--r-- 1 chris chris   925 Oct  5 18:45 main.c
  -rwxr-xr-x 1 chris chris 16424 Oct  5 18:45 time
  -rw-r--r-- 1 chris chris  1688 Oct  5 18:45 time_pipe.c
  -rw-r--r-- 1 chris chris  1556 Oct  5 18:45 time_shm.c
  -rw-r--r-- 1 chris chris     0 Oct  5 18:45 time_shm_output.txt
  Elapsed time: 0.008345 seconds
  $ ./time_shm pwd | tee -a time_shm_output.txt
  /home/chris/PA_2
  Elapsed time: 0.004326 seconds
  $ ./time_shm sleep 1 | tee -a time_shm_output.txt
  Elapsed time: 1.003995 seconds

Pipe version (time_pipe.c):

  $ ./time_pipe ls -l | tee time_pipe_output.txt
  total 36
  -rw-r--r-- 1 chris chris   925 Oct  5 18:45 main.c
  -rwxr-xr-x 1 chris chris 16512 Oct  5 18:46 time
  -rw-r--r-- 1 chris chris  1688 Oct  5 18:45 time_pipe.c
  -rw-r--r-- 1 chris chris     0 Oct  5 18:46 time_pipe_output.txt
  -rw-r--r-- 1 chris chris  1556 Oct  5 18:45 time_shm.c
  -rw-r--r-- 1 chris chris   394 Oct  5 18:46 time_shm_output.txt
  Elapsed time: 0.003273 seconds
  $ ./time_pipe pwd | tee -a time_pipe_output.txt
  /home/chris/PA_2
  Elapsed time: 0.002175 seconds
  $ ./time_pipe sleep 1 | tee -a time_pipe_output.txt
  Elapsed time: 1.001940 seconds

Both sleep 1 runs measure about 1.00 seconds, confirming the timing works.

3. COMPILATION
------------------------------------------------------------------------
Compile both source files using gcc:

  gcc -Wall time_pipe.c -o time_pipe
  gcc -Wall time_shm.c -o time_shm


4. HOW TO RUN
------------------------------------------------------------------------
Run either executable followed by any command and its optional arguments:

  ./time_pipe ls -l
  ./time_shm sleep 1
  ./time_pipe ping -c 4 google.com

To capture output to a file (as required for submission):

  ./time_pipe ls -l | tee time_pipe_output.txt
  ./time_shm ls -l | tee time_shm_output.txt
