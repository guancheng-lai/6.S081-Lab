#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#include <stddef.h>

int
main(int argc, char* argv[]) {
  int p[2];
  pipe(p);
  int pid = fork();
  if (pid == 0) {
    // Child 
    char data[5];
    read(p[0], data, 4);
    printf("%d: received %s\n", getpid(), data);
    write(p[1], "pong", 4);
    exit(0);
  } else if (pid > 0) {
    // Parent
    write(p[1], "ping", 4);
    wait(NULL);
    char data[5];
    read(p[0], data, 4);
    printf("%d: received %s\n", getpid(), data);
  } else {
    fprintf(2, "Failed to fork\n");
  }
  exit(0);
}