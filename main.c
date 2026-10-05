#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }


    // Todo: set up IPC here (pipe or shared mem), Before fork

    pid_t pid = fork();
    if (pid < 0) {perror("fork"); return 1;}

    // child

    if (pid == 0) 
    {
        struct timeval start;
        if (gettimeofday(&start, NULL) < 0) { perror("gettimeofday"); exit(1);}
        // Todo: send start to the parent
        execvp(argv[1], &argv[1]);
        perror("execvp");              // only runs if exec failed
        exit(1);
    }

    // parent
    wait(NULL);
    struct timeval end;
    if (gettimeofday(&end, NULL) < 0) {perror("gettimeofday"); return 1;}
    // todo: get start from child, compute elapsed, print with %.6f
    
    return 0;
}