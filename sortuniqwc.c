// Contributors: Mark, Nicolas, Daniyal, Joshua

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
        pid_t pid;
        int fd1[2], fd2[2];
        pipe(fd1);
        pipe(fd2);
        pid = fork();
        
        if (pid < 0) {
            // fork failed
            fprintf(stderr, "Fork failed\n");
            return 1;
        }
        if (pid == 0) {
            // first child proccess, run sort
            printf("The first child process running sort is %d\n", getpid());
            dup2(fd1[1], 1);
            close(fd1[0]);
            close(fd1[1]);
            execlp("/usr/bin/sort", "sort", NULL);
            printf("Should not be here after execlp to sort\n");
        }
        pid = fork(); // second child process
        if (pid < 0) {
            // fork failed
            fprintf(stderr, "Fork failed\n");
            return 1;
        }
        if (pid == 0) {
            // second child process, run wc
            printf("The second child process running uniq is %d\n", getpid());
            // read first child's output from pipe
            dup2(fd1[0], 0);
            close(fd1[0]);
            close(fd1[1]);
            // out to second pipe
            dup2(fd2[1], 1);
            close(fd2[0]);
            execlp("/usr/bin/uniq", "uniq", NULL);
            printf("Should not be here after execlp to uniq\n");
        }
        pid = fork(); // third child process
        if (pid < 0) {
            // fork failed
            fprintf(stderr, "Fork failed\n");
            return 1;
        }
        if (pid == 0) {
            // third child process, run uniq
            printf("The third child process running wc -l is %d\n", getpid());
            dup2(fd2[0], STDIN_FILENO);
            close(fd2[1]);
            close(fd1[0]);
            close(fd1[1]);
            execlp("/usr/bin/wc", "wc", "-l", NULL);
            printf("Should not be here after execlp to wc\n");
        }
        //Parent process code
        //Close ends of both pipes
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        // Wait for third process to end
        waitpid(pid, NULL, 0);
        printf("All child processes have completed.\n");
        return 0;
        
}
