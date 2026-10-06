#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }


    // Todo: set up IPC here (pipe or shared mem), Before fork
    struct timeval *shared_start = mmap(NULL, sizeof(struct timeval),
        PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_start == MAP_FAILED) {perror("mmap"); return 1;}

    pid_t pid = fork();
    if (pid < 0) 
    {
        perror("fork"); 
        munmap(shared_start, sizeof(struct timeval)); //small fix just clears possible memory leaks
        return 1;
    }

    // child

    if (pid == 0) 
    {
        struct timeval start;
        if (gettimeofday(&start, NULL) < 0) { perror("gettimeofday"); exit(1);}
        // Todo: send start to the parent
        *shared_start = start;
        execvp(argv[1], &argv[1]);
        perror("execvp");              // only runs if exec failed
        exit(1);
    }

    // parent
    wait(NULL);
    struct timeval end;
    if (gettimeofday(&end, NULL) < 0) {perror("gettimeofday"); return 1;}
    // todo: get start from child, compute elapsed, print with %.6f
    struct timeval start = *shared_start;
    double elapsed = (end.tv_sec - start.tv_sec)
        + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Elapsed time: %.6f seconds\n", elapsed);
    munmap(shared_start, sizeof(struct timeval));
    
    return 0;
}
