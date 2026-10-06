#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        return 1;
    }

    if (pid == 0) {
        close(fd[0]);

        struct timeval start;
        if (gettimeofday(&start, NULL) < 0) {
            perror("gettimeofday");
            close(fd[1]);
            exit(1);
        }

        if (write(fd[1], &start, sizeof(start)) != sizeof(start)) {
            perror("write"); // small error again. was wrtite now write.
            close(fd[1]);
            exit(1);
        }

        close(fd[1]);

        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(1);
    }

    close(fd[1]);

    //Fixed logic to make the time value start first then it ends. It was reversed

    struct timeval start;
    if (read(fd[0], &start, sizeof(start)) != sizeof(start)) {
        perror("read");
        close(fd[0]);
        return 1;
    }
    close(fd[0]); // finished reading from pipe

    wait(NULL); //wait for child to finish execution

    struct timeval end;
    if (gettimeofday(&end, NULL) <0) {
        perror("gettimeofday");
        return 1;
    }

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Elapsed time: %.6f seconds\n", elapsed); //uses printf not print, just a basic error

    return 0;
}
