#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "user/user.h"

#include <stddef.h>

int
main(int argc, char* argv[]) {
    int input[34] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};
    int pipe_fd[2];
    int cnt = 34;

    while (cnt > 0) {
        pipe(pipe_fd);
        int pid = fork();
        if (pid == 0) {
            // Child
            close(pipe_fd[1]);
            int recv, curr_prime = 0;
            cnt = -1;
            while (read(pipe_fd[0], &recv, sizeof(int)) != 0) {
                if (cnt == -1) {
                    curr_prime = recv;
                    cnt = 0;
                } else if (recv % curr_prime != 0) {
                    input[cnt++] = recv;
                }
            }
            printf("prime %d\n", curr_prime);
            close(pipe_fd[0]);
        } else {
            // Parent
            int i;
            for (i = 0; i < 34; ++i) {
                write(pipe_fd[1], &(input[i]), sizeof(int));
            }
            close(pipe_fd[1]);
            wait(NULL);
            break;
        }
    }
    exit(0);
}
