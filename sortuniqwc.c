#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
        pid_t pid;
        int fd1[2], fd2[2];
        pipe(fd1);
        pid = fork();
        
        if (pid < 0) {
            // fork failed
            fprintf(stderr, "Fork failed\n");
            return 1;
        }
        if (pid == 0) {
            // first child proccess, run sort
            printf("First child process is %d\n", getpid());
            dup2(fd1[1], 1);
            close(fd1[0]);
            close(fd1[1]);
            execlp("/usr/bin/sort", "sort", NULL);
            printf("should not be here after execlp to sort\n");
        }
        pid = fork(); // second child process
        if (pid < 0) {
            // fork failed
            fprintf(stderr, "Fork failed\n");
            return 1;
        }
        if (pid == 0) {
            // second child process, run wc
            printf("Second child process is %d\n", getpid());
            // read first child's output from pipe
            dup2(fd1[0], 0);
            close(fd1[0]);
            close(fd1[1]);
            // out to second pipe
            dup2(fd2[1], 1);
            close(fd2[0]);
            execlp("/usr/bin/uniq", "uniq", NULL);
            printf("should not be here after execlp to uniq\n");
        }
        pid = fork(); // third child process
        if (pid < 0) {
            // fork failed
            fprintf(stderr, "Fork failed\n");
            return 1;
        }
        if (pid == 0) {
            // third child process, run uniq
            printf("Third child process is %d\n", getpid());
            //// 
            execlp("/usr/bin/wc", "wc", "-l", NULL);
            printf("should not be here after execlp to wc\n");
        }
}